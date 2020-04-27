#!/bin/bash

cat <<- EOH
# Automated build of surge-synth-team dexed fork

This is a build of our fork of dexed from the very top of our git repository.
Currently available for Win and Mac as VST3/Standalone/AU (Mac). Linux standalone
coming soon.

This is beta software. If you want a production version of dexed you should find 
the original. This may not work and may be generally bad for you and your system. But
we are using it and moving towards a point where we think we could do something like
a solid release.

If you understand that, download below, if not, perhaps hop in our slack and
chat with us? 

EOH
date
echo ""
echo "Most recent commits:" 
echo ""
git log --pretty=oneline | head -5
