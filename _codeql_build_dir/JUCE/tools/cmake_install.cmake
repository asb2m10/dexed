# Install script for directory: /home/runner/work/dexed/dexed/libs/JUCE

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/runner/work/dexed/dexed/_codeql_build_dir/JUCE/tools/modules/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/runner/work/dexed/dexed/_codeql_build_dir/JUCE/tools/extras/Build/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-7.0.11" TYPE FILE FILES
    "/home/runner/work/dexed/dexed/_codeql_build_dir/JUCE/tools/JUCEConfigVersion.cmake"
    "/home/runner/work/dexed/dexed/_codeql_build_dir/JUCE/tools/JUCEConfig.cmake"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/JUCECheckAtomic.cmake"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/JUCEHelperTargets.cmake"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/JUCEModuleSupport.cmake"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/JUCEUtils.cmake"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/JuceLV2Defines.h.in"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/LaunchScreen.storyboard"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/PIPAudioProcessor.cpp.in"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/PIPAudioProcessorWithARA.cpp.in"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/PIPComponent.cpp.in"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/PIPConsole.cpp.in"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/RecentFilesMenuTemplate.nib"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/UnityPluginGUIScript.cs.in"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/checkBundleSigning.cmake"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/copyDir.cmake"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/juce_runtime_arch_detection.cpp"
    "/home/runner/work/dexed/dexed/libs/JUCE/extras/Build/CMake/juce_LinuxSubprocessHelper.cpp"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/runner/work/dexed/dexed/_codeql_build_dir/JUCE/tools/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/runner/work/dexed/dexed/_codeql_build_dir/JUCE/tools/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
