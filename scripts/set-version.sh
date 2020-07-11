#!/bin/sh

GIT_TAG=`git rev-parse --short HEAD`
BUILDDATE=`date +%Y%m%d`
VERSION="${GIT_TAG}-BETA"
echo "Setting dexed version to '$VERSION'"

sed -e "s/#define DEXED_ID .*/#define DEXED_ID \"${VERSION}\"/" Source/Dexed.h > Source/Dexed_versioned.h
mv Source/Dexed_versioned.h Source/Dexed.h
