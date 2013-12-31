Dexed DX7 Software Emulator
===========================

Dexed is a multi platform, multiformat plugin synth that is closely modeled on the Yamaha DX7. 
Under the hood, it uses [music-synthesizer-for-android](https://code.google.com/p/music-synthesizer-for-android) 
for the synth engine and [JUCE](http://wwww.juce.com) as a plugin wrapper.

Dexed is licensed on the GPL v2. The msfa component (music synthesizer for android, see msfa in the source 
folder) stays on the Apache 2.0 license to able to collaborate between projects.

Binary downloads
----------------
It is far from finish but for those who want to try the "music-synthesizer-for-android" project
on a PC/Mac, you can download it [here](http://le-son666.com/software/dexed)

(New) Features 
--------------
* Multi platform (OS X, Windows, Linux) and multi format (VST, AU); by using JUCE
* The sound engine [music-synthesizer-for-android](https://code.google.com/p/music-synthesizer-for-android) is closely modeled on the original DX7 characteristics
* All of the 144 DX7 parameters are available from one single panel
* Fully supports DX7 input and output Sysex messages; including controller change. This means that you can use this with a native DX7/TX7 as a patch editor
* Each operator have a realtime VU meter to know wich one is active
* Can load any DX7/TX7 sysex programs. [See this](http://www.synprez.com/SynprezFM/) for great collection of DX7 patches (SynprezFM-II-builtins.tgz)

TODO - Dexed 
------------
* Implement a better DX look and feel
* Better implementation of the LPF filter
* Better display of the amplitude envelope
* Implement a display of the algo

TODO - msfa
-----------
* LFO Amplitude
* MOD Wheel action
