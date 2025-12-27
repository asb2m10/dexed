#include "PluginProcessor.h"

//==============================================================================
void DexedAudioProcessor::getStateInformation(MemoryBlock& destData) {
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DexedAudioProcessor::setStateInformation(const void* source, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (source, sizeInBytes));
    if (xmlState != nullptr) {
        if (xmlState->hasTagName(parameters.state.getType())) {
            // Format is 1.1
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        } else if ( xmlState->hasTagName("dexedState") ) {
            // Format is 1.0 legacy
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

// void setStateInformationLegacy(const void* source, int sizeInBytes) {
//     // You should use this method to restore your parameters from this memory block,
//     // whose contents will have been created by the getStateInformation() call.
//
//     // used to LOAD plugin state
//     std::unique_ptr<XmlElement> root(getXmlFromBinary(source, sizeInBytes));
//
//     if (root == nullptr) {
//         TRACE("unknown state format");
//         return;
//     }
//
//     fx.uiCutoff = root->getDoubleAttribute("cutoff");
//     fx.uiReso = root->getDoubleAttribute("reso");
//     fx.uiGain = root->getDoubleAttribute("gain");
//     currentProgram = root->getIntAttribute("currentProgram");
//
//     String opSwitchValue = root->getStringAttribute("opSwitch");
//     //TRACE("opSwitch value %s", opSwitchValue.toRawUTF8());
//     // if ( opSwitchValue.length() != 6 ) {
//     //     strcpy(controllers.opSwitch, "111111");
//     // } else {
//     //     strncpy(controllers.opSwitch, opSwitchValue.toRawUTF8(), 6);
//     // }
//
//     controllers.wheel.parseConfig(root->getStringAttribute("wheelMod").toRawUTF8());
//     controllers.foot.parseConfig(root->getStringAttribute("footMod").toRawUTF8());
//     controllers.breath.parseConfig(root->getStringAttribute("breathMod").toRawUTF8());
//     controllers.at.parseConfig(root->getStringAttribute("aftertouchMod").toRawUTF8());
//
//     setEngineType(root->getIntAttribute("engineType", 1));
//     monoMode = root->getIntAttribute("monoMode", 0);
//     controllers.masterTune = root->getIntAttribute("masterTune", 0);
//     controllers.transpose12AsScale = ( root->getIntAttribute("transpose12AsScale", 1) != 0 );
//
//     controllers.mpePitchBendRange = ( root->getIntAttribute("mpePitchBendRange", 24) );
//     controllers.mpeEnabled = ( root->getIntAttribute("mpeEnabled", 0) != 0 );
//
//     controllers.portamento_cc = ( root->getIntAttribute("portamento", 0) );
//     controllers.portamento_enable_cc = controllers.portamento_cc > 1;
//     controllers.portamento_gliss_cc = ( root->getIntAttribute("glissando", 0) );
//     controllers.refresh();
//
//     File possibleCartridge = File(root->getStringAttribute("activeFileCartridge"));
//     if ( possibleCartridge.exists() )
//         activeFileCartridge = possibleCartridge;
//
//     auto tuningParent = root->getChildByName( "dexedTuning" );
//     if( tuningParent )
//     {
//         auto sclx = tuningParent->getChildByName( "scl" );
//         auto kbmx = tuningParent->getChildByName( "kbm" );
//         std::string s = "";
//         if( sclx && sclx->getFirstChildElement() && sclx->getFirstChildElement()->isTextElement() )
//         {
//             s = sclx->getFirstChildElement()->getText().toStdString();
//             if( s.size() > 1 )
//                 applySCLTuning(s);
//         }
//
//         std::string k = "";
//         if( kbmx && kbmx->getFirstChildElement() && kbmx->getFirstChildElement()->isTextElement() )
//         {
//             k = kbmx->getFirstChildElement()->getText().toStdString();
//             if( k.size() > 1 )
//                 applyKBMMapping(k);
//         }
//     }
//
//     XmlElement *dexedBlob = root->getChildByName("dexedBlob");
//     if ( dexedBlob == NULL ) {
//         TRACE("dexedBlob element not found");
//         return;
//     }
//
//     NamedValueSet blobSet;
//     blobSet.setFromXmlAttributes(*dexedBlob);
//
//     var sysex_blob = blobSet["sysex"];
//     var program = blobSet["program"];
//
//     if ( sysex_blob.isVoid() || program.isVoid() ) {
//         TRACE("unknown serialized blob data");
//         return;
//     }
//
//     Cartridge cart;
//     cart.load((uint8 *)sysex_blob.getBinaryData()->getData(), 4104);
//     loadCartridge(cart);
//     // TODO: load program data into 'data'
//     //memcpy(activeProgram.getUnpackedData(), program.getBinaryData()->getData(), Program::PROGRAM_SIZE);
//
//     mappedMidiCC.clear();
//     XmlElement *midiCC = root->getChildByName("midiCC");
//     // if ( midiCC != nullptr ) {
//     //     XmlElement *ccMapping = midiCC->getFirstChildElement();
//     //     while (ccMapping != nullptr) {
//     //         int cc = ccMapping->getIntAttribute("cc", -1);
//     //         String target = ccMapping->getStringAttribute("target", "");
//     //         if ( target.isNotEmpty() && cc != -1 ) {
//     //             for(int i=0;i<ctrl.size();i++) {
//     //                 if ((cc >> 8) == 0) {
//     //                     // Simple migration logic lets old mappings without channel
//     //                     // work on channel 1.
//     //                     cc |= 1 << 8;
//     //                 }
//     //                 if ( ctrl[i]->label == target) {
//     //                     TRACE("mapping CC=%d to %s", cc, target.toRawUTF8());
//     //                     mappedMidiCC.set(cc, ctrl[i]);
//     //                     break;
//     //                 }
//     //             }
//     //         }
//     //         ccMapping = ccMapping->getNextElement();
//     //     }
//     // }
//     TRACE("setting VST STATE");
//     panic();
//     updateUI();
// }
