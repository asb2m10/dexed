#!/bin/sh

GIT_TAG=`git rev-parse --short HEAD`
BUILDDATE=`date +%Y%m%d`
VERSION="bp-${GIT_TAG}-${BUILDDATE}"

rm -rf Builds/Linux/Dexed-Nightly
mkdir -p Builds/Linux/Dexed-Nightly

mkdir -p products/

cd Builds/Linux/build/
cp Dexed Dexed.a ../Dexed-Nightly
cd ..

pwd
echo tar cvzf ../../products/Dexed_${VERSION}.tgz Dexed-Nightly
tar cvzf ../../products/Dexed_${VERSION}.tgz Dexed-Nightly

