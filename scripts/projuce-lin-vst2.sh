#!/bin/bash

if [ -z $VST2SDK_DIR ]; then
    echo "VST2SDK_DIR is not set. Please point it to a directory containing the VST2 SDK"
    exit 1
else
   sed -e 's/,buildVST3/,buildVST,buildVST3/' Dexed.jucer | \
	sed -e "s@VST2SDK_DIR@${VST2SDK_DIR}@" | \
	sed -e 's/PLUGINHOST_VST="0"/PLUGINHOST_VST="1"/' | \
	sed -e 's/buildVST="0"/buildVST="1"/' > Dexed-vst2.jucer
   mv Dexed.jucer Dexed-orig.jucer
   mv Dexed-vst2.jucer Dexed.jucer
   assets/JUCE/Projucer --resave Dexed.jucer
   mv Dexed-orig.jucer Dexed.jucer
fi

