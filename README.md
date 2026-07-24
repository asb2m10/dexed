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
Donations have been discontinued (thanks to everyone who has contributed!). I'm planning to release a new Farfisa
Organ synth for iOS — that will be the way to support Dexed going forward.

Dexed Forks
-----------
 * [MiniDexed](https://github.com/probonopd/MiniDexed) Run a DX7 bare metal from a Raspberry Pi
 * [SIMD-optimized](https://github.com/risicle/dexed/tree/ris-highway) CPU optimized version with [highway](https://github.com/google/highway/tree/master)

Changelog
---------
#### Version 1.1.0
* Complete rewrite of event handling to support a modern plugin framework
  - This should solve most of the pending automation issues Dexed had with other DAWs
  - Dexed should be much more responsive now that events are handled entirely by JUCE
  - Note that a Dexed 1.1.x DAW project can read 1.0.x ones, but not the other way around
* Cleanup of most remaining usages of deprecated code
* Source tree reorganized (`Source/` -> `source/`, `Documentation/` -> `doc/`) into `core/`, `engine/`,
  `midi/`, `parameter/`, `ui/` modules; the bundled VST3 SDK submodule was removed in favor of the one
  pulled in via `libs/vst3sdk`/CLAP
* Fixed filter click on automation
* Fixed possible crash on malformed cartridge data
* New developer view that lets advanced users inspect Dexed's internal state — ALT+CLICK on the Dexed logo
  opens the debugger. This is for internal use only and unsupported.

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
* Accessibility implementation (including [keyboard shortcuts](https://github.com/asb2m10/dexed/blob/master/doc/Keybindings.md))
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

Binaries will be found in `~/dexed/build/source/Dexed_artefacts/*`
