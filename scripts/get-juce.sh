#!/bin/bash

JUCE_VERSION=6.0.1
OS=`uname -s`
JOS=windows

if [[ $OS == "Darwin" ]]; then
  JOS=osx
fi

if [[ $OS == "Linux" ]]; then
  JOS=linux
fi

echo Downloading for $JOS

mkdir -p assets
curl -L -o assets/juce-${JUCE_VERSION}-${JOS}.zip https://github.com/juce-framework/JUCE/releases/download/${JUCE_VERSION}/juce-${JUCE_VERSION}-${JOS}.zip
cd assets && unzip juce-${JUCE_VERSION}-${JOS}.zip
