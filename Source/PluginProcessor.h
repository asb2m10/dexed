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

/// Maximum allowed size for SCL and KBM files.
/// (COMMENT: Since none of the 5175 .scl files in the Scala archive 
/// at ‘https://www.huygens-fokker.org/scala/downloads.html#scales’ 
/// exceed 6 KB (in 25th Mar 2024), a maximum size of 16 KB appears 
/// to be a practical choice.)
const int MAX_SCL_KBM_FILE_SIZE = 16384;

//--- Some constants used for approximating derivation of ``FLT_CNV_FACT_*`` consts (see them below).

// Maximum allowed value of the outgoing float amplitudes from the plugin.
static const int MAX_FLOAT_AMP_PLUGIN_OUTPUT = 1.0F;

// Minimum allowed value of the outgoing float amplitudes from the plugin.
static const int MIN_FLOAT_AMP_PLUGIN_OUTPUT = -1.0F;

/// Maximum absolute value of the output amplitudes for engine ``MSFA``.
static const int32_t MAX_ABS_AMP_MSFA = MAX_ABS_AMP_FM_CORE_OP * 6; // ``*6`` = because outputs of all the 6 operators may contribute to the final output in Algorithm 32.

/// Maximum absolute value of the output amplitudes for engine ``MKI``.
static const int32_t MAX_ABS_AMP_MKI = MAX_ABS_AMP_MKI_OP * 6;

/// Maximum absolute value of the output amplitudes for engine ``OPL``.
static const int32_t MAX_ABS_AMP_OPL = MAX_ABS_AMP_OPL_OP * 6;


//--- Conversion factors to convert in32t_t sample values to float sample values for the indivudual VU indicators of the operators.
static const float FLT_CNV_FACT_MSFA_OP = 1.0F / MAX_ABS_AMP_FM_CORE_OP;
static const float FLT_CNV_FACT_MKI_OP = 1.0F / MAX_ABS_AMP_MKI_OP;
static const float FLT_CNV_FACT_OPL_OP = 1.0F / MAX_ABS_AMP_OPL_OP;

//--- Conversion factors to convert int32_t sample values to float main output sample values

// An experimental value to reproduce about the similar loudness of main output than before
static const float EXPERIMENTAL_VOLUME_CONSTANT_MSFA = 1.510;
static const float EXPERIMENTAL_VOLUME_CONSTANT_MKI = 0.1500;
static const float EXPERIMENTAL_VOLUME_CONSTANT_OPL = 1.512;

/// Conversion factor to convert int32_t sample values to float sample values 
/// into the allowed range of the plugin output for the ``MSFA`` engine
static const float FLT_CNV_FACT_MSFA_MAIN_OUTPUT = MAX_FLOAT_AMP_PLUGIN_OUTPUT / (float)MAX_ABS_AMP_MSFA * EXPERIMENTAL_VOLUME_CONSTANT_MSFA;

/// Conversion factor to convert int32_t sample values to float sample values 
/// into the allowed range of the plugin output for the ``MARK I`` engine
static const float FLT_CNV_FACT_MKI_MAIN_OUTPUT = MAX_FLOAT_AMP_PLUGIN_OUTPUT / (float)MAX_ABS_AMP_MKI * EXPERIMENTAL_VOLUME_CONSTANT_MKI;

/// Conversion factor to convert int32_t sample values to float sample values 
/// into the allowed range of the plugin output for the ``OPL`` engine
static const float FLT_CNV_FACT_OPL_MAIN_OUTPUT = MAX_FLOAT_AMP_PLUGIN_OUTPUT / (float)MAX_ABS_AMP_OPL * EXPERIMENTAL_VOLUME_CONSTANT_OPL;

//==============================================================================
/**
*/
class DexedAudioProcessor  : public AudioProcessor, public AsyncUpdater, public MidiInputCallback, public clap_juce_extensions::clap_properties
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
    
    // a multiplier used to convert int32_t samples produced by the currently selected engine to float samples
    float flt_cnv_fact_main_output = FLT_CNV_FACT_MKI_MAIN_OUTPUT;

    char clipboard[161];
    char clipboardContent;
    
    void resolvAppDir();
    
    void unpackOpSwitch(char packOpValue);
    void packOpSwitch();

    float dpiScaleFactor = -1;

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
    std::unique_ptr<CtrlDX> pitchEgRate[4];
    std::unique_ptr<CtrlDX> pitchEgLevel[4];
    std::unique_ptr<CtrlDX> pitchModSens;
    std::unique_ptr<CtrlDX> algo;
    std::unique_ptr<CtrlDX> oscSync;
    std::unique_ptr<CtrlDX> feedback;
    std::unique_ptr<CtrlDX> lfoRate;
    std::unique_ptr<CtrlDX> lfoDelay;
    std::unique_ptr<CtrlDX> lfoAmpDepth;
    std::unique_ptr<CtrlDX> lfoPitchDepth;
    std::unique_ptr<CtrlDX> lfoWaveform;
    std::unique_ptr<CtrlDX> lfoSync;
    std::unique_ptr<CtrlDX> transpose;

    std::unique_ptr<CtrlFloat> fxCutoff;
    std::unique_ptr<CtrlFloat> fxReso;
    std::unique_ptr<CtrlFloat> output;
    std::unique_ptr<Ctrl> tune;

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
    String getParameterID (int index) override;

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const;

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
    void setDpiScaleFactor(float factor);
    float getDpiScaleFactor() {
        return dpiScaleFactor;
    }    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DexedAudioProcessor)

};

#endif  // PLUGINPROCESSOR_H_INCLUDED
