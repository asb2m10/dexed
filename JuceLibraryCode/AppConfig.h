/*

    IMPORTANT! This file is auto-generated each time you save your
    project - if you alter its contents, your changes may be overwritten!

    There's a section below where you can add your own custom code safely, and the
    Projucer will preserve the contents of that block, but the best way to change
    any of these definitions is by using the Projucer's project settings.

    Any commented-out settings will assume their default values.

*/

#pragma once

//==============================================================================
// [BEGIN_USER_CODE_SECTION]

//#define JUCE_CORETEXT_AVAILABLE 0

#define JUCE_ALSA_MIDI_NAME "Dexed"

// [END_USER_CODE_SECTION]

/*
  ==============================================================================

   In accordance with the terms of the JUCE 5 End-Use License Agreement, the
   JUCE Code in SECTION A cannot be removed, changed or otherwise rendered
   ineffective unless you have a JUCE Indie or Pro license, or are using JUCE
   under the GPL v3 license.

   End User License Agreement: www.juce.com/juce-5-licence

  ==============================================================================
*/

// BEGIN SECTION A

#ifndef JUCE_DISPLAY_SPLASH_SCREEN
 #define JUCE_DISPLAY_SPLASH_SCREEN 0
#endif

#ifndef JUCE_REPORT_APP_USAGE
 #define JUCE_REPORT_APP_USAGE 0
#endif

// END SECTION A

#define JUCE_USE_DARK_SPLASH_SCREEN 1

//==============================================================================
#define JUCE_MODULE_AVAILABLE_juce_audio_basics             1
#define JUCE_MODULE_AVAILABLE_juce_audio_devices            1
#define JUCE_MODULE_AVAILABLE_juce_audio_formats            1
#define JUCE_MODULE_AVAILABLE_juce_audio_plugin_client      1
#define JUCE_MODULE_AVAILABLE_juce_audio_processors         1
#define JUCE_MODULE_AVAILABLE_juce_audio_utils              1
#define JUCE_MODULE_AVAILABLE_juce_core                     1
#define JUCE_MODULE_AVAILABLE_juce_data_structures          1
#define JUCE_MODULE_AVAILABLE_juce_events                   1
#define JUCE_MODULE_AVAILABLE_juce_graphics                 1
#define JUCE_MODULE_AVAILABLE_juce_gui_basics               1
#define JUCE_MODULE_AVAILABLE_juce_gui_extra                1

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1

//==============================================================================
// juce_audio_devices flags:

#ifndef    JUCE_ASIO
 #define   JUCE_ASIO 0
#endif

#ifndef    JUCE_WASAPI
 #define   JUCE_WASAPI 1
#endif

#ifndef    JUCE_WASAPI_EXCLUSIVE
 #define   JUCE_WASAPI_EXCLUSIVE 0
#endif

#ifndef    JUCE_DIRECTSOUND
 #define   JUCE_DIRECTSOUND 0
#endif

#ifndef    JUCE_ALSA
 #define   JUCE_ALSA 1
#endif

#ifndef    JUCE_JACK
 #define   JUCE_JACK 0
#endif

#ifndef    JUCE_USE_ANDROID_OPENSLES
 #define   JUCE_USE_ANDROID_OPENSLES 0
#endif

#ifndef    JUCE_USE_WINRT_MIDI
 //#define JUCE_USE_WINRT_MIDI 0
#endif

#ifndef    JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS
 //#define JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS 0
#endif

//==============================================================================
// juce_audio_formats flags:

#ifndef    JUCE_USE_FLAC
 #define   JUCE_USE_FLAC 0
#endif

#ifndef    JUCE_USE_OGGVORBIS
 #define   JUCE_USE_OGGVORBIS 0
#endif

#ifndef    JUCE_USE_MP3AUDIOFORMAT
 #define   JUCE_USE_MP3AUDIOFORMAT 0
#endif

#ifndef    JUCE_USE_LAME_AUDIO_FORMAT
 #define   JUCE_USE_LAME_AUDIO_FORMAT 0
#endif

#ifndef    JUCE_USE_WINDOWS_MEDIA_FORMAT
 #define   JUCE_USE_WINDOWS_MEDIA_FORMAT 0
#endif

//==============================================================================
// juce_audio_plugin_client flags:

#ifndef    JUCE_FORCE_USE_LEGACY_PARAM_IDS
 //#define JUCE_FORCE_USE_LEGACY_PARAM_IDS 0
#endif

#ifndef    JUCE_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE
 //#define JUCE_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE 0
#endif

#ifndef    JUCE_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS
 //#define JUCE_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS 1
#endif

#ifndef    JUCE_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE
 //#define JUCE_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE 0
#endif

//==============================================================================
// juce_audio_processors flags:

#ifndef    JUCE_PLUGINHOST_VST
 #define   JUCE_PLUGINHOST_VST 1
#endif

