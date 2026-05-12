#include "PluginProcessor.h"
#include "parameter/Model.h"
#include "StateLegacy.h"

//==============================================================================
void DexedAudioProcessor::getStateInformation(MemoryBlock& destData) {
    auto state = parameters.copyState();
    juce::ValueTree storage { IDs::root };
    storage.setProperty(IDs::version, DexedApvts::MODEL_VERSION, nullptr);
    storage.addChild(state, -1, nullptr);
    storage.addChild(parameters.rootVt.getChildWithName(IDs::profile).createCopy(), -1, nullptr);

    juce::ValueTree tuningVt("tuning");
    tuningVt.setProperty("scl", juce::String(currentSCLData), nullptr);
    tuningVt.setProperty("kbm", juce::String(currentKBMData), nullptr);
    storage.addChild(tuningVt, -1, nullptr);

    std::unique_ptr<juce::XmlElement> xml(storage.createXml());
    copyXmlToBinary(*xml, destData);
}

void DexedAudioProcessor::setStateInformation(const void* source, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (source, sizeInBytes));
    if (xmlState != nullptr) {
        if (xmlState->hasTagName(parameters.rootVt.getType())) {
            ValueTree storage = juce::ValueTree::fromXml(*xmlState);

            juce::String version = storage.getProperty(IDs::version);
            if ( version != juce::String(DexedApvts::MODEL_VERSION) ) {
                TRACE("Incompatible state version %s (current is %s)", version.toRawUTF8(), DexedApvts::MODEL_VERSION);
                return;
            }

            parameters.replaceState(storage.getChildWithName(IDs::parameters));
            juce::ValueTree storageProfile = storage.getChildWithName(IDs::profile);
            parameters.rootVt.getChildWithName(IDs::profile).getChildWithName(IDs::midiCCMappings).
                copyPropertiesAndChildrenFrom(storageProfile.getChildWithName(IDs::midiCCMappings), nullptr);

            juce::ValueTree tuningVt = storage.getChildWithName("tuning");
            if (tuningVt.isValid()) {
                juce::String sclData = tuningVt.getProperty("scl").toString();
                juce::String kbmData = tuningVt.getProperty("kbm").toString();
                retuneToStandard();
                if (sclData.isNotEmpty())
                    applySCLTuning(sclData.toStdString());
                if (kbmData.isNotEmpty())
                    applyKBMMapping(kbmData.toStdString());
            }
        } else if ( xmlState->hasTagName("dexedState") ) {
            // Format is 1.0 legacy
            setStateInformation10(this, xmlState.get());
            // implement legacy loader
        } else {
            TRACE("No valid state found");
        }
    }
}

// void DexedAudioProcessor::getStateInformation(MemoryBlock& destData) {
//     // You should use this method to store your parameters in the memory block.
//     // You could do that either as raw data, or use the XML or ValueTree classes
//     // as intermediaries to make it easy to save and load complex data.
//
//     // used to SAVE plugin state
//
//     XmlElement dexedState("dexedState");
//     XmlElement *dexedBlob = dexedState.createNewChildElement("dexedBlob");
//
//     dexedState.setAttribute("cutoff", fx.uiCutoff);
//     dexedState.setAttribute("reso", fx.uiReso);
//     dexedState.setAttribute("gain", fx.uiGain);
//     dexedState.setAttribute("currentProgram", currentProgram);
//     dexedState.setAttribute("engineType", (int) engineType);
//     dexedState.setAttribute("masterTune", controllers.masterTune);
//     //dexedState.setAttribute("opSwitch", controllers.opSwitch);
//     dexedState.setAttribute("transpose12AsScale", controllers.transpose12AsScale ? 1 : 0 );
//     dexedState.setAttribute("mpeEnabled", controllers.mpeEnabled ? 1 : 0 );
//     dexedState.setAttribute("mpePitchBendRange", controllers.mpePitchBendRange );
//     dexedState.setAttribute("monoMode", monoMode ? 1 : 0);
//     dexedState.setAttribute("portamento", controllers.portamento_cc);
//     dexedState.setAttribute("glissando", controllers.portamento_gliss_cc ? 1 : 0);
//
//     char mod_cfg[15];
//     controllers.wheel.setConfig(mod_cfg);
//     dexedState.setAttribute("wheelMod", mod_cfg);
//     controllers.foot.setConfig(mod_cfg);
//     dexedState.setAttribute("footMod", mod_cfg);
//     controllers.breath.setConfig(mod_cfg);
//     dexedState.setAttribute("breathMod", mod_cfg);
//     controllers.at.setConfig(mod_cfg);
//     dexedState.setAttribute("aftertouchMod", mod_cfg);
//
//     if( currentSCLData.size() > 1 || currentKBMData.size() > 1 )
//     {
//         auto tuningx = dexedState.createNewChildElement("dexedTuning" );
//         auto sclx = tuningx->createNewChildElement("scl");
//         sclx->addTextElement(currentSCLData);
//         auto kbmx = tuningx->createNewChildElement("kbm");
//         kbmx->addTextElement(currentKBMData);
//     }
//
//     if ( activeFileCartridge.exists() )
//         dexedState.setAttribute("activeFileCartridge", activeFileCartridge.getFullPathName());
//
//     NamedValueSet blobSet;
//     //blobSet.set("sysex", var((void *) currentCart.getVoiceSysex(), 4104));
//     blobSet.set("program", var((void *) activeProgram.getUnpackedData(), 161));
//
//     blobSet.copyToXmlAttributes(*dexedBlob);
//
//     XmlElement *midiCC = dexedState.createNewChildElement("midiCC");
//     HashMap<int, Ctrl *>::Iterator i(mappedMidiCC);
//     while(i.next()) {
//         XmlElement *ccMapping = midiCC->createNewChildElement("mapping");
//         ccMapping->setAttribute("cc", i.getKey());
//         Ctrl *ctrl = i.getValue();
//         ccMapping->setAttribute("target", ctrl->label);
//     }
//
//     copyXmlToBinary(dexedState, destData);
// }
