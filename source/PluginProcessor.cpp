/**
 *
 * Copyright (c) 2013-2025 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

#include <stdarg.h>
#include <bitset>

#include "PluginProcessor.h"
#include "ui/PluginEditor.h"
#include "parameter/Model.h"

#include "Dexed.h"
#include "msfa/synth.h"
#include "msfa/freqlut.h"
#include "msfa/sin.h"
#include "msfa/exp2.h"
#include "msfa/env.h"
#include "msfa/pitchenv.h"
#include "msfa/porta.h"
#include "msfa/aligned_buf.h"
#include "msfa/fm_op_kernel.h"

#if JUCE_MSVC
    #pragma comment (lib, "kernel32.lib")
    #pragma comment (lib, "user32.lib")
    #pragma comment (lib, "wininet.lib")
    #pragma comment (lib, "advapi32.lib")
    #pragma comment (lib, "ws2_32.lib")
    #pragma comment (lib, "version.lib")
    #pragma comment (lib, "shlwapi.lib")
    #pragma comment (lib, "winmm.lib")
	#pragma comment (lib, "DbgHelp.lib")
	#pragma comment (lib, "Imm32.lib")

	#ifdef _NATIVE_WCHAR_T_DEFINED
		#ifdef _DEBUG
			#pragma comment (lib, "comsuppwd.lib")
		#else
			#pragma comment (lib, "comsuppw.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment (lib, "comsuppd.lib")
		#else
			#pragma comment (lib, "comsupp.lib")
		#endif
	#endif

#endif

//==============================================================================
DexedAudioProcessor::DexedAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("output", AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr) {
    parameters.rootVt.addListener(this);

    mapParameters();

    Exp2::init();
    Tanh::init();
    Sin::init();

    synthTuningState = createStandardTuning();
    synthTuningStateLast = createStandardTuning();

    currentNote = -1;
    engineType = -1;
    
    vuSignal = 0;
    monoMode = 0;

    resolvAppDir();
    
    sendSysexChange = true;
    normalizeDxVelocity = false;
    sysexComm.listener = this;
    showKeyboard = true;
    
    memset(&voiceStatus, 0, sizeof(VoiceStatus));
    setEngineType(DEXED_ENGINE_MARKI);
    
    controllers.values_[kControllerPitchRangeUp] = 3;
    controllers.values_[kControllerPitchRangeDn] = 3;
    controllers.values_[kControllerPitchStep] = 0;
    controllers.masterTune = 0;
    
    loadPreference();

    for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
        voices[note].dx7_note = NULL;
    }

    setupStartupCart();
    setCurrentProgram(0);

    mtsClient = NULL;
    mtsClient = MTS_RegisterClient();
}

DexedAudioProcessor::~DexedAudioProcessor() {
    Logger *tmp = Logger::getCurrentLogger();
	if ( tmp != NULL ) {
		Logger::setCurrentLogger(NULL);
		delete tmp;
	}
    TRACE("Bye");
    if (mtsClient) MTS_DeregisterClient(mtsClient);
}

//==============================================================================
void DexedAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    Freqlut::init(sampleRate);
    Lfo::init(sampleRate);
    PitchEnv::init(sampleRate);
    Env::init_sr(sampleRate);
    Porta::init_sr(sampleRate);
    fx.init(sampleRate);

    vuDecayFactor = VuMeterOutput::getDecayFactor(sampleRate);
    
    for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
        voices[note].dx7_note = new Dx7Note(synthTuningState, mtsClient);
        voices[note].midi_note = -1;
        voices[note].keydown = false;
        voices[note].sustained = false;
        voices[note].live = false;
        voices[note].keydown_seq = -1;
    }

    currentNote = 0;
    nextKeydownSeq = 0;
    controllers.values_[kControllerPitch] = 0x2000;
    controllers.modwheel_cc = 0;
    controllers.foot_cc = 0;
    controllers.breath_cc = 0;
    controllers.aftertouch_cc = 0;
    controllers.portamento_enable_cc = false;
    controllers.portamento_cc = 0;
	controllers.refresh();

    sustain = false;
    extra_buf_size = 0;

    keyboardState.reset();
    
    lfo.reset(activeProgram.getParameters(137));

    command.reset();
}

void DexedAudioProcessor::releaseResources() {
    currentNote = -1;

    for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
        if ( voices[note].dx7_note != NULL ) {
            delete voices[note].dx7_note;
            voices[note].dx7_note = NULL;
        }
        voices[note].keydown = false;
        voices[note].sustained = false;
        voices[note].live = false;
    }

    keyboardState.reset();
}

void DexedAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    int numSamples = buffer.getNumSamples();
    int i;

    command.call(*this);

    if ( refreshVoice.compareAndSetBool(false, true) ) {
        for(i=0;i < MAX_ACTIVE_NOTES;i++) {
            if ( voices[i].live )
                voices[i].dx7_note->update(activeProgram.getRawData(), voices[i].midi_note, voices[i].velocity, voices[i].channel);
        }
        lfo.reset(activeProgram.getParameters(137));
    }

    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

    juce::MidiBufferIterator it = midiMessages.begin(), end = midiMessages.end();
    float *channelData = buffer.getWritePointer(0);
  
    // flush first events
    for (i=0; i < numSamples && i < extra_buf_size; i++) {
        channelData[i] = extra_buf[i];
    }
    
    // remaining buffer is still to be processed
    if (extra_buf_size > numSamples) {
        for (int j = 0; j < extra_buf_size - numSamples; j++) {
            extra_buf[j] = extra_buf[j + numSamples];
        }
        extra_buf_size -= numSamples;
        
        // flush the events, they will be process in the next cycle
        processMidiMessages(it, end, numSamples);
    } else {
        for (; i < numSamples; i += N) {
            AlignedBuf<int32_t, N> audiobuf;
            float sumbuf[N];

            processMidiMessages(it, end, i);

            for (int j = 0; j < N; ++j) {
                audiobuf.get()[j] = 0;
                sumbuf[j] = 0;
            }
            int32_t lfovalue = lfo.getsample();
            int32_t lfodelay = lfo.getdelay();
            
            bool checkMTSESPRetuning = synthTuningState->is_standard_tuning() &&
                                        MTS_HasMaster(mtsClient);
            
            for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
                if (voices[note].live) {
                    
                    if (checkMTSESPRetuning)
                        voices[note].dx7_note->updateBasePitches();
                    
                    voices[note].dx7_note->compute(audiobuf.get(), lfovalue, lfodelay, &controllers);
                    
                    for (int j=0; j < N; ++j) {
                        int32_t val = audiobuf.get()[j];
                        
                        val = val >> 4;
                        int clip_val = val < -(1 << 24) ? 0x8000 : val >= (1 << 24) ? 0x7fff : val >> 9;
                        float f = ((float) clip_val) / (float) 0x8000;
                        if( f > 1 ) f = 1;
                        if( f < -1 ) f = -1;
                        sumbuf[j] += f;
                        audiobuf.get()[j] = 0;
                    }
                }
            }
            
            int jmax = numSamples - i;
            for (int j = 0; j < N; ++j) {
                if (j < jmax) {
                    channelData[i + j] = sumbuf[j];
                } else {
                    extra_buf[j - jmax] = sumbuf[j];
                }
            }
        }
        extra_buf_size = i - numSamples;
    }

    processMidiMessages(it, end, numSamples);
    fx.process(channelData, numSamples);

    for(i=0; i<numSamples; i++) {
        float s = std::abs(channelData[i]);

        if (s > vuSignal)
            vuSignal = s;
        else if (vuSignal > /*0.001f*/ 1.26E-4F) // 1.26E-4 is equivalent to -39 dB, the min amplitude associated to leftmost LED
            vuSignal *= vuDecayFactor;
        else
            vuSignal = 0;
    }
    
    // DX7 is a mono synth, but copy it to the right channel is available
    if ( buffer.getNumChannels() > 1 )
        buffer.copyFrom(1, 0, channelData, numSamples, 1);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new DexedAudioProcessor();
}

