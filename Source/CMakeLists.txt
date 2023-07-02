set(BaseTargetName Dexed)

set(DEXED_JUCE_FORMATS Standalone)

if(NOT DEXED_SKIP_VST3)
  list(APPEND DEXED_JUCE_FORMATS VST3)
endif()

if (APPLE)
  list(APPEND DEXED_JUCE_FORMATS AU)
endif()

message(STATUS "Building Dexed in formats: ${DEXED_JUCE_FORMATS}")

juce_add_plugin("${BaseTargetName}"
        VERSION "1.0.0"   # FORCING THIS SINCE OLDER VERSION WILL NOT BE ABLE TO SYNC PLUGIN DATA. THIS WILL GET UPDATED ONCE 1.0 is released.
        ICON_BIG "../Resources/ui/dexedIcon.png"
        ICON_SMALL "../Resources/ui/dexedIcon.png"
        COMPANY_NAME "Digital Suburban"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT TRUE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        PLUGIN_MANUFACTURER_CODE DGSB
        PLUGIN_CODE Dexd
        FORMATS ${DEXED_JUCE_FORMATS}
        PRODUCT_NAME "Dexed"
        DESCRIPTION "Dexed FM Synth"
)

clap_juce_extensions_plugin(TARGET ${BaseTargetName}
        CLAP_ID "com.digital-suburban.dexed"
        CLAP_FEATURES instrument FM DX7)

juce_generate_juce_header(${PROJECT_NAME})

include_directories(../libs/MTS-ESP/Client)

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

    # until MTS-ESP has is own cmake project
    ../libs/MTS-ESP/Client/libMTSClient.cpp
)

if(DEXED_NO_ALSA)
    set(DEXED_ALSA FALSE)
    message(STATUS "Building Dexed without ALSA support")
else()
    set(DEXED_ALSA TRUE)
endif()

target_compile_definitions(${BaseTargetName} PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_JACK=1
    JUCE_ALSA=$<IF:$<BOOL:${DEXED_ALSA}>,1,0>
    JUCE_MODAL_LOOPS_PERMITTED=1 # needed for FileBrowser in CartManager
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
    # JUCE_COREGRAPHICS_RENDER_WITH_MULTIPLE_PAINT_CALLS=1     # REVERT BACK SINCE THERE IS AN ISSUE WITH JUCE 7.0.0
)

target_link_libraries(${BaseTargetName} 
PRIVATE
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
    hwy::hwy
    surgesynthteam_tuningui
    tuning-library
    DexedResources
PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
)
