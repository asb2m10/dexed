#!/bin/bash

cat <<- EOH
# Automatically generated Dexed Branch release

THIS IS NOT A SUPPORTED OR OFFICIAL RELEASE OF DEXED. YOU SHOULD NOT
USE IT UNLESS YOU ARE CHATTING WITH BACONPAUL!

Like, really. Go find the offical release and use that instead!

EOH
date
echo ""
echo "Most recent commits:" 
echo ""
git log --pretty=oneline | head -5
