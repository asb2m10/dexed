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
    prop.setValue(String("pitchRange"), controllers.pitch_range_up); // for backwards compat
    prop.setValue(String("pitchRangeUp"), controllers.pitch_range_up);
    prop.setValue(String("pitchRangeDn"), controllers.pitch_range_dn);
    prop.setValue(String("pitchStep"), controllers.pitch_step);

    prop.setValue(String("sysexIn"), sysexComm.getInput());
    prop.setValue(String("sysexOut"), sysexComm.getOutput());
    prop.setValue(String("sysexChl"), sysexComm.getChl());

    prop.setValue(String("showKeyboard"), showKeyboard);

    prop.setValue(String("zoomFactor"), zoomFactor);

    prop.save();
}
