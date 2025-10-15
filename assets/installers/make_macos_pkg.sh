#!/bin/bash

# =============================================================================
#
# This script is used to create a macOS installer package for a plugin. It has 
# evloved over time to be able to handle singing and notarze the package.
#
# It expects the following environment variables to be set:
# - MAC_SIGNING_CERT: The name (CN) of the certificate to sign the executable with
# - MAC_INSTALLING_CERT: The name (CN) of the certificate to sign the installation with
# - MAC_INSTALLING_B64: The base64 encoded certificate to sign the executable with
# - MAC_SIGNING_ID: The apple id to sign the package with
# - MAC_TEAM_ID: The team id to sign the package with
# - MAC_TEAM_PASSWORD : The password for the apple id
# Documentation for pkgbuild and productbuild: https://developer.apple.com/library/archive/documentation/DeveloperTools/Reference/DistributionDefinitionRef/Chapters/Distribution_XML_Ref.html
# Taken from surge tuning-note-claps: https://github.com/surge-synthesizer/tuning-note-claps/tree/main/scripts

# This force bash exit on error
# set -x # set this to log commands
set -e

# The product (eg plugin name, Dexed)
PRODUCT=$1
# The directory where the plugin binaries are built
INDIR=$2
# The version of the product (eg 1.0.0)
VERSION=$3
# The target directory where the final dmg is placed
TARGET_DIR=$4
# Output base filename (eg dexed-macOS-1.0.0)
OUTPUT_BASE_FILENAME=$5

TMPDIR="./installer-tmp"
VST3="${PRODUCT}.vst3"
AU="${PRODUCT}.component"
CLAP="${PRODUCT}.clap"
APP="${PRODUCT}.app"
PRODUCTFILE=`echo $PRODUCT | tr ' ' '-' | tr '[:upper:]' '[:lower:]'`

# We create a postinstall script since dropping the app in /Applications 
# does not work
mkdir -p $TMPDIR/AppResourcesPackageScript
cat > $TMPDIR/AppResourcesPackageScript/postinstall << PIEND
#!/bin/bash
rsync -r -p --delete "/tmp/dsgb-install/$APP" /Applications
chown -R \$USER:staff "/Applications/$APP";
PIEND
chmod 755 $TMPDIR/AppResourcesPackageScript/postinstall
mkdir -p $TARGET_DIR

echo "MAKE from $INDIR into $TARGET_DIR with $VERSION"

build_flavor()
{
    flavor=$1
    flavorprod=$2
    ident=$3
    loc=$4
    scripts=$5

    echo --- BUILDING ${PRODUCTFILE}_${flavor}.pkg from "$flavorprod" ---

    workdir=$TMPDIR/$flavor
    mkdir -p $workdir

    # In the past we would pkgbuild --analyze first to make a plist file, but we are perfectly fine with the
    # defaults, so we skip that step here. http://thegreyblog.blogspot.com/2014/06/os-x-creating-packages-from-command_2.html
    # was pretty handy in figuring that out and man pkgbuild convinced us to not do it, as did testing.
    #
    # The defaults only work if a component is a sole entry in a staging directory though, so synthesize that
    # by moving the product to a tmp dir

    cp -rp "$INDIR/$flavorprod" "$workdir"
    ls -l $workdir

    sca=""
    if [[ ! -z $scripts ]]; then
      sca="--scripts $scripts"
    fi

    if [[ ! -z $MAC_SIGNING_CERT ]]; then
      [[ -z $MAC_INSTALLING_CERT ]] && echo "You need an installing cert too " && exit 2
      echo "Singning $workdir/$flavorprod"
      codesign --force -s "$MAC_SIGNING_CERT" -o runtime --deep "$workdir/$flavorprod"
      codesign  -vvv --deep --stric "$workdir/$flavorprod"

      pkgbuild --sign "$MAC_INSTALLING_CERT" --root $workdir --identifier $ident --version $VERSION --install-location "$loc" "$TMPDIR/${PRODUCTFILE}_${flavor}.pkg" $sca || exit 1
    else
      pkgbuild --root $workdir --identifier $ident --version $VERSION --install-location "$loc" "$TMPDIR/${PRODUCTFILE}_${flavor}.pkg" $sca || exit 1
    fi

    #rm -rf $workdir
}

# =============================================================================
# create flavor packages
if [[ -d $INDIR/$VST3 ]]; then
    build_flavor "VST3" "$VST3" "com.digitalsuburban.${PRODUCTFILE}.vst3.pkg" "/Library/Audio/Plug-Ins/VST3"
  	VST3_PKG_REF="<pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.vst3.pkg\"/>"
  	VST3_CHOICE="<line choice=\"com.digitalsuburban.${PRODUCTFILE}.vst3.pkg\"/>"
  	VST3_CHOICE_DEF="<choice id=\"com.digitalsuburban.${PRODUCTFILE}.vst3.pkg\" visible=\"true\" start_selected=\"true\" title=\"${PRODUCT} VST3\"><pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.vst3.pkg\"/></choice><pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.vst3.pkg\" version=\"${VERSION}\" onConclusion=\"none\">${PRODUCTFILE}_VST3.pkg</pkg-ref>"
fi

