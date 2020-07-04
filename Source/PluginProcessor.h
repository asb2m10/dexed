/**
 *
 * Copyright (c) 2013-2017 Pascal Gauthier.
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

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "msfa/controllers.h"
#include "msfa/dx7note.h"
#include "msfa/lfo.h"
#include "msfa/synth.h"
#include "msfa/fm_core.h"
#include "msfa/tuning.h"
#include "PluginParam.h"
#include "PluginData.h"
#include "PluginFx.h"
#include "SysexComm.h"
#include "EngineMkI.h"
#include "EngineOpl.h"

struct ProcessorVoice {
    int channel;
    int midi_note;
    int velocity;
    bool keydown;
    bool sustained;
    bool live;

    int mpePitchBend;
    int mpePressure;
    int mpeTimbre;
    
    Dx7Note *dx7_note;
};

enum DexedEngineResolution {
    DEXED_ENGINE_MODERN,
    DEXED_ENGINE_MARKI,
    DEXED_ENGINE_OPL
};

//==============================================================================
/**
*/
class DexedAudioProcessor  : public AudioProcessor, public AsyncUpdater, public MidiInputCallback
{
    static const int MAX_ACTIVE_NOTES = 16;
    ProcessorVoice voices[MAX_ACTIVE_NOTES];
    int currentNote;

    // The original DX7 had one single LFO. Later units had an LFO per note.
    Lfo lfo;

    bool sustain;
    bool monoMode;
    
    // Extra buffering for when GetSamples wants a buffer not a multiple of N
    float extra_buf[N];
    int extra_buf_size;

    int currentProgram;
    
    /**
     * The last time the state was save, to be able to bypass a VST host bug.
     */
    long lastStateSave;
    
    /**
     * Plugin fx (the filter)
     */
    PluginFx fx;

    /**
     * This flag is used in the audio thread to know if the voice has changed
     * and needs to be updated.
     */
    bool refreshVoice;
    bool normalizeDxVelocity;
    bool sendSysexChange;
    
    void processMidiMessage(const MidiMessage *msg);
    void keydown(uint8_t chan, uint8_t pitch, uint8_t velo);
    void keyup(uint8_t, uint8_t pitch, uint8_t velo);
    
    /**
     * this is called from the Audio thread to tell
     * to update the UI / hostdata 
     */
    void handleAsyncUpdate();
    void initCtrl();

	MidiMessage* nextMidi,*midiMsg;
	bool hasMidiMessage;
    int midiEventPos;
	bool getNextEvent(MidiBuffer::Iterator* iter,const int samplePos);
    
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message);
    uint32_t engineType;
    
    FmCore engineMsfa;
    EngineMkI engineMkI;
    EngineOpl engineOpl;
    
    char clipboard[161];
    char clipboardContent;
    
    void resolvAppDir();
    
    void unpackOpSwitch(char packOpValue);
    void packOpSwitch();
    
public :
    // in MIDI units (0x4000 is neutral)
    Controllers controllers;
    StringArray programNames;
    Cartridge currentCart;
    uint8_t data[161];

    SysexComm sysexComm;
    VoiceStatus voiceStatus;
    File activeFileCartridge;
    
    bool forceRefreshUI;
    float vuSignal;
    bool showKeyboard;
    int getEngineType();
    void setEngineType(int rs);
    
    HashMap<int, Ctrl*> mappedMidiCC;
    
    Array<Ctrl*> ctrl;

    OperatorCtrl opCtrl[6];
    ScopedPointer<CtrlDX> pitchEgRate[4];
    ScopedPointer<CtrlDX> pitchEgLevel[4];
    ScopedPointer<CtrlDX> pitchModSens;
    ScopedPointer<CtrlDX> algo;
    ScopedPointer<CtrlDX> oscSync;
    ScopedPointer<CtrlDX> feedback;
    ScopedPointer<CtrlDX> lfoRate;
    ScopedPointer<CtrlDX> lfoDelay;
    ScopedPointer<CtrlDX> lfoAmpDepth;
    ScopedPointer<CtrlDX> lfoPitchDepth;
    ScopedPointer<CtrlDX> lfoWaveform;
    ScopedPointer<CtrlDX> lfoSync;
    ScopedPointer<CtrlDX> transpose;

    ScopedPointer<CtrlFloat> fxCutoff;
    ScopedPointer<CtrlFloat> fxReso;
    ScopedPointer<CtrlFloat> output;
    ScopedPointer<Ctrl> tune;

    void loadCartridge(Cartridge &cart);
    void setDxValue(int offset, int v);

    //==============================================================================
    DexedAudioProcessor();
    ~DexedAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);
    void panic();
    bool isMonoMode() {
        return monoMode;
    }
    void setMonoMode(bool mode);
    
    void copyToClipboard(int srcOp);
    void pasteOpFromClipboard(int destOp);
    void pasteEnvFromClipboard(int destOp);
    void sendCurrentSysexProgram();
    void sendCurrentSysexCartridge();
    void sendSysexCartridge(File cart);
    bool hasClipboardContent();
    
    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;
    void updateUI();
    bool peekVoiceStatus();
    int updateProgramFromSysex(const uint8 *rawdata);
    void setupStartupCart();
    
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
    void setCurrentProgram(int index);
    const String getProgramName (int index);
    void changeProgramName(int index, const String& newName);
    void resetToInitVoice();
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    
    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState keyboardState;
    void unbindUI();

    void loadPreference();
    void savePreference();
    
    static File dexedAppDir;
    static File dexedCartDir;
    
    Value lastCCUsed;

    std::shared_ptr<TuningState> synthTuningState;
    // Prompt for a file
    void applySCLTuning();
    void applyKBMMapping();

    // Load a file
    void applySCLTuning(File sclf);
    void applyKBMMapping(File kbmf);

    // Load from text
    void applySCLTuning(std::string scld);
    void applyKBMMapping(std::string kbmd);
    
    void retuneToStandard();
    void resetTuning(std::shared_ptr<TuningState> t);
    int tuningTranspositionShift();
    
    std::string currentSCLData = "";
    std::string currentKBMData = "";
    
    float dpiScaleFactor = -1;
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DexedAudioProcessor)

};

#endif  // PLUGINPROCESSOR_H_INCLUDED
