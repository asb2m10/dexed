Dexed FM Synth Software Emulator
================================

Dexed is a multi platform, multi format plugin synth that is closely modeled on the Yamaha DX7. 
Under the hood, it uses [music-synthesizer-for-android](https://code.google.com/p/music-synthesizer-for-android) 
for the synth engine and [JUCE](http://wwww.juce.com) as a plugin wrapper.

The goal of this project is to be a great tool/companion for the original DX7. Yes, the sound engine 
with 'float' values parameter; different waveform (à la TX81z) would be great but anything that 
goes beyond the DX7 should will be a fork of this project. This is to keep the compatiblity with
the original synth.

Dexed is licensed on the GPL v2. The msfa component (acronym for music synthesizer for android, see msfa 
in the source folder) stays on the Apache 2.0 license to able to collaborate between projects.

Features
--------
* Multi platform (OS X, Windows, Linux) and multi format (VST, AU and others that I don't use); by using JUCE
* The sound engine [music-synthesizer-for-android](https://code.google.com/p/music-synthesizer-for-android) is closely modeled on the original DX7 characteristics
* All of the 144 DX7 parameters are available from one single panel
* Fully supports DX7 input and output Sysex messages; including controller change. This means that you can use this with a native DX7/TX7 as a patch editor
* Each operator have a realtime VU meter to know wich one is active
* Can load/save any DX7/TX7 sysex programs. It is also possible to save a single program into a different sysex file.

Binary downloads
----------------
Dexed is not a finished product but it is stable enough to be used in a DAW environment: in normal operation it doesn't crash
and the VST state saving works.

* Version 0.3.0 [vst win32](http://le-son666.com/software/dexed/dexed-0.3.0-win32.zip) 
- [vst win64](http://le-son666.com/software/dexed/dexed-0.3.0-win64.zip)
- [vst os x](http://le-son666.com/software/dexed/dexed-0.3.0-osx.vst.zip)

Using as a DX7 editor
---------------------
This plugin can process original DX7 messages. If you change a parameter, it will send the 
corresponding DX7 sysex to midi out. Not all DAW supports sysex; for example
Ableton Live simply discard any sysex data. Reaper does process midi out, but doesn't pass any
midi in sysex input data to the plugin.

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
Raph Levien and msfa team, markusthegeek, Jean-Marc Desprez	and all the crew who made these historic
DX programs: (Dave Benson, Frank Carvalho, Tim Conrardy, Jack Deckard, Chris Dodunski, Tim Garrett, 
Hitaye, Stephan Ibsen, Christian Jezreel, Narfman, Godric Wilkie)

TODO - Dexed 
------------
* Implement a better DX look and feel (amp, pitch, algo)
* Better implementation of the LPF filter
* Various code cleanup
* Standalone executable (for full support of the editor)

TODO - msfa
-----------
* LFO Amplitude
* MOD Wheel action
* Algo 4 & 6 feedback
