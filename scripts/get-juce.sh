#!/bin/sh

JUCE_VERSION=5.4.5
OS=`uname -s`
JOS=windows

if [[ $OS == "Darwin" ]]; then
  JOS=osx
fi

echo Downloading for $JOS

mkdir -p assets
curl -o assets/juce-${JUCE_VERSION}-${JOS}.zip https://d30pueezughrda.cloudfront.net/juce/juce-${JUCE_VERSION}-${JOS}.zip
cd assets && unzip juce-${JUCE_VERSION}-${JOS}.zip