#ifndef    JUCE_PLUGINHOST_VST3
 #define   JUCE_PLUGINHOST_VST3 0
#endif

#ifndef    JUCE_PLUGINHOST_AU
 #define   JUCE_PLUGINHOST_AU 1
#endif

//==============================================================================
// juce_audio_utils flags:

#ifndef    JUCE_USE_CDREADER
 #define   JUCE_USE_CDREADER 0
#endif

#ifndef    JUCE_USE_CDBURNER
 #define   JUCE_USE_CDBURNER 0
#endif

//==============================================================================
// juce_core flags:

#ifndef    JUCE_FORCE_DEBUG
 //#define JUCE_FORCE_DEBUG 0
#endif

#ifndef    JUCE_LOG_ASSERTIONS
 //#define JUCE_LOG_ASSERTIONS 0
#endif

#ifndef    JUCE_CHECK_MEMORY_LEAKS
 //#define JUCE_CHECK_MEMORY_LEAKS 1
#endif

#ifndef    JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
 #define   JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES 1
#endif

#ifndef    JUCE_INCLUDE_ZLIB_CODE
 //#define JUCE_INCLUDE_ZLIB_CODE 1
#endif

#ifndef    JUCE_USE_CURL
 //#define JUCE_USE_CURL 0
#endif

#ifndef    JUCE_CATCH_UNHANDLED_EXCEPTIONS
 //#define JUCE_CATCH_UNHANDLED_EXCEPTIONS 1
#endif

#ifndef    JUCE_ALLOW_STATIC_NULL_VARIABLES
 //#define JUCE_ALLOW_STATIC_NULL_VARIABLES 1
#endif

//==============================================================================
// juce_events flags:

#ifndef    JUCE_EXECUTE_APP_SUSPEND_ON_IOS_BACKGROUND_TASK
 //#define JUCE_EXECUTE_APP_SUSPEND_ON_IOS_BACKGROUND_TASK 0
#endif

//==============================================================================
// juce_graphics flags:

#ifndef    JUCE_USE_COREIMAGE_LOADER
 //#define JUCE_USE_COREIMAGE_LOADER 1
#endif

#ifndef    JUCE_USE_DIRECTWRITE
 //#define JUCE_USE_DIRECTWRITE 1
#endif

//==============================================================================
// juce_gui_basics flags:

#ifndef    JUCE_ENABLE_REPAINT_DEBUGGING
 //#define JUCE_ENABLE_REPAINT_DEBUGGING 0
#endif

#ifndef    JUCE_USE_XRANDR
 //#define JUCE_USE_XRANDR 1
#endif

#ifndef    JUCE_USE_XINERAMA
 //#define JUCE_USE_XINERAMA 1
#endif

#ifndef    JUCE_USE_XSHM
 //#define JUCE_USE_XSHM 1
#endif

#ifndef    JUCE_USE_XRENDER
 //#define JUCE_USE_XRENDER 0
#endif

#ifndef    JUCE_USE_XCURSOR
 //#define JUCE_USE_XCURSOR 1
#endif

//==============================================================================
// juce_gui_extra flags:

#ifndef    JUCE_WEB_BROWSER
 #define   JUCE_WEB_BROWSER 0
#endif

#ifndef    JUCE_ENABLE_LIVE_CONSTANT_EDITOR
 //#define JUCE_ENABLE_LIVE_CONSTANT_EDITOR 0
#endif
//==============================================================================
#ifndef    JUCE_STANDALONE_APPLICATION
 #if defined(JucePlugin_Name) && defined(JucePlugin_Build_Standalone)
  #define  JUCE_STANDALONE_APPLICATION JucePlugin_Build_Standalone
 #else
  #define  JUCE_STANDALONE_APPLICATION 0
 #endif
#endif

//==============================================================================
// Audio plugin settings..

#ifndef  JucePlugin_Build_VST
 #define JucePlugin_Build_VST              1
#endif
#ifndef  JucePlugin_Build_VST3
 #define JucePlugin_Build_VST3             0
#endif
#ifndef  JucePlugin_Build_AU
 #define JucePlugin_Build_AU               1
#endif
#ifndef  JucePlugin_Build_AUv3
 #define JucePlugin_Build_AUv3             0
#endif
#ifndef  JucePlugin_Build_RTAS
 #define JucePlugin_Build_RTAS             0
#endif
#ifndef  JucePlugin_Build_AAX
 #define JucePlugin_Build_AAX              0
#endif
#ifndef  JucePlugin_Build_Standalone
 #define JucePlugin_Build_Standalone       1
#endif
#ifndef  JucePlugin_Enable_IAA
 #define JucePlugin_Enable_IAA             0
#endif
#ifndef  JucePlugin_Name
 #define JucePlugin_Name                   "Dexed"
#endif
#ifndef  JucePlugin_Desc
 #define JucePlugin_Desc                   "Dexed FM Synth"