#define ACT(v) (v.keydown ? v.midi_note : -1)

int DexedAudioProcessor::chooseNote(uint8_t pitch) {
    // order of preference:
    // 1. a note that is not playing
    // 2. a note with its key up, playing the same pitch
    // 3. a note with its key up, playing a different pitch
    // 4. a note with its key down, playing the same pitch
    // 5. a note with its key down, playing a different pitch
    // break ties by preferring note with least recent keydown
    int bestNote = currentNote;
    int bestScore = -1;
    int note = currentNote;
    for (int i=0; i<MAX_ACTIVE_NOTES; i++) {
        int score = 0;
        if ( !voices[note].dx7_note->isPlaying() ) score += 4;
        if ( !voices[note].keydown ) score += 2;
        if ( voices[note].midi_note == pitch ) score += 1;
        if ( (score > bestScore) || (score == bestScore && voices[note].keydown_seq < voices[bestNote].keydown_seq) ) {
            bestNote = note;
            bestScore = score;
        }
        note = (note + 1) % MAX_ACTIVE_NOTES;
    }
    return bestNote;
}

void DexedAudioProcessor::keydown(uint8_t channel, uint8_t pitch, uint8_t velo) {
    if ( velo == 0 ) {
        keyup(channel, pitch, velo);
        return;
    }

    pitch += tuningTranspositionShift();
    
    if ( normalizeDxVelocity ) {
        velo = ((float)velo) * 0.7874015; // 100/127
    }
  

    if( controllers.mpeEnabled ) {
        int note = currentNote;
        for( int i=0; i<MAX_ACTIVE_NOTES; ++i ) {
            if( voices[note].keydown && voices[note].channel == channel )
            {
                // If we get two keydowns on the same channel we are getting information from a non-mpe device
                controllers.mpeEnabled = false;
            }
            note = (note + 1) % MAX_ACTIVE_NOTES;
        }
    }

    bool triggerLfo = true;
    for (int i=0; i<MAX_ACTIVE_NOTES; i++) {
        if ( voices[i].keydown ) {
            triggerLfo = false;
            break;
        }
    }
    if ( triggerLfo ) {
        lfo.keydown();
    }

    int note = chooseNote(pitch);

    currentNote = (note + 1) % MAX_ACTIVE_NOTES;
    voices[note].channel = channel;
    voices[note].midi_note = pitch;
    voices[note].velocity = velo;
    voices[note].sustained = sustain;
    voices[note].keydown = true;
    voices[note].keydown_seq = nextKeydownSeq++;
    // to avoid click, don't sync oscillators when voice stealing
    bool voice_steal = voices[note].dx7_note->isPlaying();
    voices[note].dx7_note->init(activeProgram.getRawData(), pitch, velo, channel, &controllers);
    if ( activeProgram[IDs::oscKeySync.pos] && !voice_steal ) {
        voices[note].dx7_note->oscSync();
    }
    if ( (voices[lastActiveVoice].midi_note != -1 && controllers.portamento_enable_cc)
       && controllers.portamento_cc > 0 ) {
        voices[note].dx7_note->initPortamento(*voices[lastActiveVoice].dx7_note);
    }

    if ( monoMode ) {
        for(int i=0; i<MAX_ACTIVE_NOTES; i++) {            
            if ( voices[i].live ) {
                // all keys are up, only transfer signal
                if ( ! voices[i].keydown ) {
                    voices[i].live = false;
                    voices[note].dx7_note->transferSignal(*voices[i].dx7_note);
                    break;
                }
                if ( voices[i].midi_note < pitch ) {
                    voices[i].live = false;
                    voices[note].dx7_note->transferState(*voices[i].dx7_note);
                    break;
                }
                return;
            }
        }
    }
    else if ( !activeProgram[IDs::oscKeySync.pos] ) {
        // if another note at the same pitch is playing, transfer phase
        // to avoid unpredictable destructive interference. this can cause
        // clicking when voice stealing, but we've tried to choose a voice
        // to steal that will minimise the chances of clicking
        for(int i=0; i<MAX_ACTIVE_NOTES; i++) {
            if ( i != note && voices[i].dx7_note->isPlaying() && voices[i].midi_note == pitch ) {
                voices[note].dx7_note->transferPhase(*voices[i].dx7_note);
                break;
            }
        }
    }

 
    voices[note].live = true;
    lastActiveVoice = note;
	//TRACE("activate %d [ %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ]", pitch, ACT(voices[0]), ACT(voices[1]), ACT(voices[2]), ACT(voices[3]), ACT(voices[4]), ACT(voices[5]), ACT(voices[6]), ACT(voices[7]), ACT(voices[8]), ACT(voices[9]), ACT(voices[10]), ACT(voices[11]), ACT(voices[12]), ACT(voices[13]), ACT(voices[14]), ACT(voices[15]));
}

