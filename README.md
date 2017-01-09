Dexed - FM Plugin Synth
=======================

Dexed is a multi platform, multi format plugin synth that is closely modeled on the Yamaha DX7. 
Under the hood it uses [music-synthesizer-for-android](https://github.com/google/music-synthesizer-for-android) 
for the synth engine and [JUCE](http://wwww.juce.com) as a plugin wrapper.

The goal of this project is to be a tool/companion for the original DX7. Sound engine 
with 'float' value parameters, different waveform à la TX81z would be great but anything that 
goes beyond the DX7 should and will be a fork of this project. This is to keep the compatibility with
the original machine.

Dexed is licensed on the GPL v3. The msfa component (acronym for music synthesizer for android, see msfa 
in the source folder) stays on the Apache 2.0 license to able to collaborate between projects.

[Dexed User Website](http://asb2m10.github.io/dexed)

This is a fork of the original Dexed JUCE plugin. Use the branch "native-lv2"
for a stripped down version which has no own UI and should be used only as a
LV2 plugin (e.g. for mod-host).

Currently (Oct 2016) it is not running or nearly ready. If someone has time and
fun developing please contact dcoredump@gmail.com!

```
git clone https://github.com/dcoredump/dexed.git
git checkout origin native-lv2
cd dexed/src
make install
```
 
