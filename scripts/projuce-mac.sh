#!/bin/sh

SCRIPTS_PATH=`dirname $(readlink -f $0)`
DEXED_PATH=${SCRIPTS_PATH%/scripts}

## TODO: Check assets exists
cd $DEXED_PATH
assets/JUCE/Projucer.app/Contents/MacOS/Projucer --resave Dexed.jucer
