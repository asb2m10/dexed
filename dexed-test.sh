#!/bin/sh
export LV2_PATH="/usr/local/lib/lv2"
jackd --silent -r -d dummy &
mod-host -i <<ENDOFDATA
add https://github.com/dcoredump/dexed.lv2 0
add http://gareus.org/oss/lv2/stepseq#s8n8 1
connect effect_1:midiout effect_0:midi_in
connect effect_0:lv2_audio_out_1 system:playback_1
connect effect_0:lv2_audio_out_1 system:playback_2
param_set 1 grid_1_1 1
param_set 1 grid_2_3 1
ENDOFDADA
