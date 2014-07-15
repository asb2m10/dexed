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
* Multi platform (OS X, Windows or Linux) and multi format (VST, AU and others that I don't use); by using JUCE
* The sound engine [music-synthesizer-for-android](https://code.google.com/p/music-synthesizer-for-android) is closely modeled on the original DX7 characteristics
* 144 DAW automatable DX7 parameters available from one single panel
* Fully supports DX7 input and output Sysex messages; including controller change. This means that you can use this with a native DX7/TX7 as a patch editor and sysex manager
* Each operator have a realtime VU meter to know which one is active
* Can load/save any DX7/TX7 sysex programs. It is also possible to save a single program into a different sysex file.

Binary downloads
----------------
Dexed is not a finished product but it is stable enough to be used in a DAW environment: 
in normal operation it shouldn't crash and the VST state saving works. If you don't see the 
new version here but you see it in the change log, it's because this version is in development 
(current sprint). Only officials (tested) builds are listed here.


* Version 0.6.0 [vst win32/x64](http://le-son666.com/software/dexed/dexed-0.6.0-win.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.6.0-osx.vst.zip)
* Version 0.5.1 [vst win32](http://le-son666.com/software/dexed/dexed-0.5.1-win32.zip) - [vst win64](http://le-son666.com/software/dexed/dexed-0.5.1-win64.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.5.1-osx.vst.zip)
* Version 0.5.0 [vst win32](http://le-son666.com/software/dexed/dexed-0.5.0a-win32.zip) - [vst win64](http://le-son666.com/software/dexed/dexed-0.5.0a-win64.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.5.0-osx.vst.zip)
* Version 0.4.0 [vst win32](http://le-son666.com/software/dexed/dexed-0.4.0-win32.zip) - [vst win64](http://le-son666.com/software/dexed/dexed-0.4.0-win64.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.4.0-osx.vst.zip)
* Version 0.3.0 [vst win32](http://le-son666.com/software/dexed/dexed-0.3.0-win32.zip) - [vst win64](http://le-son666.com/software/dexed/dexed-0.3.0-win64.zip) - [vst os x](http://le-son666.com/software/dexed/dexed-0.3.0-osx.vst.zip)

Changelog
---------
#### Version 0.6.0
* Added external midi interface to send / receive sysex messages
* Fix Tracktion crash upon startup 
* Middle C (transpose) now works
* Identify the parameter knob/switch by simply moving the mouse over it
* Knobs now works with vertical mouse drags
* User DX7 zip cartridges

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

User DX7 zip cartridges
-----------------------
It is possible to enjoy your DX7 sysex collection from one single zip file. Simply move your zipped
sysex content to where you have installed Dexed (VST plugins dir). Then rename the zip file to 
`Dexed_cart.zip`. You can modify this file anytime you want, Dexed will simply reparse it when the 
file is changed. Directories in the zip file will be transformed into submenu when you hit the 
[CART] button.

Using as a DX7 editor
---------------------
You can use this plugin to edit your real DX7 patchs. Since midi sysex send/receive are quirky for the 
majority of VST hosts, any sysex messages (editor messages) must be send or received with the external Dexed
midi interface. This is configurable in the "PARM" panel. By setting a DX7 in / DX7 out midi interface, 
Dexed will listen to specific program/cartridge changes from your DX7 and send controller/program/cartridge
you edit.

### Prerequisite
* Before you use this interface, your DX7 must be configured to send or receive sysex messages. Do this by hitting [FUNCTION] and [8] button 
* Check the midi channel. By reading some of DX7 literature, most DX7 are supposed to support only 1 channel. Unless you are running a cluster of DX7, you should keep this to 1.
* Press [8] again and you should see: "SYS INFO AVAIL" or "SYS INFO UNAVAIL". Keep it to "SYS INFO AVAIL" to be able to receive sysex messages. This step is not required on a TX7.
* You need to remove MEMORY PROTECT on the internal or cartridge memory to be able to receive a Dexed 32 voice bulk dump. This is done by using the [MEMORY PROTECT] button and then by pushing [NO].

### To send to your DX7
* If the midi out port is configured, any parameter change will be sent to your DX7.
* If the midi out port is configured, you should see the [SEND] button, if not this button won't be available.
* You can use this [SEND] button on Dexed to send program or cartridge changes to your DX7. Be sure you have set [MEMORY PROTECT] is set to NO so the cartridge (internal or external one) on the DX7 can be overridden.

### To receive from your DX7
* The midi in port must be configured.
* Receive a program by simply using [MEMORY SELECT] then the program you want to transmit ([1] to [32]).
* Receive a cartridge by pressing [FUNCTION] then 3 times on [8] you should see " MIDI TRANSMIT ? ". If you hit yes, Dexed should receive the complete cartridge.

### Troubleshooting
* If you play on your DX7 keyboard, the |DX7 In| light should be flashing. Use this to test the midi in communication.
* If you click/play on the Dexed virtual keyboard, it will send the corresponding midi out note to the DX7 port; if configured. Use this to test the midi out communication.
* If the data sent is corrupted (wrong checksum, DX7 crash), it might be the midi interface implementation. Default Windows USB midi driver are known to send corrupt sysex data. If it is the case, use a third party device (like the midiman uno) that have his own USB driver.
* If you are unable to open the interface (error message after the using the [PARM] dialog), it might be because the midi driver doesn't support multiple clients (common on Windows). Be sure that there are no other applications that are using the same midi interface. 

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
* Some sysex files seems to be corrupted : Even if the sysex checksum doesn't match, Dexed will try to load it (this is a kind of randomize feature). Right now Dexed supports only original DX7 sysex, other DX family sysex (like the DX21) is considered as random data.

Credits & thanks
----------------
* DX Synth engine : Raph Levien and the [msfa](https://code.google.com/p/music-synthesizer-for-android) team 
* LP Filter : Filatov Vadim (2DaT); taken from the excellent [Obxd](https://obxd.wordpress.com) project
* DX7 program compilation : Jean-Marc Desprez (author of [SynprezFM](http://www.synprez.com/SynprezFM)) 
* DX7 programs : Dave Benson, Frank Carvalho, Tim Conrardy, Jack Deckard, Chris Dodunski, Tim Garrett, Hitaye, Stephan Ibsen, Christian Jezreel, Narfman, Godric Wilkie
* markusthegeek direct implication for this project

Planned milestones 
------------------
* 0.6 DX7 editor + user cartridges
* 0.7 msfa engine tuning
* 0.8 UI redesign

TODO - Dexed 
------------
* Yamaha 4 operators (DX21/DX27/DX100) sysex import
* Various code cleanup

TODO - msfa
-----------
* The sample rate should not change the response of the envelopes
* Portamento implementation
* LFO/Mod-wheel amplitude
* Algo 4 & 6 feedback
