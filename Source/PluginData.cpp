/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

#include <time.h>

#include "PluginParam.h"
#include "PluginProcessor.h"
#include "PluginData.h"


uint8_t sysexChecksum(const char *sysex, int size) {
    int sum = 0;
    int i;
    
    for (i = 0; i < 4096; sum -= sysex[i++]);
    return sum & 0x7F;
}

void extractProgramNames(const char *block, StringArray &dest) {
    char programName[11];
    
    dest.clear();
    
    for (int i = 0; i < 32; i++) {
        memcpy(programName, block + ((i * 128) + 118), 11);
        
        for (int j = 0; j < 10; j++) {
            char c = (unsigned char) programName[j];
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
            programName[j] = c;
        }
        programName[10] = 0;
        
        dest.add(String(programName));
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

void DexedAudioProcessor::unpackProgram(int idx) {
    char *bulk = sysex + (idx * 128);
    
    for (int op = 0; op < 6; op++) {
        // eg rate and level, brk pt, depth, scaling

        for(int i=0; i<11; i++) {
            data[op * 21 + i] = normparm(bulk[op * 17 + i], 99, i);
        }

        memcpy(data + op * 21, bulk + op * 17, 11);
        char leftrightcurves = bulk[op * 17 + 11];
        data[op * 21 + 11] = leftrightcurves & 3;
        data[op * 21 + 12] = (leftrightcurves >> 2) & 3;
        char detune_rs = bulk[op * 17 + 12];
        data[op * 21 + 13] = detune_rs & 7;
        char kvs_ams = bulk[op * 17 + 13];
        data[op * 21 + 14] = kvs_ams & 3;
        data[op * 21 + 15] = kvs_ams >> 2;
        data[op * 21 + 16] = bulk[op * 17 + 14];  // output level
        char fcoarse_mode = bulk[op * 17 + 15];
        data[op * 21 + 17] = fcoarse_mode & 1;
        data[op * 21 + 18] = fcoarse_mode >> 1;
        data[op * 21 + 19] = bulk[op * 17 + 16];  // fine freq
        data[op * 21 + 20] = detune_rs >> 3;
    }

    for (int i=0; i<8; i++)  {
        data[126+i] = normparm(bulk[102+i], 99, 126+i);
    }
    data[134] = normparm(bulk[110], 31, 134);
    
    char oks_fb = bulk[111];
    data[135] = oks_fb & 7;
    data[136] = oks_fb >> 3;
    memcpy(data + 137, bulk + 112, 4);  // lfo
    char lpms_lfw_lks = bulk[116];
    data[141] = lpms_lfw_lks & 1;
    data[142] = (lpms_lfw_lks >> 1) & 7;
    data[143] = lpms_lfw_lks >> 4;
    memcpy(data + 144, bulk + 117, 11);  // transpose, name
    data[155] = 1;  // operator on/off
    data[156] = 1;
    data[157] = 1;
    data[158] = 1;
    data[159] = 1;
    data[160] = 1;
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
    memcpy(data, rawdata, 160);
    triggerAsyncUpdate();
}

void DexedAudioProcessor::loadBuiltin(int idx) {
    char syx_data[4104];
    memset(&syx_data, 0, 4104);
    cartManager.getSysex(idx, (char *) &syx_data);
    importSysex((char *) &syx_data);
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

CartridgeManager::CartridgeManager() {
    MemoryInputStream *mis = new MemoryInputStream(BinaryData::builtin_pgm_zip, BinaryData::builtin_pgm_zipSize, false);
    builtin_pgm = new ZipFile(mis, true);
    builtin_pgm->sortEntriesByFilename();

    for(int i=0;i<builtin_pgm->getNumEntries();i++) {
        const ZipFile::ZipEntry *e = builtin_pgm->getEntry(i);
        cartNames.add(e->filename.dropLastCharacters(4));
    }
}

void CartridgeManager::getSysex(int idx, char *dest) {
    InputStream *is = builtin_pgm->createStreamForEntry(idx);

    if ( is == NULL ) {
        TRACE("ENTRY IN ZIP NOT FOUND");
        return;
    }

    is->read(dest, 4104);
    delete is;
}

