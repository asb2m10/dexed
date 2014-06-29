Dexed - FM Plugin Synth
=======================

Dexed is a multi platform, multi format plugin synth that is closely modeled on the Yamaha DX7. 
Under the hood it uses [music-synthesizer-for-android](https://code.google.com/p/music-synthesizer-for-android) 
for the synth engine and [JUCE](http://wwww.juce.com) as a plugin wrapper.

The goal of this project is to be a tool/companion for the original DX7. Sound engine 
with 'float' value parameters, different waveform à la TX81z would be great but anything that 
goes beyond the DX7 should and will be a fork of this project. This is to keep the compatibility with
the original machine.

Dexed is licensed on the GPL v2. The msfa component (acronym for music synthesizer for android, see msfa 
in the source folder) stays on the Apache 2.0 license to able to collaborate between projects.

Features
--------
* Multi platform (OS X, Windows, Linux) and multi format (VST, AU and others that I don't use); by using JUCE
* The sound engine [music-synthesizer-for-android](https://code.google.com/p/music-synthesizer-for-android) is closely modeled on the original DX7 characteristics
* 144 DAW automatable DX7 parameters available from one single panel
* Fully supports DX7 input and output Sysex messages; including controller change. This means that you can use this with a native DX7/TX7 as a patch editor
* Each operator have a realtime VU meter to know which one is active
* Can load/save any DX7/TX7 sysex programs. It is also possible to save a single program into a different sysex file.

Changelog
---------
#### Version 0.6.0 (current sprint)
* Added external midi interface to send / receive sysex message
* Fix Tracktion crash upon startup 
* Middle C (transpose) now works
* Mouse over the interface controls now tells what it does without having to change it

#### Version 0.5.1
* Fix distortion issue with FL (DAW blocksize not multiple of 64)
* OS X 64bit build (the VST package contains both 32bit and 64bit)

#### Version 0.5.0
* Linux support
* Fix large DAW blocksize with midi events 

#### Version 0.4.0
* Modulation wheel support
* Now using the [Obxd](https://obxd.wordpress.com) 4-pole lowpass filter implementation
* Pitchbend range / step
* Output/Volume VU status

Binary downloads
----------------
Dexed is not a finished product but it is stable enough to be used in a DAW environment: 
in normal operation it shouldn't crash and the VST state saving works. If you don't see the new version here but you see it in the change log, it's because this version is in development (current sprint). Only officials (tested) builds are listed here.

* Version 0.5.1 [vst win32](http://le-son666.com/software/dexed/dexed-0.5.1-win32.zip) - [vst win64](http://le-son666.com/software/dexed/dexed-0.5.1-win64.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.5.1-osx.vst.zip)
* Version 0.5.0 [vst win32](http://le-son666.com/software/dexed/dexed-0.5.0a-win32.zip) - [vst win64](http://le-son666.com/software/dexed/dexed-0.5.0a-win64.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.5.0-osx.vst.zip)
* Version 0.4.0 [vst win32](http://le-son666.com/software/dexed/dexed-0.4.0-win32.zip) - [vst win64](http://le-son666.com/software/dexed/dexed-0.4.0-win64.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.4.0-osx.vst.zip)
* Version 0.3.0 [vst win32](http://le-son666.com/software/dexed/dexed-0.3.0-win32.zip) - [vst win64](http://le-son666.com/software/dexed/dexed-0.3.0-win64.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.3.0-osx.vst.zip)

Using as a DX7 editor
---------------------
You can use this plugin to edit your real DX7 patch. Since sysex support on most DAW is missing, you might need to configure this plugin to send/receive sysex data to a specific midi port.

Randomized programs
-------------------
Dexed doesn't check the sysex checksum so you can load any type of files. If the checksum doesn't 
match, it will tell you but load it anyway. This feature act as a kind of a randomization 
for DX programs.

Warning: loading corrupt/random data can crash the plugin and ultimatly your DAW. I'm doing my
best to filter out values that can crash the DX engine; but I keep it to the minimum to get the
digital circuit bending feel.

Saving those corrupt/random sysex data will regenerate a 32 bulk program sysex dump with a 
valid checksum for your DX7 keyboard. I'm in now way responsible if this breaks your DX7 keyboard.

FAQ (possibly)
--------------
* Some programs can generate distortion : This is because the voice summing still needs some tuning. You can simply lower the volume on those programs.
* Some sysex seems to be corrupted : Even if the sysex checksum doesn't match, Dexed will try to load it (this is a kind of randomize feature). Right now Dexed supports only original DX7 sysex, other DX family sysex (like the DX21) is considered as random data.
* Dexed doesn't receive/send parameter data from/to my DX7 : Most DX7 parameter change are done via sysex and very few VST host actually implements sysex. Configure this plugin to send sysex data to a specific midi interface (see PARM panel).

Credits & thanks
----------------
* DX Synth engine : Raph Levien and the [msfa](https://code.google.com/p/music-synthesizer-for-android) team 
* LP Filter : Filatov Vadim (2DaT); taken from the excellent [Obxd](https://obxd.wordpress.com) project
* DX7 program compilation : Jean-Marc Desprez (author of [SynprezFM](http://www.synprez.com/SynprezFM)) 
* DX7 programs : Dave Benson, Frank Carvalho, Tim Conrardy, Jack Deckard, Chris Dodunski, Tim Garrett, Hitaye, Stephan Ibsen, Christian Jezreel, Narfman, Godric Wilkie
* markusthegeek direct implication for this project

TODO - Dexed 
------------
* Implement a better DX look and feel (amp, pitch, algo) *** commin Q4 2014
* Various code cleanup
* 4op (DX21/DX27/DX100) sysex import

TODO - msfa
-----------
* The sample rate should not change the response of the envelopes
* Portamento implementation
* LFO/Mod-wheel amplitude
* Algo 4 & 6 feedback
