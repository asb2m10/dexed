#!/bin/sh

SCRIPTS_PATH=`dirname $(readlink -f $0)`
DEXED_PATH=${SCRIPTS_PATH%/scripts}

cd $DEXED_PATH
GIT_TAG=`git rev-parse --short HEAD`
BUILDDATE=`date +%Y%m%d`
VERSION="${GIT_TAG}-BETA-surge-synth-team"
echo "Setting dexed version to '$VERSION'"

sed -e "s/#define DEXED_ID .*/#define DEXED_ID \"${VERSION}\"/" $DEXED_PATH/Source/Dexed.h > $DEXED_PATH/Source/Dexed_versioned.h
mv $DEXED_PATH/Source/Dexed_versioned.h $DEXED_PATH/Source/Dexed.h
