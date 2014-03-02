Dexed - FM Synth Plugin
=======================

Dexed is a multi platform, multi format plugin synth that is closely modeled on the Yamaha DX7. 
Under the hood it uses [music-synthesizer-for-android](https://code.google.com/p/music-synthesizer-for-android) 
for the synth engine and [JUCE](http://wwww.juce.com) as a plugin wrapper.

The goal of this project is to be a great tool/companion for the original DX7. Sound engine 
with 'float' value parameters, different waveform à la TX81z would be great but anything that 
goes beyond the DX7 should and will be a fork of this project. This is to keep the compatiblity with
the original DX7.

Dexed is licensed on the GPL v2. The msfa component (acronym for music synthesizer for android, see msfa 
in the source folder) stays on the Apache 2.0 license to able to collaborate between projects.

Features
--------
* Multi platform (OS X, Windows, Linux) and multi format (VST, AU and others that I don't use); by using JUCE
* The sound engine [music-synthesizer-for-android](https://code.google.com/p/music-synthesizer-for-android) is closely modeled on the original DX7 characteristics
* 144 DAW automatable DX7 parameters available from one single panel
* Fully supports DX7 input and output Sysex messages; including controller change. This means that you can use this with a native DX7/TX7 as a patch editor
* Each operator have a realtime VU meter to know wich one is active
* Can load/save any DX7/TX7 sysex programs. It is also possible to save a single program into a different sysex file.

Changelog
---------
#### Version 0.4.0 (current sprint)
* Modulation wheel support
* Now using the [Obxd](https://obxd.wordpress.com) 4-pole lowpass filter implementation

Binary downloads
----------------
Dexed is not a finished product but it is stable enough to be used in a DAW environment: 
in normal operation it shouldn't crash and the VST state saving works.

* Version 0.3.0 [vst win32](http://le-son666.com/software/dexed/dexed-0.3.0-win32.zip) - [vst win64](http://le-son666.com/software/dexed/dexed-0.3.0-win64.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.3.0-osx.vst.zip)

Using as a DX7 editor
---------------------
This plugin can process original DX7 sysex messages. If you change a parameter, 
it will send the corresponding DX7 sysex to midi out. Not all DAW supports 
sysex; for example Ableton Live simply discard any sysex data. Reaper does 
process midi out, but doesn't pass any midi in sysex input data to the plugin.

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

Credits & thanks
----------------
* DX Synth engine : Raph Levien and the [msfa](https://code.google.com/p/music-synthesizer-for-android) team 
* LP Filter : Filatov Vadim (2DaT); taken from the excellent [Obxd](https://obxd.wordpress.com) project
* Original DX7 compilation : Jean-Marc Desprez (author of [SynprezFM](http://www.synprez.com/SynprezFM)) and to all the all crew who made these historic DX programs: Dave Benson, Frank Carvalho, Tim Conrardy, Jack Deckard, Chris Dodunski, Tim Garrett, Hitaye, Stephan Ibsen, Christian Jezreel, Narfman, Godric Wilkie
* markusthegeek direct implication for this project

TODO - Dexed 
------------
* Implement a better DX look and feel (amp, pitch, algo)
* Various code cleanup
* Standalone executable (for full support of the sysex editor)

TODO - msfa
-----------
* LFO/Mod-wheel Amplitude
* Algo 4 & 6 feedback