#endif
#ifndef  JucePlugin_Manufacturer
 #define JucePlugin_Manufacturer           "Digital Suburban"
#endif
#ifndef  JucePlugin_ManufacturerWebsite
 #define JucePlugin_ManufacturerWebsite    ""
#endif
#ifndef  JucePlugin_ManufacturerEmail
 #define JucePlugin_ManufacturerEmail      ""
#endif
#ifndef  JucePlugin_ManufacturerCode
 #define JucePlugin_ManufacturerCode       0x44475342 // 'DGSB'
#endif
#ifndef  JucePlugin_PluginCode
 #define JucePlugin_PluginCode             0x44657864 // 'Dexd'
#endif
#ifndef  JucePlugin_IsSynth
 #define JucePlugin_IsSynth                1
#endif
#ifndef  JucePlugin_WantsMidiInput
 #define JucePlugin_WantsMidiInput         1
#endif
#ifndef  JucePlugin_ProducesMidiOutput
 #define JucePlugin_ProducesMidiOutput     1
#endif
#ifndef  JucePlugin_IsMidiEffect
 #define JucePlugin_IsMidiEffect           0
#endif
#ifndef  JucePlugin_EditorRequiresKeyboardFocus
 #define JucePlugin_EditorRequiresKeyboardFocus  0
#endif
#ifndef  JucePlugin_Version
 #define JucePlugin_Version                1.0.0
#endif
#ifndef  JucePlugin_VersionCode
 #define JucePlugin_VersionCode            0x10000
#endif
#ifndef  JucePlugin_VersionString
 #define JucePlugin_VersionString          "1.0.0"
#endif
#ifndef  JucePlugin_VSTUniqueID
 #define JucePlugin_VSTUniqueID            JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_VSTCategory
 #define JucePlugin_VSTCategory            kPlugCategSynth
#endif
#ifndef  JucePlugin_AUMainType
 #define JucePlugin_AUMainType             kAudioUnitType_MusicDevice
#endif
#ifndef  JucePlugin_AUSubType
 #define JucePlugin_AUSubType              JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_AUExportPrefix
 #define JucePlugin_AUExportPrefix         DexedAU
#endif
#ifndef  JucePlugin_AUExportPrefixQuoted
 #define JucePlugin_AUExportPrefixQuoted   "DexedAU"
#endif
#ifndef  JucePlugin_AUManufacturerCode
 #define JucePlugin_AUManufacturerCode     JucePlugin_ManufacturerCode
#endif
#ifndef  JucePlugin_CFBundleIdentifier
 #define JucePlugin_CFBundleIdentifier     com.digitalsuburban.Dexed
#endif
#ifndef  JucePlugin_RTASCategory
 #define JucePlugin_RTASCategory           ePlugInCategory_SWGenerators
#endif
#ifndef  JucePlugin_RTASManufacturerCode
 #define JucePlugin_RTASManufacturerCode   JucePlugin_ManufacturerCode
#endif
#ifndef  JucePlugin_RTASProductId
 #define JucePlugin_RTASProductId          JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_RTASDisableBypass
 #define JucePlugin_RTASDisableBypass      0
#endif
#ifndef  JucePlugin_RTASDisableMultiMono
 #define JucePlugin_RTASDisableMultiMono   0
#endif
#ifndef  JucePlugin_AAXIdentifier
 #define JucePlugin_AAXIdentifier          com.yourcompany.Dexed
#endif
#ifndef  JucePlugin_AAXManufacturerCode
 #define JucePlugin_AAXManufacturerCode    JucePlugin_ManufacturerCode
#endif
#ifndef  JucePlugin_AAXProductId
 #define JucePlugin_AAXProductId           JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_AAXCategory
 #define JucePlugin_AAXCategory            AAX_ePlugInCategory_Dynamics
#endif
#ifndef  JucePlugin_AAXDisableBypass
 #define JucePlugin_AAXDisableBypass       0
#endif
#ifndef  JucePlugin_AAXDisableMultiMono
 #define JucePlugin_AAXDisableMultiMono    0
#endif
#ifndef  JucePlugin_IAAType
 #define JucePlugin_IAAType                0x61757269 // 'auri'
#endif
#ifndef  JucePlugin_IAASubType
 #define JucePlugin_IAASubType             JucePlugin_PluginCode
#endif
#ifndef  JucePlugin_IAAName
 #define JucePlugin_IAAName                "Digital Suburban: Dexed"
#endif
#ifndef  JucePlugin_MaxNumInputChannels
 #define JucePlugin_MaxNumInputChannels    0
#endif
#ifndef  JucePlugin_MaxNumOutputChannels
 #define JucePlugin_MaxNumOutputChannels   2
#endif
#ifndef  JucePlugin_PreferredChannelConfigurations
 #define JucePlugin_PreferredChannelConfigurations  {0, 2}
#endif
