set(BaseTargetName Dexed)

juce_add_plugin("${BaseTargetName}"
        # VERSION ...                               # Set this if the plugin version is different to the project version
        ICON_BIG "../Resources/ui/dexedIcon.png"
        ICON_SMALL "../Resources/ui/dexedIcon.png"
        COMPANY_NAME "Digital Suburban"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD FALSE
        PLUGIN_MANUFACTURER_CODE DGSB
        PLUGIN_CODE Dexd
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "Dexed"
        DESCRIPTION "Dexed FM Synth"
)

juce_generate_juce_header(${PROJECT_NAME})

target_sources(${BaseTargetName} PRIVATE
	AlgoDisplay.cpp
	CartManager.cpp
	DXComponents.cpp
	DXLookNFeel.cpp
	EngineMkI.cpp
	EngineOpl.cpp
	GlobalEditor.cpp
	OperatorEditor.cpp
	ParamDialog.cpp
	PluginData.cpp
	PluginEditor.cpp
	PluginFx.cpp
	PluginParam.cpp
	PluginProcessor.cpp
	ProgramListBox.cpp
	SysexComm.cpp
	TuningShow.cpp
        msfa/dx7note.cc
        msfa/env.cc
        msfa/exp2.cc
        msfa/fm_core.cc
        msfa/fm_op_kernel.cc
        msfa/freqlut.cc
        msfa/lfo.cc
        msfa/pitchenv.cc
        msfa/sin.cc
        msfa/tuning.cc
)

target_compile_definitions(${BaseTargetName} PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_JACK=1
    JUCE_MODAL_LOOPS_PERMITTED=1 # needed for FileBrowser in CartManager
)

target_link_libraries(${BaseTargetName} PRIVATE
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    surgesynthteam_tuningui
    tuning-library
    DexedResources
)


