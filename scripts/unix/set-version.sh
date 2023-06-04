#!/bin/sh

GIT_TAG=`git rev-parse --short HEAD`
BUILDDATE=`date +%Y%m%d`
if [ $1 = "Nightly" ]; then 
    VERSION="${GIT_TAG}-Nightly"
else
    VERSION=$1
fi

echo "Setting dexed version to '$VERSION'"

sed -e "s/#define DEXED_ID .*/#define DEXED_ID \"${VERSION}\"/" Source/Dexed.h > Source/Dexed_versioned.h
mv Source/Dexed_versioned.h Source/Dexed.h
