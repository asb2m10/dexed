/**
 *
 * Copyright (c) 2014-2017 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

#include <time.h>

#include "PluginParam.h"
#include "PluginProcessor.h"
#include "PluginData.h"
#include "Dexed.h"

#include <fstream>
using namespace ::std;

uint8_t sysexChecksum(const uint8_t *sysex, int size) {
    int sum = 0;
    int i;
    
    for (i = 0; i < size; sum -= sysex[i++]);
    return sum & 0x7F;
}

void exportSysexPgm(uint8_t *dest, uint8_t *src) {
    uint8_t header[] = { 0xF0, 0x43, 0x00, 0x00, 0x01, 0x1B };
   
    memcpy(dest, header, 6);
    
    // copy 1 unpacked voices
    memcpy(dest+6, src, 155);
        
    // make checksum for dump
    uint8_t footer[] = { sysexChecksum(src, 155), 0xF7 };
    
    memcpy(dest+161, footer, 2);
}

/**
 * Pack a program into a 32 packed sysex
 */
void Cartridge::packProgram(uint8_t *src, int idx, String name, char *opSwitch) {
    uint8_t *bulk = voiceData + 6 + (idx * 128);
    
    for(int op = 0; op < 6; op++) {
        // eg rate and level, brk pt, depth, scaling
        memcpy(bulk + op * 17, src + op * 21, 11);
        int pp = op*17;
        int up = op*21;
        
        // left curves
        bulk[pp+11] = (src[up+11]&0x03) | ((src[up+12]&0x03) << 2);
        bulk[pp+12] = (src[up+13]&0x07) | ((src[up+20]&0x0f) << 3);
        // kvs_ams
        bulk[pp+13] = (src[up+14]&0x03) | ((src[up+15]&0x07) << 2);
        // output lvl
        if ( opSwitch[op] == '0' )
            bulk[pp+14] = 0;
        else
            bulk[pp+14] = src[up+16];
        // fcoarse_mode
        bulk[pp+15] = (src[up+17]&0x01) | ((src[up+18]&0x1f) << 1);
        // fine freq
        bulk[pp+16] = src[up+19];
    }
    memcpy(bulk + 102, src + 126, 9);      // pitch env, algo
    bulk[111] = (src[135]&0x07) | ((src[136]&0x01) << 3);
    memcpy(bulk + 112, src + 137, 4);      // lfo
    bulk[116] = (src[141]&0x01) | (((src[142]&0x07) << 1) | ((src[143]&0x07) << 4));
    bulk[117] = src[144];
        
    int eos = 0;

    for(int i=0; i < 10; i++) {
        char c = (char) name[i];
        if ( c == 0 )
            eos = 1;
        if ( eos ) {
            bulk[118+i] = ' ';
            continue;
        }
        c = c < 32 ? ' ' : c;
        c = c > 127 ? ' ' : c;
        bulk[118+i] = c;
    }
}

/**
 * This function normalize data that comes from corrupted sysex.
 * It used to avoid engine crashing upon extreme values
 */
char normparm(char value, char max, int id) {
    if ( value <= max && value >= 0 )
        return value;
    
    // if this is beyond the max, we expect a 0-255 range, normalize this
    // to the expected return value; and this value as a random data.
    
    value = abs(value);
    
    char v = ((float)value)/255 * max;

    return v;
}

