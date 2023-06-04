#!/bin/bash

cat <<- EOH
# Automated build of Dexed

This is an automated build of Dexed from the head of our github
repository. This software is the absolute latest version of our
source code and as such may still include work in progress features
and other issues needing resolution. 

EOH
date
echo ""
echo "Most recent commits:" 
echo ""
git log --pretty=oneline | head -5
