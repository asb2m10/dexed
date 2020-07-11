#!/bin/sh

GIT_TAG=`git rev-parse --short HEAD`
BUILDDATE=`date +%Y%m%d`
VERSION="${GIT_TAG}-${BUILDDATE}"
UN=`uname -a`

rm -rf Builds/Linux/Dexed-Nightly
mkdir -p Builds/Linux/Dexed-Nightly

mkdir -p products/

cd Builds/Linux/build/
cp -R Dexed Dexed.vst3 ../Dexed-Nightly
cd ..

cat <<EOF > Dexed-Nightly/BuildInfo.txt
Build in azure pipeline
$VERSION
$UN

EOF

md5sum Dexed-Nightly/Dexed* >> Dexed-Nightly/BuildInfo.txt

cat Dexed-Nightly/BuildInfo.txt

pwd
tar cvzf ../../products/Dexed_ubuntu_linux_${VERSION}.tgz Dexed-Nightly

