Initial cmake support
=====================

This initial cmake support is based on Eyal Amir's JUCECmakeRepoPrototype [1]. It is not considered complete
and has only been tested on Debian. No linker or compile options have been set for  other operating systems.

But, since this initial cmake support doesn't conflict with the projucer based build system and
not a single file needs to be changed for it to work it might serve as a first building step for a proper transition to cmake.


# How to build with cmake

After you clone your first step is within the main directory:

```
git submodule update --init --recursive
```


Then proceed in the usual cmake way:

```
# Leave project directory
cd ..
# Create build directory and change into it
mkdir build-dexed && cd build-dexed
# invoke cmake
cmake ../dexed/
# build (i.e. make on Linux)
make
```


JUCE will be automatically downloaded via CPM [2] in the cmake step.
So there's no need invoke the ./scripts/get-juce.sh script.

You'll find the build results in:

```
build-dexed/Source/Dexed_artefacts/
```



[1] https://github.com/eyalamirmusic/JUCECmakeRepoPrototype
[2] https://github.com/cpm-cmake/CPM.cmake