if [[ -d $INDIR/$AU ]]; then
    build_flavor "AU" "$AU" "com.digitalsuburban.${PRODUCTFILE}.component.pkg" "/Library/Audio/Plug-Ins/Components"
  	AU_PKG_REF="<pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.component.pkg\"/>"
  	AU_CHOICE="<line choice=\"com.digitalsuburban.${PRODUCTFILE}.component.pkg\"/>"
  	AU_CHOICE_DEF="<choice id=\"com.digitalsuburban.${PRODUCTFILE}.component.pkg\" visible=\"true\" start_selected=\"true\" title=\"${PRODUCT} Audio Unit\"><pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.component.pkg\"/></choice><pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.component.pkg\" version=\"${VERSION}\" onConclusion=\"none\">${PRODUCTFILE}_AU.pkg</pkg-ref>"
fi

if [[ -d $INDIR/$APP ]]; then
    chmod +x "$INDIR/$APP/Contents/MacOS/$PRODUCT"
    build_flavor "APP" "$APP" "com.digitalsuburban.${PRODUCTFILE}.app.pkg" "/tmp/dsgb-install" $TMPDIR/AppResourcesPackageScript
  	APP_PKG_REF="<pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.app.pkg\"/>"
  	APP_CHOICE="<line choice=\"com.digitalsuburban.${PRODUCTFILE}.app.pkg\"/>"
  	APP_CHOICE_DEF="<choice id=\"com.digitalsuburban.${PRODUCTFILE}.app.pkg\" visible=\"true\" start_selected=\"true\" title=\"${PRODUCT} App\"><pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.app.pkg\"/></choice><pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.app.pkg\" version=\"${VERSION}\" onConclusion=\"none\">${PRODUCTFILE}_APP.pkg</pkg-ref>"
fi

if [[ -d $INDIR/$CLAP ]]; then
    build_flavor "CLAP" "$CLAP" "com.digitalsuburban.${PRODUCTFILE}.clap.pkg" "/Library/Audio/Plug-Ins/CLAP"
  	CLAP_PKG_REF="<pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.clap.pkg\"/>"
  	CLAP_CHOICE="<line choice=\"com.digitalsuburban.${PRODUCTFILE}.clap.pkg\"/>"
  	CLAP_CHOICE_DEF="<choice id=\"com.digitalsuburban.${PRODUCTFILE}.clap.pkg\" visible=\"true\" start_selected=\"true\" title=\"${PRODUCT} CLAP\"><pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.clap.pkg\"/></choice><pkg-ref id=\"com.digitalsuburban.${PRODUCTFILE}.clap.pkg\" version=\"${VERSION}\" onConclusion=\"none\">${PRODUCTFILE}_CLAP.pkg</pkg-ref>"
fi

# =============================================================================
# create distribution.xml
cat > $TMPDIR/distribution.xml << XMLEND
<?xml version="1.0" encoding="utf-8"?>
<installer-gui-script minSpecVersion="1">
    <os-version min="10.9"/>
    <title>${PRODUCT} ${VERSION}</title>
    <license file="License.txt" />
    <readme file="Readme.rtf" />
    ${VST3_PKG_REF}
    ${AU_PKG_REF}
    ${CLAP_PKG_REF}
    ${APP_PKG_REF}
    <options require-scripts="false" customize="always" hostArchitectures="x86_64,arm64"/>
    <choices-outline>
        ${VST3_CHOICE}
        ${AU_CHOICE}
        ${CLAP_CHOICE}
        ${APP_CHOICE}
    </choices-outline>
    ${VST3_CHOICE_DEF}
    ${AU_CHOICE_DEF}
    ${CLAP_CHOICE_DEF}
    ${APP_CHOICE_DEF}
</installer-gui-script>
XMLEND

# =============================================================================
# create the main package
pushd ${TMPDIR}
if [[ ! -z $MAC_INSTALLING_CERT ]]; then
    echo "Building SIGNED PKG"
    productbuild --sign "$MAC_INSTALLING_CERT" --distribution ./distribution.xml --package-path "." "$OUTPUT_BASE_FILENAME.pkg"
else 
    echo "Building UNSIGNED PKG"
    productbuild --distribution ./distribution.xml --package-path "." "$OUTPUT_BASE_FILENAME.pkg"
fi

popd

# =============================================================================
# create the final DMG
SetFile -a C "${TMPDIR}/${OUTPUT_BASE_FILENAME}.pkg"
mkdir -p "${TMPDIR}/${PRODUCTFILE}"
mv "${TMPDIR}/${OUTPUT_BASE_FILENAME}.pkg" "${TMPDIR}/${PRODUCTFILE}"
if [[ -f "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg" ]]; then
    rm "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg"
fi
hdiutil create /tmp/tmp.dmg -ov -volname "$OUTPUT_BASE_FILENAME" -fs HFS+ -srcfolder "${TMPDIR}/${PRODUCTFILE}/"
hdiutil convert /tmp/tmp.dmg -format UDZO -o "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg"

# =============================================================================
# Sign the DMG
if [[ ! -z $MAC_SIGNING_CERT ]]; then
    codesign --force -s "$MAC_SIGNING_CERT" --timestamp "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg"
    codesign -vvv "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg"
    xcrun notarytool submit "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg" --apple-id "${MAC_SIGNING_ID}" --team-id "${MAC_TEAM_ID}" --password "${MAC_TEAM_PASSWORD}" --wait
    xcrun stapler staple "${TARGET_DIR}/${OUTPUT_BASE_FILENAME}.dmg"
fi

echo Sucessfully created "${TARGET_DIR}/$OUTPUT_BASE_FILENAME.dmg"