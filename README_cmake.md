Initial cmake support
=====================

This initial cmake support is based on Eyal Amir's JUCECmakeRepoPrototype [1]. It is not considered complete
and has only been tested on Debian and Ubuntu. No linker or compile options have been set for  other operating systems.

But, since this initial cmake support doesn't conflict with the projucer based build system and
not a single file needs to be changed for it to work it might serve as a first building step for a proper transition to cmake.


# How to build with cmake

After you clone your first step is within the main directory:

```
git submodule update --init --recursive
```

Then proceed in the usual cmake way:

```
# Debug build of all targets (vst3 and Standalone)
#
cmake -Bbuild-dexed  
cmake --build build-dexed -j4
```
```
# Release build of just a vst3
#
cmake -Bbuild-dexed -DCMAKE_BUILD_TYPE=Release 
cmake --build build-dexed --target Dexed_VST3 -j4
```
(Change the number in `-j4` to match your cpu's parallel processing count)

You'll find the build results in:

```
build-dexed/Source/Dexed_artefacts/
```

JUCE will be downloaded during the `submodule update` step.
So there's no need to invoke the ./scripts/get-juce.sh script.

If you want to try building with a different (local) JUCE version, you can do so with
```
cmake -Bbuild-blah -DDEXED_JUCE_PATH=/location/of/JUCE
```
as the first cmake command.

CMake will use whatever is the default compiler on your system.
If you want to switch to a different one you can do:
```
# example using clang
#
cmake -Bbuild -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```



[1] https://github.com/eyalamirmusic/JUCECmakeRepoPrototype

