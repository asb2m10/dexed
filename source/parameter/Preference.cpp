#include "PluginProcessor.h"
#include "Model.h"

void DexedAudioProcessor::loadPreference() {
    File propFile = DexedAudioProcessor::dexedAppDir.getChildFile("Dexed.xml");
    PropertiesFile::Options prefOptions;
    PropertiesFile prop(propFile, prefOptions);

    if ( ! prop.isValidFile() ) {
        return;
    }

    if ( prop.containsKey( String("normalizeDxVelocity") ) ) {
        normalizeDxVelocity = prop.getIntValue( String("normalizeDxVelocity") );
    }

    if ( prop.containsKey( String("pitchRange") ) ) {
        controllers.values_[kControllerPitchRangeUp] = prop.getIntValue( String("pitchRange") );
    }

    if ( prop.containsKey( String("pitchRangeDn") ) ) {
        controllers.values_[kControllerPitchRangeDn] = prop.getIntValue( String("pitchRangeDn") );
    } else {
        controllers.values_[kControllerPitchRangeDn] = controllers.values_[kControllerPitchRangeUp];
    }

    if ( prop.containsKey( String("pitchStep") ) ) {
        controllers.values_[kControllerPitchStep] = prop.getIntValue( String("pitchStep") );
    }

    if ( prop.containsKey( String("sysexIn") ) ) {
        sysexComm.setInput( prop.getValue("sysexIn") );
    }

    if ( prop.containsKey( String("sysexOut") ) ) {
        sysexComm.setOutput( prop.getValue("sysexOut") );
    }

    if ( prop.containsKey( String("sysexChl") ) ) {
        sysexComm.setChl( prop.getIntValue( String("sysexChl") ) );
    }

    if ( prop.containsKey( String("engineType") ) ) {
        setEngineType(prop.getIntValue(String("engineType")));
    }

    if ( prop.containsKey( String("showKeyboard") ) ) {
        showKeyboard = prop.getIntValue( String("showKeyboard") );
    }

    if ( prop.containsKey( String("wheelMod") ) ) {
        controllers.wheel.parseConfig(prop.getValue(String("wheelMod")).toRawUTF8());
    }

    if ( prop.containsKey( String("footMod") ) ) {
        controllers.foot.parseConfig(prop.getValue(String("footMod")).toRawUTF8());
    }

    if ( prop.containsKey( String("breathMod") ) ) {
        controllers.breath.parseConfig(prop.getValue(String("breathMod")).toRawUTF8());
    }

    if ( prop.containsKey( String("aftertouchMod") ) ) {
        controllers.at.parseConfig(prop.getValue(String("aftertouchMod")).toRawUTF8());
    }

    if ( prop.containsKey( String("zoomFactor") ) ) {
        zoomFactor = prop.getDoubleValue(String("zoomFactor"));
    }

    controllers.refresh();
}

void DexedAudioProcessor::savePreference() {
    File propFile = DexedAudioProcessor::dexedAppDir.getChildFile("Dexed.xml");
    PropertiesFile::Options prefOptions;
    PropertiesFile prop(propFile, prefOptions);

    prop.setValue(String("normalizeDxVelocity"), normalizeDxVelocity);
    prop.setValue(String("pitchRange"), controllers.values_[kControllerPitchRangeUp]); // for backwards compat
    prop.setValue(String("pitchRangeUp"), controllers.values_[kControllerPitchRangeUp]);
    prop.setValue(String("pitchRangeDn"), controllers.values_[kControllerPitchRangeDn]);
    prop.setValue(String("pitchStep"), controllers.values_[kControllerPitchStep]);

    prop.setValue(String("sysexIn"), sysexComm.getInput());
    prop.setValue(String("sysexOut"), sysexComm.getOutput());
    prop.setValue(String("sysexChl"), sysexComm.getChl());

    prop.setValue(String("showKeyboard"), showKeyboard);

    char mod_cfg[15];
    controllers.wheel.setConfig(mod_cfg);
    prop.setValue(String("wheelMod"), mod_cfg);
    controllers.foot.setConfig(mod_cfg);
    prop.setValue(String("footMod"), mod_cfg);
    controllers.breath.setConfig(mod_cfg);
    prop.setValue(String("breathMod"), mod_cfg);
    controllers.at.setConfig(mod_cfg);
    prop.setValue(String("aftertouchMod"), mod_cfg);

    prop.setValue(String("engineType"), (int) engineType);
    prop.setValue(String("zoomFactor"), zoomFactor);

    prop.save();
}