void Cartridge::unpackProgram(uint8_t *unpackPgm, int idx) {
    // TODO put this in uint8_t :D
    char *bulk = (char *)voiceData + 6 + (idx * 128);
    
    for (int op = 0; op < 6; op++) {
        // eg rate and level, brk pt, depth, scaling
        
        for(int i=0; i<11; i++) {
            unpackPgm[op * 21 + i] = normparm(bulk[op * 17 + i], 99, i);
        }
        
        memcpy(unpackPgm + op * 21, bulk + op * 17, 11);
        char leftrightcurves = bulk[op * 17 + 11];
        unpackPgm[op * 21 + 11] = leftrightcurves & 3;
        unpackPgm[op * 21 + 12] = (leftrightcurves >> 2) & 3;
        char detune_rs = bulk[op * 17 + 12];
        unpackPgm[op * 21 + 13] = detune_rs & 7;
        char kvs_ams = bulk[op * 17 + 13];
        unpackPgm[op * 21 + 14] = kvs_ams & 3;
        unpackPgm[op * 21 + 15] = kvs_ams >> 2;
        unpackPgm[op * 21 + 16] = bulk[op * 17 + 14];  // output level
        char fcoarse_mode = bulk[op * 17 + 15];
        unpackPgm[op * 21 + 17] = fcoarse_mode & 1;
        unpackPgm[op * 21 + 18] = fcoarse_mode >> 1;
        unpackPgm[op * 21 + 19] = bulk[op * 17 + 16];  // fine freq
        unpackPgm[op * 21 + 20] = detune_rs >> 3;
    }
    
    for (int i=0; i<8; i++)  {
        unpackPgm[126+i] = normparm(bulk[102+i], 99, 126+i);
    }
    unpackPgm[134] = normparm(bulk[110], 31, 134);
    
    char oks_fb = bulk[111];
    unpackPgm[135] = oks_fb & 7;
    unpackPgm[136] = oks_fb >> 3;
    memcpy(unpackPgm + 137, bulk + 112, 4);  // lfo
    char lpms_lfw_lks = bulk[116];
    unpackPgm[141] = lpms_lfw_lks & 1;
    unpackPgm[142] = (lpms_lfw_lks >> 1) & 7;
    unpackPgm[143] = lpms_lfw_lks >> 4;
    memcpy(unpackPgm + 144, bulk + 117, 11);  // transpose, name
}

void DexedAudioProcessor::loadCartridge(Cartridge &sysex) {
    currentCart = sysex;
    currentCart.getProgramNames(programNames);
}

void DexedAudioProcessor::packOpSwitch() {
    char value = (controllers.opSwitch[5] == '1') << 5;
    value += (controllers.opSwitch[4] == '1') << 4;
    value += (controllers.opSwitch[3] == '1') << 3;
    value += (controllers.opSwitch[2] == '1') << 2;
    value += (controllers.opSwitch[1] == '1') << 1;
    value += (controllers.opSwitch[0] == '1');
    data[155] = value;
}

void DexedAudioProcessor::unpackOpSwitch(char packOpValue) {
    controllers.opSwitch[5] = ((packOpValue >> 5) &1) + 48;
    controllers.opSwitch[4] = ((packOpValue >> 4) &1) + 48;
    controllers.opSwitch[3] = ((packOpValue >> 3) &1) + 48;
    controllers.opSwitch[2] = ((packOpValue >> 2) &1) + 48;
    controllers.opSwitch[1] = ((packOpValue >> 1) &1) + 48;
    controllers.opSwitch[0] = (packOpValue &1) + 48;
}

int DexedAudioProcessor::updateProgramFromSysex(const uint8_t *rawdata) {
    memcpy(data, rawdata, 155);
    unpackOpSwitch(0x3F);
    lfo.reset(data + 137);
    triggerAsyncUpdate();
    if (sysexChecksum(rawdata, 155) != rawdata[155]) // rawdata[155] is a checksum in a sysex dump
        return 1; // just return 1 if the checksum doesn't match, might be normal if it is loaded from a cartridge
    return 0;
}

void DexedAudioProcessor::setupStartupCart() {
    File startup = dexedCartDir.getChildFile("Dexed_01.syx");

    if ( currentCart.load(startup) != -1 ) {
        loadCartridge(currentCart);
        return;
    }
    
    // The user deleted the file :/, load from the builtin zip file.
    MemoryInputStream *mis = new MemoryInputStream(BinaryData::builtin_pgm_zip, BinaryData::builtin_pgm_zipSize, false);
    ZipFile *builtin_pgm = new ZipFile(mis, true);
    InputStream *is = builtin_pgm->createStreamForEntry(builtin_pgm->getIndexOfFileName(("Dexed_01.syx")));
    Cartridge init;
    
    if ( init.load(*is) != -1 ) {
        loadCartridge(init);
    }

    delete is;
    delete builtin_pgm;
}

