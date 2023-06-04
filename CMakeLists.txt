cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME "Dexed")
set(VERSION "0.9.6")
set(FORMATS "VST3" "Standalone") # Standalone Unity VST VST3 AU AUv3 AAX LV2

project(${PROJECT_NAME} VERSION ${VERSION})

set(LIBS_DIR "./libs")
set(SRC_DIR "./Source")
set(RSC_DIR "./Resources/ui")

if(DEFINED ENV{JUCE_PATH})
    set(JUCE_DIR $ENV{JUCE_PATH}) # try %JUCE_PATH% env first
else()
    set(JUCE_DIR "${LIBS_DIR}/JUCE")
endif()

add_subdirectory(${JUCE_DIR} "juce")

# for ASIO support create the following dir structure containing headers from https://www.steinberg.net/asiosdk
# ├── sdk
# │   └── asio
# │       ├── asio.h
# │       ├── asiosys.h
# │       └── iasiodrv.h
if(WIN32 AND CMAKE_PLUGIN_IS_STANDALONE)
    message(STATUS "Building for Windows...")
    set(ASIO_DIR "sdk/asio") 
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${ASIO_DIR})
        set(BUILD_ASIO 1)
    endif()
endif()

# for legacy VST support create the dir structure (due to licensing issues you need to find it by yourself)
# ├── sdk
# │   └── vst2
# |       └──pluginterfaces
# |          └──vst2.x
# |             ├── aeffect.h
# |             ├── aeffectx.h
# |             └── vstfxstore.h
set(VST2_DIR "sdk/vst2")
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${VST2_DIR} AND NOT CMAKE_PLUGIN_IS_STANDALONE)
    set(BUILD_VST2 1)
    juce_set_vst2_sdk_path(${VST2_DIR})
    list(APPEND FORMATS "VST")
endif()

juce_add_plugin(${PROJECT_NAME}
    VERSION ${VERSION}
    PLUGIN_NAME ${PROJECT_NAME}
    DESCRIPTION "${PROJECT_NAME} FM Synth (Mod)"
    COMPANY_NAME "Digital Suburban"
    COMPANY_COPYRIGHT "Digital Suburban"
    BUNDLE_ID "com.digital-suburban.dexed"
    AAX_IDENTIFIER "com.digital-suburban.dexed"
    LV2URI "https://dexed.digital-suburban.com"
    AU_EXPORT_PREFIX "DexedAU"
    PLUGIN_CODE "Dexd"
    PLUGIN_MANUFACTURER_CODE "DGSB"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    ICON_BIG "${RSC_DIR}/dexedIcon.png"
    ICON_SMALL "${RSC_DIR}/dexedIcon.png"
    FORMATS ${FORMATS}
)

juce_generate_juce_header(${PROJECT_NAME})
juce_add_module(${LIBS_DIR}/surgesynthteam_tuningui) # tuning ui lib
add_subdirectory(${LIBS_DIR}/clap-juce-extensions) # "*.clap" format support
clap_juce_extensions_plugin(TARGET ${PROJECT_NAME}
    CLAP_ID "com.digital-suburban.dexed"
    CLAP_FEATURES instrument FM DX7
)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBS_DIR}/MTS-ESP/Client) # microtuning lib
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBS_DIR}/tuning-library/include) # tuning lib

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_MODAL_LOOPS_PERMITTED=1
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
)

target_sources(${PROJECT_NAME}
  PRIVATE
    "${SRC_DIR}/AlgoDisplay.cpp"
    "${SRC_DIR}/CartManager.cpp"
    "${SRC_DIR}/DXComponents.cpp"
    "${SRC_DIR}/DXLookNFeel.cpp"
    "${SRC_DIR}/EngineMkI.cpp"
    "${SRC_DIR}/EngineOpl.cpp"
    "${SRC_DIR}/GlobalEditor.cpp"
    "${SRC_DIR}/OperatorEditor.cpp"
    "${SRC_DIR}/ParamDialog.cpp"
    "${SRC_DIR}/PluginData.cpp"
    "${SRC_DIR}/PluginEditor.cpp"
    "${SRC_DIR}/PluginFx.cpp"
    "${SRC_DIR}/PluginParam.cpp"
    "${SRC_DIR}/PluginProcessor.cpp"
    "${SRC_DIR}/ProgramListBox.cpp"
    "${SRC_DIR}/SysexComm.cpp"
    "${SRC_DIR}/TuningShow.cpp"
    "${SRC_DIR}/msfa/dx7note.cc"
    "${SRC_DIR}/msfa/env.cc"
    "${SRC_DIR}/msfa/exp2.cc"
    "${SRC_DIR}/msfa/fm_core.cc"
    "${SRC_DIR}/msfa/fm_op_kernel.cc"
    "${SRC_DIR}/msfa/freqlut.cc"
    "${SRC_DIR}/msfa/lfo.cc"
    "${SRC_DIR}/msfa/pitchenv.cc"
    "${SRC_DIR}/msfa/sin.cc"
    "${SRC_DIR}/msfa/tuning.cc"

    "${LIBS_DIR}/MTS-ESP/Client/libMTSClient.cpp"
)

juce_add_binary_data(${PROJECT_NAME}_BinaryData
  SOURCES
    "${RSC_DIR}/../builtin_pgm.zip"
    "${RSC_DIR}/ButtonUnlabeled_50x30.png"
    "${RSC_DIR}/GlobalEditor_1728x288.png"
    "${RSC_DIR}/HelpButton.png"
    "${RSC_DIR}/Knob_68x68.png"
    "${RSC_DIR}/LFO_36_26.png"
    "${RSC_DIR}/Light_28x28.png"
    "${RSC_DIR}/Meter_140x8.png"
    "${RSC_DIR}/NotoSans-Bold.ttf"
    "${RSC_DIR}/NotoSans-Regular.ttf"
    "${RSC_DIR}/OperatorEditor_574x436.png"
    "${RSC_DIR}/Scaling_36_26.png"
    "${RSC_DIR}/Slider_52x52.png"
    "${RSC_DIR}/Switch_64x64.png"
    "${RSC_DIR}/Switch_96x52.png"
    "${RSC_DIR}/SwitchLighted_48x26.png"
    "${RSC_DIR}/dexed-logo.png"
    "${RSC_DIR}/dexedIcon.png"
)

if(BUILD_VST2)
    message(STATUS "Adding VST2...")
    target_compile_definitions(${PROJECT_NAME} PUBLIC JUCE_VST3_CAN_REPLACE_VST2=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC JUCE_VST3_CAN_REPLACE_VST2=0)
endif(BUILD_VST2)

if(BUILD_ASIO)
    message(STATUS "Adding ASIO...")
    target_compile_definitions(${PROJECT_NAME} PRIVATE JUCE_ASIO=1)
    target_include_directories(${PROJECT_NAME} PRIVATE ${ASIO_DIR})
endif(BUILD_ASIO)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(-fPIC)
endif()

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${PROJECT_NAME}_BinaryData
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
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
