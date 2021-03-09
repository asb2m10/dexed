#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
   PROJUCER=assets/JUCE/Projucer.app/Contents/MacOS/Projucer
else 
   PROJUCER=assets/JUCE/Projucer
fi

if ! [ -x $PROJUCER ]; then
   echo "Projucer was not found, please check the asset directory and why the Projucer executable is not there."
   exit 1
fi

if [ -z $VST2SDK_DIR ]; then
   echo "Generating build for VST3"
   $PROJUCER --resave Dexed.jucer
else
   echo "Generating build for VST2"
   sed -e 's/,buildVST3/,buildVST,buildVST3/' Dexed.jucer | \
	sed -e "s@VST2SDK_DIR@${VST2SDK_DIR}@" | \
	sed -e 's/PLUGINHOST_VST="0"/PLUGINHOST_VST="1"/' | \
	sed -e 's/buildVST="0"/buildVST="1"/' > Dexed-vst2.jucer
   mv Dexed.jucer Dexed-orig.jucer
   mv Dexed-vst2.jucer Dexed.jucer
   $PROJUCER --resave Dexed.jucer
   mv Dexed-orig.jucer Dexed.jucer
fi