void DexedAudioProcessor::resetToInitVoice() {
    const char init_voice[] =
      { 99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 0, 0, 0, 0, 0, 0, 0, 0, 99, 0, 1, 0, 7,
        99, 99, 99, 99, 50, 50, 50, 50, 0, 0, 1, 35, 0, 0, 0, 1, 0, 3, 24,
        73, 78, 73, 84, 32, 86, 79, 73, 67, 69 };
    
    for(int i=0;i<sizeof(init_voice);i++) {
        data[i] = init_voice[i];
    }
    unpackOpSwitch(0x3F);
    panic();
    triggerAsyncUpdate();
}

void DexedAudioProcessor::copyToClipboard(int srcOp) {
    memcpy(clipboard, data, 161);
    clipboardContent = srcOp;
}

void DexedAudioProcessor::pasteOpFromClipboard(int destOp) {
    memcpy(data+(destOp*21), clipboard+(clipboardContent*21), 21);
    triggerAsyncUpdate();
}

void DexedAudioProcessor::pasteEnvFromClipboard(int destOp) {
    memcpy(data+(destOp*21), clipboard+(clipboardContent*21), 8);
    triggerAsyncUpdate();
}

void DexedAudioProcessor::sendCurrentSysexProgram() {
    uint8_t raw[163];
    
    packOpSwitch();
    exportSysexPgm(raw, data);
    raw[2] = raw[2] | sysexComm.getChl();
    if ( sysexComm.isOutputActive() ) {
        sysexComm.send(MidiMessage(raw, 163));
    }
}

void DexedAudioProcessor::sendCurrentSysexCartridge() {
    uint8_t raw[4104];
    
    currentCart.saveVoice(raw);
    raw[2] = raw[2] | sysexComm.getChl();
    if ( sysexComm.isOutputActive() ) {
        sysexComm.send(MidiMessage(raw, 4104));
    }
}

void DexedAudioProcessor::sendSysexCartridge(File cart) {
    if ( ! sysexComm.isOutputActive() )
        return;
    
    std::unique_ptr<juce::FileInputStream> fis = cart.createInputStream();
    if ( fis == NULL ) {
        String f = cart.getFullPathName();
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Error",
                                          "Unable to open: " + f);
    }
    
    uint8 syx_data[65535];
    int sz = fis->read(syx_data, 65535);
    
    if (syx_data[0] != 0xF0) {
        String f = cart.getFullPathName();
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Error",
                                          "File: " + f + " doesn't seems to contain any sysex data");
        return;
    }
    sysexComm.send(MidiMessage(syx_data, sz));
}


bool DexedAudioProcessor::hasClipboardContent() {
    return clipboardContent != -1;
}

//==============================================================================
void DexedAudioProcessor::getStateInformation(MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // used to SAVE plugin state
    
    XmlElement dexedState("dexedState");
    XmlElement *dexedBlob = dexedState.createNewChildElement("dexedBlob");
    
    dexedState.setAttribute("cutoff", fx.uiCutoff);
    dexedState.setAttribute("reso", fx.uiReso);
    dexedState.setAttribute("gain", fx.uiGain);
    dexedState.setAttribute("currentProgram", currentProgram);
    dexedState.setAttribute("monoMode", monoMode);
    dexedState.setAttribute("engineType", (int) engineType);
    dexedState.setAttribute("masterTune", controllers.masterTune);
    //TRACE("saving opswitch %s", controllers.opSwitch);
    dexedState.setAttribute("opSwitch", controllers.opSwitch);
    dexedState.setAttribute("transpose12AsScale", controllers.transpose12AsScale ? 1 : 0 );
    dexedState.setAttribute("mpeEnabled", controllers.mpeEnabled ? 1 : 0 );
    dexedState.setAttribute("mpePitchBendRange", controllers.mpePitchBendRange );
    
    char mod_cfg[15];
    controllers.wheel.setConfig(mod_cfg);
    dexedState.setAttribute("wheelMod", mod_cfg);
    controllers.foot.setConfig(mod_cfg);
    dexedState.setAttribute("footMod", mod_cfg);
    controllers.breath.setConfig(mod_cfg);
    dexedState.setAttribute("breathMod", mod_cfg);
    controllers.at.setConfig(mod_cfg);
    dexedState.setAttribute("aftertouchMod", mod_cfg);

    if( currentSCLData.size() > 1 || currentKBMData.size() > 1 )
    {
        auto tuningx = dexedState.createNewChildElement("dexedTuning" );
        auto sclx = tuningx->createNewChildElement("scl");
        sclx->addTextElement(currentSCLData);
        auto kbmx = tuningx->createNewChildElement("kbm");
        kbmx->addTextElement(currentKBMData);
    }
    
    if ( activeFileCartridge.exists() )
        dexedState.setAttribute("activeFileCartridge", activeFileCartridge.getFullPathName());

    NamedValueSet blobSet;
    blobSet.set("sysex", var((void *) currentCart.getVoiceSysex(), 4104));
    blobSet.set("program", var((void *) &data, 161));
    
    blobSet.copyToXmlAttributes(*dexedBlob);
    
    XmlElement *midiCC = dexedState.createNewChildElement("midiCC");
    HashMap<int, Ctrl *>::Iterator i(mappedMidiCC);
    while(i.next()) {
        XmlElement *ccMapping = midiCC->createNewChildElement("mapping");
        ccMapping->setAttribute("cc", i.getKey());
        Ctrl *ctrl = i.getValue();
        ccMapping->setAttribute("target", ctrl->label);
    }
    
    copyXmlToBinary(dexedState, destData);
}

