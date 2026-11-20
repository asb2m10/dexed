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

    // Pitch bend settings are stored as 0-48 / 0-12 values; "pitchRange" is the pre-1.1 name of the up range
    auto setPitchParam = [this](const juce::String &id, int value) {
        if ( auto *p = parameters.getParameter(id) ) {
            float v = p->convertFrom0to1(1.0f) < (float) value ? p->convertFrom0to1(1.0f) : (float) value;
            p->setValueNotifyingHost(p->convertTo0to1(v));
        }
    };

    if ( prop.containsKey( String("pitchRangeUp") ) || prop.containsKey( String("pitchRange") ) ) {
        int up = prop.containsKey( String("pitchRangeUp") ) ? prop.getIntValue( String("pitchRangeUp") )
                                                             : prop.getIntValue( String("pitchRange") );
        setPitchParam(IDs::pitchRangeUp.name, up);
        // Old versions had a single range, the down range defaults to the up one
        int dn = prop.containsKey( String("pitchRangeDn") ) ? prop.getIntValue( String("pitchRangeDn") ) : up;
        setPitchParam(IDs::pitchRangeDown.name, dn);
    } else if ( prop.containsKey( String("pitchRangeDn") ) ) {
        setPitchParam(IDs::pitchRangeDown.name, prop.getIntValue( String("pitchRangeDn") ));
    }

    if ( prop.containsKey( String("pitchStep") ) ) {
        setPitchParam(IDs::pitchStep.name, prop.getIntValue( String("pitchStep") ));
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
