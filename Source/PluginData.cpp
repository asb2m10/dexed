/**
 *
 * Copyright (c) 2014-2015 Pascal Gauthier.
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

uint8_t sysexChecksum(const char *sysex, int size) {
    int sum = 0;
    int i;
    
    for (i = 0; i < size; sum -= sysex[i++]);
    return sum & 0x7F;
}

String normalizeSysexName(const char *sysexName) {
    char buffer[11];

    memcpy(buffer, sysexName, 10);
    
    for (int j = 0; j < 10; j++) {
        char c = (unsigned char) buffer[j];
        switch (c) {
            case 92:
                c = 'Y';
                break; /* yen */
            case 126:
                c = '>';
                break; /* >> */
            case 127:
                c = '<';
                break; /* << */
            default:
                if (c < 32 || c > 127)
                    c = 32;
                break;
        }
        buffer[j] = c;
    }
    buffer[10] = 0;

    return String(buffer);
}


void extractProgramNames(const char *block, StringArray &dest) {
    dest.clear();
    
    for (int i = 0; i < 32; i++) {
        dest.add(String(normalizeSysexName(block + ((i * 128) + 118))));
    }
}

void exportSysexCart(char *dest, char *src, char sysexChl) {
    uint8_t header[] = { 0xF0, 0x43, 0x00, 0x09, 0x20, 0x00 };
    header[2] = sysexChl;
    
    memcpy(dest, header, 6);
    
    // copy 32 voices
    memcpy(dest+6, src, 4096);
    
    // make checksum for dump
    uint8_t footer[] = { sysexChecksum(src, 4096), 0xF7 };

    memcpy(dest+4102, footer, 2);
}


void exportSysexPgm(char *dest, char *src, char sysexChl) {
    uint8_t header[] = { 0xF0, 0x43, 0x00, 0x00, 0x01, 0x1B };
    header[2] = sysexChl;
    
    memcpy(dest, header, 6);
    
    // copy 1 unpacked voices
    memcpy(dest+6, src, 155);
    
    // put some logic to "mute" an operator if the level is 0
    
    // make checksum for dump
    uint8_t footer[] = { sysexChecksum(src, 155), 0xF7 };
    
    memcpy(dest+161, footer, 2);
}

/**
 * Pack a program into a 32 packed sysex
 */
void packProgram(uint8_t *dest, uint8_t *src, int idx, String name) {
    uint8_t *bulk = dest + (idx * 128);
    
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
 * It used to avoid engine crashing upon extrem values
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

void unpackProgramFromSysex(char *unpackPgm, char *sysexCart, int idx) {
    char *bulk = sysexCart + (idx * 128);
    
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
    unpackPgm[155] = 1;  // operator on/off
    unpackPgm[156] = 1;
    unpackPgm[157] = 1;
    unpackPgm[158] = 1;
    unpackPgm[159] = 1;
    unpackPgm[160] = 1;
}

void DexedAudioProcessor::unpackProgram(int idx) {
    unpackProgramFromSysex(data, sysex, idx);
}

int DexedAudioProcessor::importSysex(const char *imported) {
    memcpy(sysex, imported + 6, 4096);
    
    uint8_t checksum = sysexChecksum(((char *) &sysex), 4096);
    extractProgramNames(sysex, programNames);
    
    if ( checksum != imported[4102] ) {
        TRACE("sysex import checksum doesnt match %d != %d", checksum, imported[4102]);
        return 1;
    }
    
    return 0;
}

void DexedAudioProcessor::updateProgramFromSysex(const uint8 *rawdata) {
    memcpy(data, rawdata, 161);
    triggerAsyncUpdate();
}

void DexedAudioProcessor::setupStartupCart() {
    char syx_data[4104];
    memset(&syx_data, 0, 4104);
    
    File startup = dexedCartDir.getChildFile("Dexed_01.syx");
    
    if ( startup.exists() ) {
        ScopedPointer<FileInputStream> fis = startup.createInputStream();
        if ( fis == nullptr ) {
            TRACE("unable to open default cartridge");
            return;
        }
        fis->read(syx_data, 4104);
    } else {
        // The user deleted the file :/, load from the builtin zip file.
        MemoryInputStream *mis = new MemoryInputStream(BinaryData::builtin_pgm_zip, BinaryData::builtin_pgm_zipSize, false);
        ScopedPointer<ZipFile> builtin_pgm = new ZipFile(mis, true);
        ScopedPointer<InputStream> is = builtin_pgm->createStreamForEntry(builtin_pgm->getIndexOfFileName(("Dexed_01.syx")));
        is->read(syx_data, 4104);
    }
    importSysex((char *) &syx_data);
}

void DexedAudioProcessor::resetToInitVoice() {
    const char init_voice[] =
      { 99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 99, 0, 1, 0, 7,
        99, 99, 99, 99, 50, 50, 50, 50, 0, 0, 1, 35, 0, 0, 0, 1, 0, 3, 24,
        73, 78, 73, 84, 32, 86, 79, 73, 67, 69 };
    
    for(int i=0;i<sizeof(init_voice);i++) {
        data[i] = init_voice[i];
    }
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
    uint8_t raw[167];
    
    exportSysexPgm((char *) raw, data, sysexComm.getChl());
    if ( sysexComm.isOutputActive() ) {
        sysexComm.send(MidiMessage(raw, 163));
    }
}

void DexedAudioProcessor::sendCurrentSysexCartridge() {
    uint8_t raw[4104];
    
    exportSysexCart((char *) raw, (char *) &sysex, sysexComm.getChl());
    if ( sysexComm.isOutputActive() ) {
        sysexComm.send(MidiMessage(raw, 4104));
    }
}

void DexedAudioProcessor::sendSysexCartridge(File cart) {
    if ( ! sysexComm.isOutputActive() )
        return;
    String f = cart.getFullPathName();
    uint8_t syx_data[4104];
    ifstream fp_in(f.toRawUTF8(), ios::binary);
    if (fp_in.fail()) {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Error",
                                          "Unable to open: " + f);
        return;
    }
    fp_in.read((char *)syx_data, 4104);
    fp_in.close();
    sysexComm.send(MidiMessage(syx_data, 4104));
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
    
    if ( activeFileCartridge.exists() )
        dexedState.setAttribute("activeFileCartridge", activeFileCartridge.getFullPathName());

    char sysex_blob[4104];
    exportSysexCart((char *) &sysex_blob, (char *) sysex, 0);
    
    NamedValueSet blobSet;
    blobSet.set("sysex", var((void *) &sysex_blob, 4104));
    blobSet.set("program", var((void *) &data, 161));
    
    blobSet.copyToXmlAttributes(*dexedBlob);
    copyXmlToBinary(dexedState, destData);
}