void DexedAudioProcessor::setStateInformation(const void* source, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // used to LOAD plugin state
    std::unique_ptr<XmlElement> root(getXmlFromBinary(source, sizeInBytes));

    if (root == nullptr) {
        TRACE("unknown state format");
        return;
    }
    
    fx.uiCutoff = root->getDoubleAttribute("cutoff");
    fx.uiReso = root->getDoubleAttribute("reso");
    fx.uiGain = root->getDoubleAttribute("gain");
    currentProgram = root->getIntAttribute("currentProgram");
    
    String opSwitchValue = root->getStringAttribute("opSwitch");
    //TRACE("opSwitch value %s", opSwitchValue.toRawUTF8());
    if ( opSwitchValue.length() != 6 ) {
        strcpy(controllers.opSwitch, "111111");
    } else {
        strncpy(controllers.opSwitch, opSwitchValue.toRawUTF8(), 6);
    }
    
    controllers.wheel.parseConfig(root->getStringAttribute("wheelMod").toRawUTF8());
    controllers.foot.parseConfig(root->getStringAttribute("footMod").toRawUTF8());
    controllers.breath.parseConfig(root->getStringAttribute("breathMod").toRawUTF8());
    controllers.at.parseConfig(root->getStringAttribute("aftertouchMod").toRawUTF8());
    
    controllers.refresh();
    
    setEngineType(root->getIntAttribute("engineType", 1));
    monoMode = root->getIntAttribute("monoMode", 0);
    controllers.masterTune = root->getIntAttribute("masterTune", 0);
    controllers.transpose12AsScale = ( root->getIntAttribute("transpose12AsScale", 1) != 0 );

    controllers.mpePitchBendRange = ( root->getIntAttribute("mpePitchBendRange", 24) );
    controllers.mpeEnabled = ( root->getIntAttribute("mpeEnabled", 0) != 0 );
    
    File possibleCartridge = File(root->getStringAttribute("activeFileCartridge"));
    if ( possibleCartridge.exists() )
        activeFileCartridge = possibleCartridge;

    auto tuningParent = root->getChildByName( "dexedTuning" );
    if( tuningParent )
    {
        auto sclx = tuningParent->getChildByName( "scl" );
        auto kbmx = tuningParent->getChildByName( "kbm" );
        std::string s = "";
        if( sclx && sclx->getFirstChildElement() && sclx->getFirstChildElement()->isTextElement() )
        {
            s = sclx->getFirstChildElement()->getText().toStdString();
            if( s.size() > 1 )
                applySCLTuning(s);
        }

        std::string k = "";
        if( kbmx && kbmx->getFirstChildElement() && kbmx->getFirstChildElement()->isTextElement() )
        {
            k = kbmx->getFirstChildElement()->getText().toStdString();
            if( k.size() > 1 )
                applyKBMMapping(k);
        }
    }
    
    XmlElement *dexedBlob = root->getChildByName("dexedBlob");
    if ( dexedBlob == NULL ) {
        TRACE("dexedBlob element not found");
        return;
    }
    
    NamedValueSet blobSet;
    blobSet.setFromXmlAttributes(*dexedBlob);
    
    var sysex_blob = blobSet["sysex"];
    var program = blobSet["program"];
    
    if ( sysex_blob.isVoid() || program.isVoid() ) {
        TRACE("unknown serialized blob data");
        return;
    }
    
    Cartridge cart;
    cart.load((uint8 *)sysex_blob.getBinaryData()->getData(), 4104);
    loadCartridge(cart);
    memcpy(data, program.getBinaryData()->getData(), 161);
    
    mappedMidiCC.clear();
    XmlElement *midiCC = root->getChildByName("midiCC");
    if ( midiCC != nullptr ) {
        XmlElement *ccMapping = midiCC->getFirstChildElement();
        while (ccMapping != nullptr) {
            int cc = ccMapping->getIntAttribute("cc", -1);
            String target = ccMapping->getStringAttribute("target", "");
            if ( target.isNotEmpty() && cc != -1 ) {
                for(int i=0;i<ctrl.size();i++) {
                    if ( ctrl[i]->label == target) {
                        TRACE("mapping CC=%d to %s", cc, target.toRawUTF8());
                        mappedMidiCC.set(cc, ctrl[i]);
                        break;
                    }
                }
            }
            ccMapping = ccMapping->getNextElement();
        }
    }
    
    lastStateSave = (long) time(NULL);    
    TRACE("setting VST STATE");
    updateUI();
}

