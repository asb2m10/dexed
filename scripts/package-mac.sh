#!/bin/sh

GIT_TAG=`git rev-parse --short HEAD`
BUILDDATE=`date +%Y%m%d`
VERSION="bp-${GIT_TAG}-${BUILDDATE}"

rm -rf Builds/MacOSX/Dexed-Nightly
rm -rf *dmg
mkdir -p Builds/MacOSX/Dexed-Nightly

mkdir -p products/

cd Builds/MacOSX/build/Release
tar cf - Dexed.component/* | ( cd ../../Dexed-nightly ; tar xf - )
tar cf - Dexed.vst3/* | ( cd ../../Dexed-nightly ; tar xf - )
tar cf - Dexed.app/* | ( cd ../../Dexed-nightly ; tar xf - )
cd ../../../..

hdiutil create products/Dexed_${VERSION}.dmg -ov -volname "Dexed_${VERSION}" -fs HFS+ -srcfolder Builds/MacOSX/Dexed-nightly/