void DexedAudioProcessor::keyup(uint8_t chan, uint8_t pitch, uint8_t velo) {
    pitch += tuningTranspositionShift();

    int note;
    for (note=0; note<MAX_ACTIVE_NOTES; ++note) {
        if ( ( ( controllers.mpeEnabled && voices[note].channel == chan ) || // MPE node - find voice by channel
               (!controllers.mpeEnabled && voices[note].midi_note == pitch ) ) && // regular mode find voice by pitch
             voices[note].keydown ) // but still only grab the one which is keydown
        {
            voices[note].keydown = false;
			//TRACE("deactivate %d [ %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ]", pitch, ACT(voices[0]), ACT(voices[1]), ACT(voices[2]), ACT(voices[3]), ACT(voices[4]), ACT(voices[5]), ACT(voices[6]), ACT(voices[7]), ACT(voices[8]), ACT(voices[9]), ACT(voices[10]), ACT(voices[11]), ACT(voices[12]), ACT(voices[13]), ACT(voices[14]), ACT(voices[15]));
            break;
        }
    }
    
    // note not found ?
    if ( note >= MAX_ACTIVE_NOTES ) {
		TRACE("note found ??? %d [ %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ]", pitch, ACT(voices[0]), ACT(voices[1]), ACT(voices[2]), ACT(voices[3]), ACT(voices[4]), ACT(voices[5]), ACT(voices[6]), ACT(voices[7]), ACT(voices[8]), ACT(voices[9]), ACT(voices[10]), ACT(voices[11]), ACT(voices[12]), ACT(voices[13]), ACT(voices[14]), ACT(voices[15]));
        return;
    }
    
    if ( monoMode ) {
        int highNote = -1;
        int target = 0;
        for (int i=0; i<MAX_ACTIVE_NOTES;i++) {
            if ( voices[i].keydown && voices[i].midi_note > highNote ) {
                target = i;
                highNote = voices[i].midi_note;
            }
        }
        
        if ( highNote != -1 && voices[note].live ) {
            voices[note].live = false;
            voices[target].live = true;
            voices[target].dx7_note->transferState(*voices[note].dx7_note);
        }
    }
    
    if ( sustain ) {
        voices[note].sustained = true;
    } else {
        voices[note].dx7_note->keyup();
    }
}

