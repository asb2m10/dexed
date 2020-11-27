#!/bin/bash
#
# This shell script does, more or less, waht the azure pipelines do for you if you
# just want a clean build

OS=`uname -s`
JOS=win

if [[ $OS == "Darwin" ]]; then
  JOS=mac
fi

if [[ $OS == "Linux" ]]; then
  JOS=lin
fi


if [ ! -f "assets/JUCE/LICENSE.md" ]; then
    ./scripts/get-juce.sh
fi

./scripts/projuce-${JOS}.sh
./scripts/build-${JOS}.sh
./scripts/package-${JOS}.sh

