#!/bin/bash
#
# This shell script does, more or less, waht the azure pipelines do for you if you
# just want a clean build

SCRIPTS_PATH=`dirname $(readlink -f $0)`
DEXED_PATH=${SCRIPTS_PATH%/scripts}

OS=`uname -s`
JOS=win

if [[ $OS == "Darwin" ]]; then
  JOS=mac
fi

if [[ $OS == "Linux" ]]; then
  JOS=lin
fi


if [ ! -f "assets/JUCE/LICENSE.md" ]; then
    $DEXED_PATH/scripts/get-juce.sh
fi

$DEXED_PATH/scripts/projuce-${JOS}.sh
$DEXED_PATH/scripts/build-${JOS}.sh
$DEXED_PATH/scripts/package-${JOS}.sh
