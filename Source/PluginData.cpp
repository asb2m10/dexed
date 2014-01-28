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


uint8_t sysexChecksum(const char *sysex) {
    uint8_t sum = 0;
    for (int i=0; i<4096; i++)
        sum = (sum + sysex[i]) % (1 << 8);
    return ((1 << 8) - sum);
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

int DexedAudioProcessor::importSysex(const char *imported) {
    memcpy(sysex, imported + 6, 4096);
    
    uint8_t checksum = sysexChecksum(sysex);
    extractProgramNames(sysex, programNames);
    
    if ( checksum != imported[4102] ) {
        TRACE("sysex import checksum doesnt match");
        return 1;
    }
    
    return 0;
}

void DexedAudioProcessor::exportSysex(char *dest) {
    uint8_t header[] = { 0xF0, 0x43, 0x00, 0x09, 0x20, 0x00 };
    memcpy(dest, header, 6);
    
    // copy 32 voices
    memcpy(dest+6, sysex, 4096);
    
    // make checksum for dump
    uint8_t footer[] = { sysexChecksum(sysex), 0xF7 };
    
    memcpy(dest+4102, footer, 2);
}

/**
 * This function normalize data that comes from corrupted sysex data.
 * It used to avoid engine crashing upon extrem values
 */
char normparm(char value, char max) {
    if ( value <= max )
        return value;
    
    // if this is beyond the max, we expect a 0-255 range, normalize this
    // to the expected return value; and this value as a random data.
    
    return ((float)value)/255 * max;
}

void DexedAudioProcessor::unpackProgram(int idx) {
    char *bulk = sysex + (idx * 128);
    
    for (int op = 0; op < 6; op++) {
        // eg rate and level, brk pt, depth, scaling
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
    memcpy(data + 126, bulk + 102, 9);  // pitch env, algo
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

void DexedAudioProcessor::packProgram(int idx, const char *name) {
    char *bulk = sysex + (idx * 128);
    
    for(int op = 0; op < 6; op++) {
        // eg rate and level, brk pt, depth, scaling
        memcpy(bulk + op * 17, data + op * 21, 11);
        int pp = op*17;
        int up = op*21;
        
        // left curves
        bulk[pp+11] = (data[up+11]&0x03) | ((data[up+12]&0x03) << 2);
        bulk[pp+12] = (data[up+13]&0x07) | ((data[up+20]&0x0f) << 3);
        // kvs_ams
        bulk[pp+13] = (data[up+14]&0x03) | ((data[up+15]&0x07) << 2);
        // output lvl
        bulk[pp+14] = data[up+16];
        // fcoarse_mode
        bulk[pp+15] = (data[up+17]&0x01) | ((data[up+18]&0x1f) << 1);
        // fine freq
        bulk[pp+16] = data[up+19];
    }
    memcpy(bulk + 102, data + 126, 9);      // pitch env, algo
    bulk[111] = (data[135]&0x07) | ((data[136]&0x01) << 3);
    memcpy(bulk + 112, data + 137, 4);      // lfo
    bulk[116] = (data[141]&0x01) | (((data[142]&0x07) << 1) | ((data[143]&0x07) << 4));
    bulk[117] = data[144];
    int eos = 0;
    for(int i=0; i < 10; i++) {
        char c = name[i];
        if ( c == 0 )
            eos = 1;
        if ( eos ) {
            bulk[117+i] = ' ';
            continue;
        }
        c = c < 32 ? ' ' : c;
        c = c > 127 ? ' ' : c;
        bulk[117+i] = c;
    }
    
    char programName[11];
    memcpy(programName, bulk+117, 10);
    programName[10] = 0;
    programNames.set(idx, String(programName));
}

void DexedAudioProcessor::updateProgramFromSysex(const uint8 *rawdata) {
    memcpy(data, rawdata, 160);
    triggerAsyncUpdate();
}

void DexedAudioProcessor::loadBuiltin(int idx) {
    InputStream *is = builtin_pgm->createStreamForEntry(idx);
    
    if ( is == NULL ) {
        TRACE("ENTRY IN ZIP NOT FOUND");
        return;
    }
    
    uint8_t syx_data[4104];
    is->read(&syx_data, 4104);
    delete is;
    
    importSysex((char *) &syx_data);
}

#define CURRENT_PLUGINSTATE_VERSION 2
struct PluginState {
    int version;
    uint8_t sysex[4104];
    uint8_t program[161];
    float cutoff;
    float reso;
    int programNum;
};

//==============================================================================
void DexedAudioProcessor::getStateInformation(MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // used to SAVE plugin state
    
    PluginState state;
    
    state.version = CURRENT_PLUGINSTATE_VERSION;
    
    exportSysex((char *)(&state.sysex));
    memcpy(state.program, data, 161);
    state.cutoff = fx.uiCutoff;
    state.reso = fx.uiReso;
    state.programNum = currentProgram;
    
    destData.insert(&state, sizeof(PluginState), 0);
}

void DexedAudioProcessor::setStateInformation(const void* source, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // used to LOAD plugin state
    
    PluginState state;
    
    if ( sizeInBytes < sizeof(PluginState) ) {
        TRACE("too small plugin state size %d", sizeInBytes);
        return;
    }
    
    if ( sizeInBytes > sizeof(PluginState) ) {
        TRACE("too big plugin state size %d", sizeInBytes);
        sizeInBytes = sizeof(PluginState);
    }
    
    memcpy((void *) &state, source, sizeInBytes);
    
    if ( state.version != CURRENT_PLUGINSTATE_VERSION ) {
        TRACE("version of VST chunk is not compatible, bailing out");
        return;
    }
    
    importSysex((char *) state.sysex);
    memcpy(data, state.program, 161);
    
    fx.uiCutoff = state.cutoff;
    fx.uiReso = state.reso;
    currentProgram = state.programNum;
    
    lastStateSave = (long) time(NULL);
    TRACE("setting VST STATE");
    updateUI();
}

//==============================================================================
/*void DexedAudioProcessor::getCurrentProgramStateInformation(
 MemoryBlock& destData) {
 destData.insert(data, 161, 0);
 }
 
 void DexedAudioProcessor::setCurrentProgramStateInformation(const void* source,
 int sizeInBytes) {
 memcpy((void *) data, source, sizeInBytes);
 updateUI();
 }*/