File DexedAudioProcessor::dexedAppDir;
File DexedAudioProcessor::dexedCartDir;

void DexedAudioProcessor::resolvAppDir() {
    #if JUCE_MAC || JUCE_IOS
        File parent = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory().getParentDirectory().getParentDirectory().getSiblingFile("Dexed");
    
        if ( parent.isDirectory() ) {
            dexedAppDir = parent;
        } else {
            dexedAppDir = File("~/Library/Application Support/DigitalSuburban/Dexed");
        }
    #elif JUCE_WINDOWS
        if ( File::getSpecialLocation(File::currentExecutableFile).getSiblingFile("Dexed").isDirectory() ) {
            dexedAppDir = File::getSpecialLocation(File::currentExecutableFile).getSiblingFile("Dexed");
        } else {
            dexedAppDir = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("DigitalSuburban").getChildFile("Dexed");
        }
    #else
        if ( File::getSpecialLocation(File::currentExecutableFile).getSiblingFile("Dexed").isDirectory() ) {
            dexedAppDir = File::getSpecialLocation(File::currentExecutableFile).getSiblingFile("Dexed");
        } else {
            char *xdgHome = getenv("XDG_DATA_HOME");
            if ( xdgHome == nullptr ) {
                dexedAppDir = File("~/.local/share").getChildFile("DigitalSuburban").getChildFile("Dexed");
            } else {
                dexedAppDir = File(xdgHome).getChildFile("DigitalSuburban").getChildFile("Dexed");
            }
        }
    #endif
    
    if ( ! dexedAppDir.exists() ) {
        dexedAppDir.createDirectory();
        // ==========================================================================
        // For older versions, we move the Dexed.xml config file
        // This code will be removed in 0.9.0
        File cfgFile = dexedAppDir.getParentDirectory().getChildFile("Dexed.xml");
        if ( cfgFile.exists() )
            cfgFile.moveFileTo(dexedAppDir.getChildFile("Dexed.xml"));
        // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
        // ==========================================================================
    }
    
    dexedCartDir = dexedAppDir.getChildFile("Cartridges");

    if ( ! dexedCartDir.exists() ) {
        // Initial setup, we unzip the built-in cartridges
        dexedCartDir.createDirectory();
        File synprezFmDir = dexedCartDir.getChildFile("SynprezFM");
        synprezFmDir.createDirectory();
        
        MemoryInputStream *mis = new MemoryInputStream(BinaryData::builtin_pgm_zip, BinaryData::builtin_pgm_zipSize, false);
        ZipFile *builtin_pgm = new ZipFile(mis, true);
        
        for(int i=0;i<builtin_pgm->getNumEntries();i++) {
            if ( builtin_pgm->getEntry(i)->filename == "Dexed_01.syx" ) {
                builtin_pgm->uncompressEntry(i, dexedCartDir);
            } else {
                builtin_pgm->uncompressEntry(i, synprezFmDir);
            }
        }
        delete builtin_pgm;
    }
}