void DexedAudioProcessor::panic() {
    for(int i=0;i<MAX_ACTIVE_NOTES;i++) {
        voices[i].midi_note = -1;
        voices[i].keydown = false;
        voices[i].live = false;
        if ( voices[i].dx7_note != NULL ) {
            voices[i].dx7_note->oscSync();
        }
    }
    keyboardState.reset();
}

int DexedAudioProcessor::getEngineType() {
    return engineType;
}

void DexedAudioProcessor::setEngineType(int tp) {
    TRACE("settings engine %d", tp);

    switch (tp)  {
        case DEXED_ENGINE_MARKI:
            controllers.core = &engineMkI;
            break;
        case DEXED_ENGINE_OPL:
            controllers.core = &engineOpl;
            break;
        default:
            controllers.core = &engineMsfa;
            break;
    }
    engineType = tp;
}

void DexedAudioProcessor::resetMonoMode() {
    panic();
    monoMode = parameters.getParameter(IDs::monoMode.name)->getValue() > 0.5f;
}

// ====================================================================
bool DexedAudioProcessor::peekVoiceStatus() {
    if ( currentNote == -1 )
        return false;

    // we are trying to find the last "keydown" note
    int note = currentNote;
    for (int i = 0; i < MAX_ACTIVE_NOTES; i++) {
        if (voices[note].keydown) {
            voices[note].dx7_note->peekVoiceStatus(voiceStatus);
            return true;
        }
        if ( --note < 0 )
            note = MAX_ACTIVE_NOTES-1;
    }

    // not found; try a live note
    note = currentNote;
    for (int i = 0; i < MAX_ACTIVE_NOTES; i++) {
        if (voices[note].live) {
            voices[note].dx7_note->peekVoiceStatus(voiceStatus);
            return true;
        }
        if ( --note < 0 )
            note = MAX_ACTIVE_NOTES-1;
    }

    return true;
}

const String DexedAudioProcessor::getInputChannelName (int channelIndex) const {
    return String (channelIndex + 1);
}

const String DexedAudioProcessor::getOutputChannelName (int channelIndex) const {
    return String (channelIndex + 1);
}

bool DexedAudioProcessor::isInputChannelStereoPair (int index) const {
    return true;
}

bool DexedAudioProcessor::isOutputChannelStereoPair (int index) const {
    return true;
}

bool DexedAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
    return layouts.getMainOutputChannelSet() == AudioChannelSet::mono()
                || layouts.getMainOutputChannelSet() == AudioChannelSet::stereo();
}

bool DexedAudioProcessor::acceptsMidi() const {
    return true;
}

bool DexedAudioProcessor::producesMidi() const {
    return true;
}

bool DexedAudioProcessor::silenceInProducesSilenceOut() const {
    return false;
}

double DexedAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

const String DexedAudioProcessor::getName() const {
    return JucePlugin_Name;
}

//==============================================================================
bool DexedAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DexedAudioProcessor::createEditor() {
    AudioProcessorEditor* editor = new DexedAudioProcessorEditor (this);
    return editor;
}

void DexedAudioProcessor::setZoomFactor(float factor) {
    zoomFactor = factor;
}

void DexedAudioProcessor::applyProgram(const Program &program) {
    panic();
    activeProgram = program;
    activeProgram.pushToParameters(parameters);
    lfo.reset(activeProgram.getParameters(IDs::lfoRate.pos));
}

void dexed_trace(const char *source, const char *fmt, ...) {
    char output[4096];
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(output, 4095, fmt, argptr);
    va_end(argptr);

    String dest;
    dest << source << " " << output;
    Logger::writeToLog(dest);
}
