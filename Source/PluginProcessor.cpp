/**
 *
 * Copyright (c) 2013 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "msfa/synth.h"
#include "msfa/freqlut.h"
#include "msfa/sin.h"
#include "msfa/exp2.h"
#include "msfa/pitchenv.h"
#include "msfa/aligned_buf.h"


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
    workBlock = NULL;
    initCtrl();
    setCurrentProgram(0);
    sendSysexChange = true;
    normalizeDxVelocity = false;

    memset(&voiceStatus, 0, sizeof(VoiceStatus));

    prefOptions.applicationName = String("Dexed");
    prefOptions.filenameSuffix = String("xml");
    prefOptions.folderName = String("DigitalSuburban");
    prefOptions.osxLibrarySubFolder = String("Application Support");
}

DexedAudioProcessor::~DexedAudioProcessor() {
    TRACE("Bye");
}

//==============================================================================
void DexedAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    Freqlut::init(sampleRate);
    Lfo::init(sampleRate);
    PitchEnv::init(sampleRate);
    fx.init(sampleRate);
    
    for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
        voices[note].dx7_note = new Dx7Note;
        voices[note].keydown = false;
        voices[note].sustained = false;
        voices[note].live = false;
    }

    currentNote = 0;
    controllers.values_[kControllerPitch] = 0x2000;
    controllers.values_[KcontrollerModWheel] = 0;
    sustain = false;
    extra_buf_size = 0;

    workBlockSize = samplesPerBlock;
    workBlock = new SInt16[samplesPerBlock];

    keyboardState.reset();
}

void DexedAudioProcessor::releaseResources() {
    currentNote = -1;

    for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
        delete voices[note].dx7_note;        
        voices[note].keydown = false;
        voices[note].sustained = false;
        voices[note].live = false;
    }

    if ( workBlock != NULL ) {
        delete workBlock;
    }

    keyboardState.reset();
}

void DexedAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
    int numSamples = buffer.getNumSamples();
    
    if ( refreshVoice ) {
        for(int i=0;i<MAX_ACTIVE_NOTES;i++) {
            if ( voices[i].live )
                voices[i].dx7_note->update(data, voices[i].midi_note);
        }
        lfo.reset(data + 137);
        refreshVoice = false;
    }

    // check buffer size
    if ( numSamples > workBlockSize ) {
        delete workBlock;
        workBlockSize = numSamples;
        workBlock = new SInt16[workBlockSize];
    }

    // Now pass any incoming midi messages to our keyboard state object, and let it
    // add messages to the buffer if the user is clicking on the on-screen keys
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);
    
    // check input
    MidiBuffer::Iterator it(midiMessages);
    MidiMessage msg;
    int pos;
    
    while(it.getNextEvent(msg, pos)) {
        processMidiMessage(&msg);
    }
    midiMessages.clear();

    processSamples(numSamples, workBlock);
    float *channelData = buffer.getSampleData(0);
    for(int i = 0; i < numSamples; i++ ) {
        float f = ((float) workBlock[i]) / (float) 32768;
        if( f > 1 ) f = 1;
        if( f < -1 ) f = -1;
        channelData[i] = (double) f;
   }

    fx.process(channelData, numSamples);
    
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

    if ( ! midiOut.isEmpty() ) {
        midiMessages.swapWith(midiOut);
    }
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new DexedAudioProcessor();
}

void DexedAudioProcessor::processMidiMessage(MidiMessage *msg) {
    if ( msg->isSysEx() ) {

        const uint8 *buf = msg->getSysExData();
        int sz = msg->getSysExDataSize();
        TRACE("SYSEX RECEIVED %d", sz);
        if ( sz < 3 )
            return;

        // test if it is a Yamaha Sysex
        if ( buf[0] != 0x43 ) {
            TRACE("not a yamaha sysex %d", buf[0]);
            return;
        }
        
        // single voice dump
        if ( buf[2] == 0 ) {
            if ( sz < 155 ) {
                TRACE("wrong single voice datasize %d", sz);
                return;
            }
            TRACE("program update sysex");
            updateProgramFromSysex(buf+4);
            triggerAsyncUpdate();
            return;
        }

        // 32 voice dump
        if ( buf[2] == 9 ) {
            if ( sz < 4016 ) {
                TRACE("wrong 32 voice datasize %d", sz);
                return;
            }
            TRACE("update 32bulk voice)");
            importSysex((const char *)buf+4);
            currentProgram = 0;
            triggerAsyncUpdate();
        }
        return;
    }

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
                controllers.values_[KcontrollerModWheel] = value;
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
            controllers.values_[kControllerPitch] = (char) buf[1] | (buf[2] << 7);
        break;
    }

}

void DexedAudioProcessor::keydown(uint8_t pitch, uint8_t velo) {
    if ( velo == 0 ) {
        keyup(pitch);
        return;
    }

    if ( normalizeDxVelocity ) {
        velo = ((float)velo) * 0.7874015; // 100/127
    }
    
    int note = currentNote;
    for (int i = 0; i < MAX_ACTIVE_NOTES; i++) {
        if (!voices[note].keydown) {
            currentNote = (note + 1) % MAX_ACTIVE_NOTES;
            lfo.keydown();  // TODO: should only do this if # keys down was 0
            voices[note].midi_note = pitch;
            voices[note].keydown = true;
            voices[note].sustained = sustain;
            voices[note].live = true;
            voices[note].dx7_note->init(data, pitch, velo);
            return;
        }
        note = (note + 1) % MAX_ACTIVE_NOTES;
    }

}

void DexedAudioProcessor::keyup(uint8_t pitch) {
    for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
        if (voices[note].midi_note == pitch && voices[note].keydown) {
            if (sustain) {
                voices[note].sustained = true;
            } else {
                voices[note].dx7_note->keyup();
            }
            voices[note].keydown = false;
        }
    }
}


void DexedAudioProcessor::processSamples(int n_samples, int16_t *buffer) {
    int i;
    for (i = 0; i < n_samples && i < extra_buf_size; i++) {
        buffer[i] = extra_buf[i];
    }
    if (extra_buf_size > n_samples) {
        for (int j = 0; j < extra_buf_size - n_samples; j++) {
            extra_buf[j] = extra_buf[j + n_samples];
        }
        extra_buf_size -= n_samples;
        return;
    }
    
    for (; i < n_samples; i += N) {
        AlignedBuf<int32_t, N> audiobuf;
        for (int j = 0; j < N; ++j) {
            audiobuf.get()[j] = 0;
        }
        int32_t lfovalue = lfo.getsample();
        int32_t lfodelay = lfo.getdelay();
        for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
            if (voices[note].live) {
                voices[note].dx7_note->compute(audiobuf.get(), lfovalue, lfodelay, &controllers);
            }
        }
        int jmax = n_samples - i;
        for (int j = 0; j < N; ++j) {
            int32_t val = audiobuf.get()[j] >> 4;
            int clip_val = val < -(1 << 24) ? 0x8000 : val >= (1 << 24) ? 0x7fff :
            val >> 9;
            // val = val & 0x0FFF7000;
            // TODO: maybe some dithering?
            if (j < jmax) {
                buffer[i + j] = clip_val;
            } else {
                extra_buf[j - jmax] = clip_val;
            }
        }
    }
    extra_buf_size = i - n_samples;
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

