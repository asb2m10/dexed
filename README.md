Dexed - FM Plugin Synth
=======================

Please see [Dexed User Website](https://asb2m10.github.io/dexed) for user and download information.

Dexed is a multi-platform, multi format plugin synth that is closely modeled on the Yamaha DX7.
Under the hood it uses [music-synthesizer-for-android](https://github.com/google/music-synthesizer-for-android)
for the synth engine and [JUCE](https://www.juce.com) as an application/plugin wrapper.

The goal of this project is to be a tool/companion for the original DX7. Sound engine
with 'float' value parameters, different waveform à la TX81z would be great but anything that
goes beyond the DX7 should and will be a fork of this project. This is to keep the compatibility with
the original machine.

Dexed is licensed on the GPL v3. The msfa component (acronym for music synthesizer for android, see msfa
in the source folder) stays on the Apache 2.0 license to be able to collaborate between projects.

Pre-release builds are available with the [Nightly Releases](https://github.com/asb2m10/dexed/releases/tag/Nightly)

Donation
--------
As a maintainer of this 10-year-old project, donations are welcomed. This also applies for the Apple users 
to cover for the notarization of this software in the coming years. Thank you!

https://www.paypal.com/paypalme/asb2m10

Dexed Forks
-----------
 * [MiniDexed](https://github.com/probonopd/MiniDexed) Run a DX7 bare metal from a Raspberry Pi
 * [SIMD-optimized](https://github.com/risicle/dexed/tree/ris-highway) CPU optimized version with [highway](https://github.com/google/highway/tree/master)

Changelog
---------
#### Version 1.0.1
* Let DAW scale the UI and then let the user zoom it further if needed
* Fixed scaling issue with CLAP format
* Plugin parameters dialogs are now part of the main plugin window (no more separate windows)

#### Version 0.9.9
* Partial portamento implementation. Thanks @jpcima
* More accurate LFO implementation. Thanks @mtarenskeen
* Fix UI scaling resize bug
* Avoid clicks on voice allocation issue. Thanks @akwizgran
* Respect MIDI channel on control change mappings. Thanks @voidshine
* Mouse wheel movement is now more precise with shift down
* Dexed now replies to sysex "Patch Request" messages
* Active program is now easier to see

#### Version 0.9.8
* Accessibility implementation (including [keyboard shortcuts](https://github.com/asb2m10/dexed/blob/master/Documentation/Keybindings.md))
* UI component refresh
* Mono/Poly parameter is now a plugin parameter
* Copy/paste operator values are now text based (and system wide)
* Fix Apple Logic startup issue

#### Version 0.9.7
* [MTS-ESP](https://oddsound.com/index.php) microtuning support
* [CLAP](https://github.com/free-audio/clap) plugin support (sadly scaling is not available for now, but we are working on this)
* Scalable UI upgrade (better resolution), optimized UI redraw
* More accurate VU meter. Thanks @FulopNandor
* Releases are now notarized for macOS
* Fix for VST3 automation (again)
* For developers: cmake is now the built system

#### Version 0.9.6
* Apple Silicon M1 builds
* Fix VST3 automation issues
* Fix to apply UI scaling only when applicable
* Fix hang notes on program changes

#### Version 0.9.5
* Added support for SCL/KBM micro tuning
* Added initial support for MPE performance
* Upgraded build system to use JUCE 6.0 and build from locally acquired JUCE
* A Collection of small UI changes, including higher contrast GUI assets, better sub-window management, 
  and mouse wheel support
* Added VST3 support

#### Version 0.9.4HF1
* Fixed sysex messages with unwanted pitch bend

#### Version 0.9.4
* Standalone application version of Dexed
* Midi learn support for midi CC messages
* More accurate detune for operators
* More accurate EG envelopes. thanks @jeremybernstein
* Pitchbend now listen to any channels
* Fixed 'clicking' in mono mode
* Fixed implementation for Midi CC 0x78 and 0x7B (All Sound Off / All Notes Off)
* Windows/macOS installers

#### Version 0.9.3
* Preliminary live operator level update
* Support for incoming parameter change messages
* More permissive on cartridge sysex type; the validation is now done on size (4104/4096 bytes)
* Midi 'All Note Off; 0x7B' support
* Breakpoint values are now shown as notes. thanks @tico-tico
* Fixed op feedback calibrated for Mark I engine
* Fixed for breakpoint levels. thanks @Sentinel77
* Fixed LFO delay issue issues upon large values
* Fixed for receiving sysex cartridge. thanks @Sentinel77
* Fixed operator switch changes are now sent via sysex

#### Version 0.9.2
* Mark I engine is now the default engine
* Added operator mute switch
* Added Tune (MASTER TUNE ADJ) knob
* Correct feedback implementation for Algo 4 and 6 (engine Mk I only)
* Single click program select
* Fixed sysex issue with wrong machine ID
* Fixed for parameter hosts values. thanks @Sentinel77

#### Version 0.9.1
* Mark I engine now uses 10-bit sine/exp tables. Still a work in progress but we are getting there
* More accurate FM feedback on the Mark I and OPL Series engine
* Wheel, Foot, Breath, Aftertouch modulation support
* Fixed the display of some more 'complex' algorithms
* Drop of .syx files in the cartridge manager
* Dexed data directory can now optionally reside in the same location where the binary is installed
* 'Send current program to DX7' in Cartridge Manager works as designed
* Support for sysex streams (multiple sysex messages in one .syx file)
* falkTX upstream fixes for Linux

#### Version 0.9.0
* Apple AU support
* Cartridge Manager
* Store action also can update the currently loaded cartridge
* Basic theming
* Fixed the UI corruption when more than one Dexed instance was loaded
* Fixed wrong display value issues (coarse and fine)

Credits & thanks
----------------
* DX Synth engine : Raph Levien and the [msfa](https://github.com/google/music-synthesizer-for-android) team
* [Surge Synth Team](https://surge-synth-team.org/) for substantial contributions like microtuning and MPE support.
* Graphical design : AZur Studio
* [Sentinel77](https://github.com/Sentinel77) for numerous engine fixes
* LP Filter : Filatov Vadim (2DaT); taken from the excellent [Obxd](https://obxd.wordpress.com) project
* PPPlay : Great [OPL3](https://github.com/stohrendorf/ppplay) implementation, with documented code :D
* DX7 program compilation : Jean-Marc Desprez (author of [SynprezFM](http://www.synprez.com/SynprezFM))
* DX7 programs : Dave Benson, Frank Carvalho, Tim Conrardy, Jack Deckard, Chris Dodunski, Tim Garrett, Hitaye, Stephan Ibsen, Christian Jezreel, Narfman, Godric Wilkie
* falkTX [distrho](http://distrho.sourceforge.net/)

TODO - Dexed
------------
* Various code cleanup
* Yamaha 4 operators (DX21/DX27/DX100) sysex import

TODO - msfa
-----------
* Better Amplitude Modulation
* Accurate live operator level envelope updates

# How to build

Clone Dexed from github

```
~ $ git clone https://github.com/asb2m10/dexed.git
```

Dexed has several submodules it now depends on, including VST3/CLAP and a library to support non standard tuning. After you clone your first step is

```
~ $ cd dexed
~/dexed $ git submodule update --init --recursive
```

Then you crate the cmake build files that are will be created in the build directory. On that build you can trigger your favorite IDE or simply use `--build` cmake option.

```
~/dexed $ mkdir build
~/dexed $ cd build
~/dexed/build $ cmake .. -DJUCE_COPY_PLUGIN_AFTER_BUILD=TRUE
~/dexed/build $ cmake --build .
```

If you get missing header compilation errors, be sure to check the [known Linux dependencies](https://github.com/asb2m10/dexed/wiki/Linux-build-dependencies) based on your distribution for Linux.

Binaries will be found in `~/dexed/build/Source/Dexed_artefacts/*`
