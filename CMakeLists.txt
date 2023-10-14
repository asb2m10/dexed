cmake_minimum_required(VERSION 3.16)
project(Dexed VERSION 0.9.6)

find_package(hwy REQUIRED)

#Compile commands, useful for some IDEs like VS-Code
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

#Minimum MacOS target, set globally

if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
    set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0 CACHE STRING "Minimum OS X deployment version" FORCE)

    #code signing to run on an iOS device:
#    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer" CACHE STRING "" FORCE)
#    set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "XXXXXXXXXX" CACHE STRING "" FORCE)
else()
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version" FORCE)
endif()

option(UniversalBinary "Build universal binary for mac" OFF)

if (UniversalBinary)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif()

#static linking in Windows
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# include JUCE *AFTER* the MSVC runtime and so on is set up
set(DEXED_JUCE_PATH "${CMAKE_SOURCE_DIR}/libs/JUCE" CACHE STRING "Path to JUCE library source tree")
add_subdirectory(${DEXED_JUCE_PATH} ${CMAKE_BINARY_DIR}/JUCE EXCLUDE_FROM_ALL)
add_subdirectory(libs/clap-juce-extensions EXCLUDE_FROM_ALL)



#Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

#set any of these to "ON" if you want to build one of the juce examples
#or extras (Projucer/AudioPluginHost, etc):
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

add_subdirectory(libs)
add_subdirectory(Resources)
add_subdirectory(Source)

# uncomment to enable warnings are errors,
# suppress warnings (or remove when fixed)
# or add -Wall, -Wextra or specific warnings as you see fit 
if (APPLE)
target_compile_options(${PROJECT_NAME} PUBLIC
        #-Werror
        #-Wno-deprecated-declarations
        #-Wno-unused-value
        )
elseif(UNIX AND NOT APPLE)
target_compile_options(${PROJECT_NAME} PUBLIC
        #-Werror
        -Wno-deprecated-declarations
        -Wno-unused-value
        )
else()
target_compile_options(${PROJECT_NAME} PUBLIC
        #/WX # Warnings are errors
        #/wd4996 # deprecated-declarations
        # (I don't know all the windows warning numbers
        #  replace cxxx with /wdxxx to supress them)
        )
endif()