void DexedAudioProcessor::setStateInformation(const void* source, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // used to LOAD plugin state
    ScopedPointer<XmlElement> root(getXmlFromBinary(source, sizeInBytes));
    
    if (root == nullptr) {
        TRACE("unkown state format");
        return;
    }
    
    fx.uiCutoff = root->getDoubleAttribute("cutoff");
    fx.uiReso = root->getDoubleAttribute("reso");
    fx.uiGain = root->getDoubleAttribute("gain");
    currentProgram = root->getIntAttribute("currentProgram");
    
    setEngineType(root->getIntAttribute("engineType", 0));
    monoMode = root->getIntAttribute("monoMode", 0);
    
    File possibleCartridge = File(root->getStringAttribute("activeFileCartridge"));
    if ( possibleCartridge.exists() )
        activeFileCartridge = possibleCartridge;
    
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
        TRACE("unkown serialized blob data");
        return;
    }
    
    importSysex((char *) sysex_blob.getBinaryData()->getData());
    memcpy(data, program.getBinaryData()->getData(), 161);
    
    lastStateSave = (long) time(NULL);
    TRACE("setting VST STATE");
    updateUI();
}

File DexedAudioProcessor::dexedAppDir;
File DexedAudioProcessor::dexedCartDir;

void DexedAudioProcessor::resolvAppDir() {
    
#if JUCE_MAC || JUCE_IOS
    dexedAppDir = File("~/Library/Application Support/DigitalSuburban/Dexed");
#elif JUCE_WINDOWS
    dexedAppDir = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("DigitalSuburban").getChildFile("Dexed");
#else
    char *xdgHome = getenv("XDG_DATA_HOME");
    if ( xdgHome == nullptr ) {
        dexedAppDir = File("~/.local/share").getChildFile("DigitalSuburban").getChildFile("Dexed");
    } else {
        dexedAppDir = File(xdgHome).getChildFile("DigitalSuburban").getChildFile("Dexed");
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
        ScopedPointer<ZipFile> builtin_pgm = new ZipFile(mis, true);
        
        for(int i=0;i<builtin_pgm->getNumEntries();i++) {
            if ( builtin_pgm->getEntry(i)->filename == "Dexed_01.syx" ) {
                builtin_pgm->uncompressEntry(i, dexedCartDir);
            } else {
                builtin_pgm->uncompressEntry(i, synprezFmDir);
            }
        }
    }
}
