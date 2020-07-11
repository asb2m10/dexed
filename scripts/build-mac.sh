#!/bin/sh

SCRIPTS_PATH=`dirname $(readlink -f $0)`
DEXED_PATH=${SCRIPTS_PATH%/scripts}

xcodebuild build -configuration Release -project $DEXED_PATH/Builds/MacOSX/Dexed.xcodeproj
