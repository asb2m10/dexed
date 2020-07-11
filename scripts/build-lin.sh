#!/bin/sh

SCRIPTS_PATH=`dirname $(readlink -f $0)`
DEXED_PATH=${SCRIPTS_PATH%/scripts}

export CONFIG=Release
cd $DEXED_PATH/Builds/Linux && make
