# Surge-Synth-Team Dexed Branch

The surge synth team has a branch of dexed to do a few things

- upgrade and make builds regularly availble
- add tuning
- potentially add mpe
- just generally work on dexed
- curious? [You can join our slack via this link](https://raw.githubusercontent.com/surge-synthesizer/surge-synthesizer.github.io/master/_includes/slack_invite_link)

At this time this is a work in progress. Probably best to not pay too much attention...

If you want to build this, you may notice the JUCER files are gone. THat's OK! We build them as needed now using a set of
scripts. Your easiest path is to run 'scripts/build.sh' but this documentation will improve shortly.

Since Steinberg has discontinued the VST2 API we no longer distribute it. On Mac and Windows we strongly recommend you
use the provided VST3. On Linux, the situation is a little trickier, with VST3 support in hosts being uneven. If you
are a licensee to the VST2SDK, though, you can build a VST2 version of DEXED yourself with these commands

```
./scripts/get-juce.sh
export VST2SDK_DIR=~/path/to/vst2/sdk/dir
./scripts/projuce-lin-vst2.sh
./scripts/build-lin.sh
```

and you will get a VST2 and Standalone as opposed to just a Standalone.


Dexed - FM Plugin Synth
=======================


Please see [Dexed User Website](https://asb2m10.github.io/dexed) for user and download information.

Dexed is a multi platform, multi format plugin synth that is closely modeled on the Yamaha DX7. 
Under the hood it uses [music-synthesizer-for-android](https://github.com/google/music-synthesizer-for-android) 
for the synth engine and [JUCE](https://www.juce.com) as an application/plugin wrapper.

The goal of this project is to be a tool/companion for the original DX7. Sound engine 
with 'float' value parameters, different waveform à la TX81z would be great but anything that 
goes beyond the DX7 should and will be a fork of this project. This is to keep the compatibility with
the original machine.

Dexed is licensed on the GPL v3. The msfa component (acronym for music synthesizer for android, see msfa 
in the source folder) stays on the Apache 2.0 license to able to collaborate between projects.

Changelog
---------
#### Version 0.9.5
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
* DX Synth engine : Raph Levien and the [msfa](https://code.google.com/p/music-synthesizer-for-android) team 
* Graphical design : AZur Studio
* [Sentinel77](https://github.com/Sentinel77) for numerous engine fixes
* LP Filter : Filatov Vadim (2DaT); taken from the excellent [Obxd](https://obxd.wordpress.com) project
* PPPlay : Great [OPL3](https://github.com/stohrendorf/ppplay) implementation, with documented code :D
* DX7 program compilation : Jean-Marc Desprez (author of [SynprezFM](http://www.synprez.com/SynprezFM)) 
* DX7 programs : Dave Benson, Frank Carvalho, Tim Conrardy, Jack Deckard, Chris Dodunski, Tim Garrett, Hitaye, Stephan Ibsen, Christian Jezreel, Narfman, Godric Wilkie
* falkTX [distrho](http://distrho.sourceforge.net/)

TODO - Dexed 
------------
* Scalable UI upgrade, better UI redraw
* Various code cleanup
* Yamaha 4 operators (DX21/DX27/DX100) sysex import

TODO - msfa
-----------
* Portamento implementation
* Better Amplitude Modulation
* Accurate live operator level envelope updates

