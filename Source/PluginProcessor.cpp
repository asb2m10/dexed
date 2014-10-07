/**
 *
 * Copyright (c) 2013 Pascal Gauthier.
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

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Dexed.h"
#include "msfa/synth.h"
#include "msfa/freqlut.h"
#include "msfa/sin.h"
#include "msfa/exp2.h"
#include "msfa/env.h"
#include "msfa/pitchenv.h"
#include "msfa/aligned_buf.h"
#include "msfa/fm_op_kernel.h"


//==============================================================================
DexedAudioProcessor::DexedAudioProcessor() {
#ifdef DEBUG
    Logger *tmp = Logger::getCurrentLogger();
    if ( tmp == NULL ) {
        Logger::setCurrentLogger(FileLogger::createDateStampedLogger("Dexed", "DebugSession-", "log", "DexedAudioProcessor Created"));
    }
    TRACE("Hi");
#endif

    Exp2::init();
    Tanh::init();
    Sin::init();

    lastStateSave = 0;
    
    currentNote = -1;
    vuSignal = 0;
    initCtrl();
    setCurrentProgram(0);
    sendSysexChange = true;
    normalizeDxVelocity = false;
    sysexComm.listener = this;
    engineType = -1;
    
    memset(&voiceStatus, 0, sizeof(VoiceStatus));

    prefOptions.applicationName = String("Dexed");
    prefOptions.filenameSuffix = String("xml");
    prefOptions.folderName = String("DigitalSuburban");
    prefOptions.osxLibrarySubFolder = String("Application Support");
    
    controllers.values_[kControllerPitchRange] = 3;
    controllers.values_[kControllerPitchStep] = 0;
    loadPreference();
    
    setEngineType(DEXED_ENGINE_MODERN);
    
    for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
        voices[note].dx7_note = NULL;
    }
    nextMidi = NULL;
    midiMsg = NULL;

}

DexedAudioProcessor::~DexedAudioProcessor() {
    TRACE("Bye");
}

//==============================================================================
void DexedAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    Freqlut::init(sampleRate);
    Lfo::init(sampleRate);
    PitchEnv::init(sampleRate);
    Env::init_sr(sampleRate);
    fx.init(sampleRate);
    
    for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
        voices[note].dx7_note = new Dx7Note;
        voices[note].keydown = false;
        voices[note].sustained = false;
        voices[note].live = false;
    }

    currentNote = 0;
    controllers.values_[kControllerPitch] = 0x2000;
    controllers.values_[kControllerModWheel] = 0;
    
    sustain = false;
    extra_buf_size = 0;

    keyboardState.reset();
    
    nextMidi = new MidiMessage(0xF0);
	midiMsg = new MidiMessage(0xF0);
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
    if ( nextMidi != NULL ) {
        delete nextMidi;
        nextMidi = NULL;
    }
    if ( midiMsg != NULL ) {
        delete midiMsg;
        midiMsg = NULL;
    }
}

void DexedAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
    int numSamples = buffer.getNumSamples();
    int i = 0;
    
    if ( refreshVoice ) {
        for(int i=0;i<MAX_ACTIVE_NOTES;i++) {
            if ( voices[i].live )
                voices[i].dx7_note->update(data, voices[i].midi_note);
        }
        lfo.reset(data + 137);
        refreshVoice = false;
    }

    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
    
    MidiBuffer::Iterator it(midiMessages);
    hasMidiMessage = it.getNextEvent(*nextMidi,midiEventPos);

    float *channelData = buffer.getSampleData(0);
  
    // flush first events
    for (i = 0; i < numSamples && i < extra_buf_size; i++) {
        channelData[i] = extra_buf[i];
    }
    
    // remaining buffer is still to be processed
    if (extra_buf_size > numSamples) {
        for (int j = 0; j < extra_buf_size - numSamples; j++) {
            extra_buf[j] = extra_buf[j + numSamples];
        }
        extra_buf_size -= numSamples;
        
        // flush the events, they will be process in the next cycle
        while(getNextEvent(&it, numSamples)) {
            processMidiMessage(midiMsg);
        }
    } else {
        for (; i < numSamples; i += N) {
            AlignedBuf<int32_t, N> audiobuf;
            float sumbuf[N];
            
            while(getNextEvent(&it, i)) {
                processMidiMessage(midiMsg);
            }
            
            for (int j = 0; j < N; ++j) {
                audiobuf.get()[j] = 0;
                sumbuf[j] = 0;
            }
            int32_t lfovalue = lfo.getsample();
            int32_t lfodelay = lfo.getdelay();
            
            for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
                if (voices[note].live) {
                    voices[note].dx7_note->compute(audiobuf.get(), lfovalue, lfodelay, &controllers);
                    
                    for (int j=0; j < N; ++j) {
                        int32_t val = audiobuf.get()[j]; //& 0xFFFFF000);
                        
                        val = val >> 4;
                        int clip_val = val < -(1 << 24) ? 0x8000 : val >= (1 << 24) ? 0x7fff : val >> 9;
                        float f = ((float) clip_val) / (float) 32768;
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
    
    while(getNextEvent(&it, numSamples)) {
        processMidiMessage(midiMsg);
    }

    fx.process(channelData, numSamples);
    for(int i=0; i<numSamples; i++) {
        float s = std::abs(channelData[i]);
        
        const double decayFactor = 0.99992;
        if (s > vuSignal)
            vuSignal = s;
        else if (vuSignal > 0.001f)
            vuSignal *= decayFactor;
        else
            vuSignal = 0;
    }
    
    // DX7 is a mono synth
    for (int channel = 1; channel < getNumInputChannels(); ++channel) {
        buffer.copyFrom(channel, 0, channelData, numSamples, 1);
    }

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new DexedAudioProcessor();
}

bool DexedAudioProcessor::getNextEvent(MidiBuffer::Iterator* iter,const int samplePos) {
	if (hasMidiMessage && midiEventPos <= samplePos) {
		*midiMsg = *nextMidi;
		hasMidiMessage = iter->getNextEvent(*nextMidi, midiEventPos);
		return true;
	}
	return false;
}

void DexedAudioProcessor::processMidiMessage(const MidiMessage *msg) {
    const uint8 *buf  = msg->getRawData();
    uint8_t cmd = buf[0];

    switch(cmd & 0xf0) {
        case 0x80 :
            keyup(buf[1]);
        return;

        case 0x90 :
            keydown(buf[1], buf[2]);
        return;

        case 0xb0 : {
            int controller = buf[1];
            int value = buf[2];
            
            // mod wheel
            if ( controller == 1 ) {
                controllers.values_[kControllerModWheel] = value;
                return;
            }
            
            // pedal
            if (controller == 64) {
                sustain = value != 0;
                if (!sustain) {
                    for (int note = 0; note < MAX_ACTIVE_NOTES; note++) {
                        if (voices[note].sustained && !voices[note].keydown) {
                            voices[note].dx7_note->keyup();
                            voices[note].sustained = false;
                        }
                    }
                }
                return;
            }
        }
        return;

        case 0xc0 :
            setCurrentProgram(buf[1]);
        return;        
    }

    switch (cmd) {
        case 0xe0 :
            controllers.values_[kControllerPitch] = buf[1] | (buf[2] << 7);
        break;
    }

}

void DexedAudioProcessor::keydown(uint8_t pitch, uint8_t velo) {
    if ( velo == 0 ) {
        keyup(pitch);
        return;
    }

    pitch += data[144] - 24;
    
    if ( normalizeDxVelocity ) {
        velo = ((float)velo) * 0.7874015; // 100/127
    }
    
    int note = currentNote;
    for (int i=0; i<MAX_ACTIVE_NOTES; i++) {
        if (!voices[note].keydown) {
            currentNote = (note + 1) % MAX_ACTIVE_NOTES;
            lfo.keydown();  // TODO: should only do this if # keys down was 0
            voices[note].midi_note = pitch;
            voices[note].sustained = sustain;
            voices[note].keydown = true;
            voices[note].dx7_note->init(data, pitch, velo);
            break;
        }
        note = (note + 1) % MAX_ACTIVE_NOTES;
    }
    
    if ( monoMode ) {
        for(int i=0; i<MAX_ACTIVE_NOTES; i++) {            
            if ( voices[i].live ) {
                // all keys are up, don't transfert anything
                if ( ! voices[i].keydown ) {
                    voices[i].live = false;
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
 
    voices[note].live = true;
}

void DexedAudioProcessor::keyup(uint8_t pitch) {
    pitch += data[144] - 24;

    int note;
    for (note=0; note<MAX_ACTIVE_NOTES; ++note) {
        if ( voices[note].midi_note == pitch && voices[note].keydown ) {
            voices[note].keydown = false;
            break;
        }
    }
    
    // note not found ?
    if ( note >= MAX_ACTIVE_NOTES ) {
        TRACE("note-off not found???");
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
        
        if ( highNote != -1 ) {
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
        voices[i].keydown = false;
        voices[i].live = false;
    }
    keyboardState.reset();
}

void DexedAudioProcessor::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) {
    if ( message.isActiveSense() ) 
        return;

    sysexComm.inActivity = true;

    if ( ! message.isSysEx() )
        return;

    //const uint8 *buf = msg->getSysExData();
    const uint8 *buf = message.getRawData();
    int sz = message.getRawDataSize();

    if ( sz < 3 )
        return;

    TRACE("SYSEX RECEIVED %d", sz);

    // test if it is a Yamaha Sysex
    if ( buf[1] != 0x43 ) {
        TRACE("not a yamaha sysex %d", buf[0]);
        return;
    }
        
    // single voice dump
    if ( buf[3] == 0 ) {
         if ( sz < 155 ) {
            TRACE("wrong single voice datasize %d", sz);
            return;
        }

        TRACE("program update sysex");
        updateProgramFromSysex(buf+6);
        String name = normalizeSysexName((const char *) buf+151);
        packProgram((uint8_t *) sysex, (uint8_t *) data, currentProgram, name); 
        programNames.set(currentProgram, name);
    }

    // 32 voice dump
    if ( buf[3] == 9 ) {
        if ( sz < 4104 ) {
            TRACE("wrong 32 voice datasize %d", sz);
            return;
        }
        TRACE("update 32bulk voice");
        importSysex((const char *)buf);
        setCurrentProgram(0);
    }

    updateHostDisplay();
    forceRefreshUI = true;
}

int DexedAudioProcessor::getEngineType() {
    return engineType;
}

void DexedAudioProcessor::setEngineType(int tp) {
    switch (tp)  {
        case DEXED_ENGINE_MODERN :
            controllers.core = &engineMsfa;
            break;
        case DEXED_ENGINE_MARKI:
            controllers.sinBitFilter = 0xFFFFC000;  // 10 bit
            controllers.dacBitFilter = 0xFFFFF000;  // semi 14 bit
            controllers.core = &engineMkI;
            break;
        case DEXED_ENGINE_OPL:
            controllers.core = &engineOpl;
            break;
    }
    engineType = tp;
}

void DexedAudioProcessor::setMonoMode(bool mode) {
    panic();
    monoMode = mode;
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

bool DexedAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DexedAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
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

void DexedAudioProcessor::updateUI() {
    // notify host something has changed
    updateHostDisplay();
 
    AudioProcessorEditor *editor = getActiveEditor();
    if ( editor == NULL ) {
        return;
    }
	DexedAudioProcessorEditor *dexedEditor = (DexedAudioProcessorEditor *) editor;
    dexedEditor->updateUI();
}

AudioProcessorEditor* DexedAudioProcessor::createEditor() {
    return new DexedAudioProcessorEditor (this);
}

void DexedAudioProcessor::handleAsyncUpdate() {
    updateUI();
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

