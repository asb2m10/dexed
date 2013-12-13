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

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "msfa/controllers.h"
#include "msfa/dx7note.h"
#include "msfa/lfo.h"
#include "msfa/synth.h"
#include "PluginParam.h"

struct ProcessorVoice {
    int midi_note;
    bool keydown;
    bool sustained;
    bool live;
    Dx7Note *dx7_note;
};

//==============================================================================
/**
*/
class DexedAudioProcessor  : public AudioProcessor
{
    static const int MAX_ACTIVE_NOTES = 16;
    ProcessorVoice voices[MAX_ACTIVE_NOTES];
    int currentNote;

    // The original DX7 had one single LFO. Later units had an LFO per note.
    Lfo lfo;

    // in MIDI units (0x4000 is neutral)
    Controllers controllers;

    bool sustain;

    // Extra buffering for when GetSamples wants a buffer not a multiple of N
    int16_t extra_buf[N];
    int extra_buf_size;

    SInt16 *workBlock;
    int workBlockSize;

    int currentProgram;

    char sysex[4096];
    char patchNames[32][13];
    void unpackSysex(int idx);

public :
    bool isDirty;
    Array<Ctrl*> ctrl;

    // PARAMETER CONTAINER
    OperatorCtrl opCtrl[6];
    ScopedPointer<CtrlInt> pitchEgRate[4];
    ScopedPointer<CtrlInt> pitchEgLevel[4];

    ScopedPointer<CtrlInt> algo;
    ScopedPointer<CtrlInt> lfoRate;
    ScopedPointer<CtrlInt> lfoDelay;
    ScopedPointer<CtrlInt> lfoAmpDepth;
    ScopedPointer<CtrlInt> lfoPitchDepth;
    ScopedPointer<CtrlInt> lfoWaveform;
    ScopedPointer<CtrlInt> lfoSync;

    void initCtrl();

    int importSysex(const char *imported);

    String getPatchName(int idx);
    void setDxValue(int offset, int v);
    char data[161];

    void unbindUI();

    
    void processMidiMessage(const uint8_t *buf, int buf_size);
    void keydown(uint8_t pitch, uint8_t velo);
    void keyup(uint8_t pitch);
    void processSamples(int n_samples, int16_t *buffer);
    
public:
    //==============================================================================
    DexedAudioProcessor();
    ~DexedAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;
    void updateUI();
    bool peekEnvStatus(int32_t *values);

    //==============================================================================
    const String getName() const;
    int getNumParameters();
    float getParameter (int index);
    void setParameter (int index, float newValue);
    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    void getCurrentProgramStateInformation (MemoryBlock& destData);
    void setCurrentProgramStateInformation (const void* data, int sizeInBytes);
    //==============================================================================
    
    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState keyboardState;

    static void log(const char *source, const char *fmt, ...);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DexedAudioProcessor)

};

#define TRACE(fmt, ...) DexedAudioProcessor::log(__PRETTY_FUNCTION__,fmt,##__VA_ARGS__)

#endif  // PLUGINPROCESSOR_H_INCLUDED
