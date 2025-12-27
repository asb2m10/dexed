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

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "clap-juce-extensions/clap-juce-extensions.h"

#include "msfa/controllers.h"
#include "msfa/dx7note.h"
#include "msfa/lfo.h"
#include "msfa/synth.h"
#include "msfa/fm_core.h"
#include "msfa/tuning.h"
#include "PluginParam.h"
#include "PluginData.h"
#include "PluginFx.h"
#include "midi/SysexComm.h"
#include "engine/EngineMkI.h"
#include "engine/EngineOpl.h"
#include "parameter/DexedApvts.h"
#include "core/Program.h"
#include "util/CommandFifo.h"

struct ProcessorVoice {
    int channel;
    int midi_note;
    int velocity;
    bool keydown;
    bool sustained;
    bool live;
    int32_t keydown_seq;

    int mpePitchBend;
    Dx7Note *dx7_note;
};

enum DexedEngineResolution {
    DEXED_ENGINE_MODERN,
    DEXED_ENGINE_MARKI,
    DEXED_ENGINE_OPL
};

/// Maximum allowed size for SCL and KBM files.
/// (COMMENT: Since none of the 5175 .scl files in the Scala archive
/// at https://www.huygens-fokker.org/scala/downloads.html#scales
/// exceed 6 KB (in 25th Mar 2024), a maximum size of 16 KB appears
/// to be a practical choice.)
const int MAX_SCL_KBM_FILE_SIZE = 16384;

//==============================================================================
/**
*/
class DexedAudioProcessor  : public AudioProcessor, public MidiInputCallback, public clap_juce_extensions::clap_properties,
    juce::ValueTree::Listener
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
     * Plugin fx (the filter)
     */
    PluginFx fx;

    /**
     * This flag is used in the audio thread to know if the voice has changed
     * and needs to be updated.
     */
    juce::Atomic<bool> refreshVoice { false };

    bool normalizeDxVelocity;
    bool sendSysexChange;
    
    void processMidiMessage(const MidiMessage *msg);
    void keydown(uint8_t chan, uint8_t pitch, uint8_t velo);
    void keyup(uint8_t, uint8_t pitch, uint8_t velo);

	MidiMessage* nextMidi,*midiMsg;
	bool hasMidiMessage;
    int midiEventPos;
	bool getNextEvent(MidiBuffer::Iterator* iter,const int samplePos);
    
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;
    uint32_t engineType;
    
    FmCore engineMsfa;
    EngineMkI engineMkI;
    EngineOpl engineOpl;

    void resolvAppDir();
    
    // void unpackOpSwitch(char packOpValue);
    // void packOpSwitch();

    float zoomFactor = 1;

    void mapParameters();
    void setDxValue(int offset, int v);

public :
    DexedApvts parameters;
    Program activeProgram;

    // in MIDI units (0x4000 is neutral)
    Controllers controllers;
    StringArray programNames;
    Cartridge currentCart;

    SysexComm sysexComm;
    VoiceStatus voiceStatus;
    File activeFileCartridge;

    /**
     * This flag indicates the UI to refresh itself
     */
    juce::Atomic<bool> refreshUI { false };

    float vuSignal;
    double vuDecayFactor = 0.999361; // (for 48 kHz sampling rate)
    bool showKeyboard;
    int getEngineType();
    void setEngineType(int rs);
    
    HashMap<int, Ctrl*> mappedMidiCC;

    void loadCartridge(Cartridge &cart);

    //==============================================================================
    DexedAudioProcessor();
    ~DexedAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
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
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    bool peekVoiceStatus();
    int updateProgramFromSysex(const uint8 *rawdata);
    void setupStartupCart();
    
    //==============================================================================
    const String getName() const override;

    const String getInputChannelName (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair (int index) const override;
    bool isOutputChannelStereoPair (int index) const override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool silenceInProducesSilenceOut() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName (int index) override;
    void changeProgramName(int index, const String& newName) override;
    void resetToInitVoice() ;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    MidiKeyboardState keyboardState;

    void loadPreference();
    void savePreference();
    
    static File dexedAppDir;
    static File dexedCartDir;

    Value lastCCUsed;
    int lastActiveVoice = 0;

    void setZoomFactor(float factor);
    float getZoomFactor() {
        return zoomFactor;
    }

    CommandFifo<DexedAudioProcessor> commandFifo;

    void applyProgram(const Program &program);

    // TUNING STUFF ====================================================
    MTSClient *mtsClient;
    std::shared_ptr<TuningState> synthTuningState;

    // holds the previous working tuning state;
    // used to restore tuning state when there was a problem
    // with loading/applying a new .SCL and/or .KBM file
    std::shared_ptr<TuningState> synthTuningStateLast;

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
    // TUNING STUFF ====================================================
private:
    int chooseNote(uint8_t pitch);
    int32_t nextKeydownSeq;;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DexedAudioProcessor)

};

#endif  // PLUGINPROCESSOR_H_INCLUDED
