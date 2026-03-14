/**
 *
 * Copyright (c) 2014-2025 Pascal Gauthier.
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
uint8_t normparm(uint8_t value, char max, int id) {
    if ( value <= max )
        return value;
    // if this is beyond the max, we expect a 0-255 range, normalize this
    // to the expected return value; and this value as a random data.
    return ((float)value)/255 * max;
}

void Cartridge::unpackProgram(uint8_t *unpackPgm, int idx) {
    // TODO put this in uint8_t :D
    char *bulk = (char *)voiceData + 6 + (idx * 128);
    
    for (int op = 0; op < 6; op++) {
        // eg rate and level, brk pt, depth, scaling
        
        for(int i=0; i<11; i++) {
            uint8_t currparm = bulk[op * 17 + i] & 0x7F; // mask BIT7 (don't care per sysex spec) 
            unpackPgm[op * 21 + i] = normparm(currparm, 99, i);
        }
        
        // Note: the memcpy that was here has been removed — it was overwriting
        // the normalized values computed by the loop above with raw unnormalized data.
        char leftrightcurves = bulk[op * 17 + 11]&0xF; // bits 4-7 don't care per sysex spec
        unpackPgm[op * 21 + 11] = leftrightcurves & 3;
        unpackPgm[op * 21 + 12] = (leftrightcurves >> 2) & 3;
        char detune_rs = bulk[op * 17 + 12]&0x7F;
        unpackPgm[op * 21 + 13] = detune_rs & 7;
        char kvs_ams = bulk[op * 17 + 13]&0x1F; // bits 5-7 don't care per sysex spec
        unpackPgm[op * 21 + 14] = kvs_ams & 3;
        unpackPgm[op * 21 + 15] = (kvs_ams >> 2) & 7;
        unpackPgm[op * 21 + 16] = bulk[op * 17 + 14]&0x7F;  // output level
        char fcoarse_mode = bulk[op * 17 + 15]&0x3F; //bits 6-7 don't care per sysex spec
        unpackPgm[op * 21 + 17] = fcoarse_mode & 1;
        unpackPgm[op * 21 + 18] = (fcoarse_mode >> 1)&0x1F;
        unpackPgm[op * 21 + 19] = bulk[op * 17 + 16]&0x7F;  // fine freq
        unpackPgm[op * 21 + 20] = (detune_rs >> 3) &0x7F;
    }
    
    for (int i=0; i<8; i++)  {
        uint8_t currparm = bulk[102 + i] & 0x7F; // mask BIT7 (don't care per sysex spec)
        unpackPgm[126+i] = normparm(currparm, 99, 126+i);
    }
    unpackPgm[134] = normparm(bulk[110]&0x1F, 31, 134); // bits 5-7 are don't care per sysex spec
    
    char oks_fb = bulk[111]&0xF;//bits 4-7 are don't care per spec
    unpackPgm[135] = oks_fb & 7;
    unpackPgm[136] = oks_fb >> 3;
    unpackPgm[137] = bulk[112] & 0x7F; // lfs
    unpackPgm[138] = bulk[113] & 0x7F; // lfd
    unpackPgm[139] = bulk[114] & 0x7F; // lpmd
    unpackPgm[140] = bulk[115] & 0x7F; // lamd
    char lpms_lfw_lks = bulk[116] & 0x7F;
    unpackPgm[141] = lpms_lfw_lks & 1;
    unpackPgm[142] = (lpms_lfw_lks >> 1) & 7;
    unpackPgm[143] = lpms_lfw_lks >> 4;
    unpackPgm[144] = bulk[117] & 0x7F;
    for (int name_idx = 0; name_idx < 10; name_idx++) {
        unpackPgm[145 + name_idx] = bulk[118 + name_idx] & 0x7F;
    } //name_idx
//    memcpy(unpackPgm + 144, bulk + 117, 11);  // transpose, name
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
    panic();
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

    if ( is != nullptr && init.load(*is) != -1 ) {
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

void DexedAudioProcessor::randomizeVoice() {
    auto& rng = juce::Random::getSystemRandom();

    // Musically useful coarse frequency ratios (harmonics and common sub-ratios)
    // These produce tonal results rather than dissonant noise
    const int usefulCoarse[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 16 };
    const int numUsefulCoarse = 12;

    // Per-operator parameters (6 operators, 21 bytes each)
    for (int op = 0; op < 6; op++) {
        int off = op * 21;

        // EG rates 1-4 (0-99) — slower overall for more pad-like sounds
        data[off + 0] = 20 + rng.nextInt(60);       // R1 (attack): varied, some slow
        data[off + 1] = 10 + rng.nextInt(35);        // R2 (decay): slow-moderate
        data[off + 2] = 5 + rng.nextInt(25);         // R3 (sustain): slow
        data[off + 3] = 10 + rng.nextInt(40);        // R4 (release): slow-moderate

        // EG levels 1-4 (0-99) — high sustain for pad-like tails
        data[off + 4] = 90 + rng.nextInt(10);        // L1: near max (attack peak)
        data[off + 5] = 70 + rng.nextInt(29);        // L2: high
        data[off + 6] = 60 + rng.nextInt(35);        // L3: sustain level — high for pads
        data[off + 7] = 0;                            // L4: zero (silence at release end)

        // Keyboard level scaling break point (0-99) — center around middle C
        data[off + 8] = 27 + rng.nextInt(30);
        // Left/right depth (0-99) — keep subtle
        data[off + 9] = rng.nextInt(40);
        data[off + 10] = rng.nextInt(40);
        // Left/right curve (0-3)
        data[off + 11] = rng.nextInt(4);
        data[off + 12] = rng.nextInt(4);
        // Keyboard rate scaling (0-7) — keep low
        data[off + 13] = rng.nextInt(4);
        // Amp mod sensitivity (0-3)
        data[off + 14] = rng.nextInt(4);
        // Key velocity sensitivity (0-7)
        data[off + 15] = 2 + rng.nextInt(5);
        // Output level (0-99) — bias toward audible
        data[off + 16] = 60 + rng.nextInt(40);
        // Osc mode: almost always ratio (0), rarely fixed (1)
        data[off + 17] = rng.nextInt(20) < 1 ? 1 : 0;
        // Freq coarse — use musically useful ratios
        data[off + 18] = usefulCoarse[rng.nextInt(numUsefulCoarse)];
        // Freq fine (0-99) — keep low for more tonal results
        data[off + 19] = rng.nextInt(30);
        // Detune (0-14, center=7) — subtle detune around center
        data[off + 20] = 5 + rng.nextInt(5);
    }

    // Ensure carriers have good output levels and modulators are varied
    // Op6 (index 5) is a carrier in most algorithms
    data[5 * 21 + 16] = 80 + rng.nextInt(20);
    // Some modulators can be quieter for subtlety
    for (int op = 0; op < 4; op++) {
        if (rng.nextInt(3) == 0)
            data[op * 21 + 16] = 30 + rng.nextInt(50);
    }

    // Pitch EG rates (0-99) — fast so pitch settles quickly
    for (int i = 126; i < 130; i++)
        data[i] = 70 + rng.nextInt(25);
    // Pitch EG levels (0-99) — flat at 50 = no pitch shift
    for (int i = 130; i < 134; i++)
        data[i] = 50;

    // Algorithm (0-31)
    data[134] = rng.nextInt(32);
    // Feedback (0-7) — keep moderate to avoid harsh distortion
    data[135] = rng.nextInt(5);
    // Oscillator sync (0-1)
    data[136] = rng.nextInt(2);
    // LFO speed (0-99) — slow for gentle movement
    data[137] = rng.nextInt(35);
    // LFO delay (0-99) — some delay before LFO kicks in
    data[138] = 20 + rng.nextInt(60);
    // LFO pitch mod depth (0-99) — very subtle, no heavy vibrato
    data[139] = rng.nextInt(10);
    // LFO amp mod depth (0-99) — subtle tremolo
    data[140] = rng.nextInt(20);
    // LFO sync (0-1)
    data[141] = rng.nextInt(2);
    // LFO waveform (0-5) — favor sine(4) and triangle(0) for smooth movement
    data[142] = rng.nextInt(3) == 0 ? rng.nextInt(6) : (rng.nextInt(2) == 0 ? 0 : 4);
    // Pitch mod sensitivity (0-7) — very low to minimize vibrato
    data[143] = rng.nextInt(2);
    // Transpose (0-48, center=24 is C3)
    data[144] = 24;

    // Voice name: "RND-" + 6 random chars
    const char* nameChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    data[145] = 'R'; data[146] = 'N'; data[147] = 'D'; data[148] = '-';
    for (int i = 0; i < 6; i++)
        data[149 + i] = nameChars[rng.nextInt(36)];

    unpackOpSwitch(0x3F);
    panic();
    lfo.reset(data + 137);
    triggerAsyncUpdate();
}

void DexedAudioProcessor::copyToClipboard(int srcOp) {
    DexedClipboard clipboard(data + (srcOp *21), 21);
    clipboard.write(String("Program: '") + getProgramName(getCurrentProgram()) + "' operator: " + String(6-srcOp));
}

void DexedAudioProcessor::pasteOpFromClipboard(int destOp) {
    DexedClipboard clipboard;

    jassert(clipboard.isOperatorData());

    memcpy(data+(destOp*21), clipboard.getRawData(), 21);
    triggerAsyncUpdate();
}

void DexedAudioProcessor::pasteEnvFromClipboard(int destOp) {
    DexedClipboard clipboard;

    jassert(clipboard.isOperatorData());

    memcpy(data+(destOp*21), clipboard.getRawData(), 8);
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
        return;
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
    dexedState.setAttribute("engineType", (int) engineType);
    dexedState.setAttribute("masterTune", controllers.masterTune);
    //TRACE("saving opswitch %s", controllers.opSwitch);
    dexedState.setAttribute("opSwitch", controllers.opSwitch);
    dexedState.setAttribute("transpose12AsScale", controllers.transpose12AsScale ? 1 : 0 );
    dexedState.setAttribute("mpeEnabled", controllers.mpeEnabled ? 1 : 0 );
    dexedState.setAttribute("mpePitchBendRange", controllers.mpePitchBendRange );
    dexedState.setAttribute("monoMode", monoMode ? 1 : 0);
    dexedState.setAttribute("portamento", controllers.portamento_cc);
    dexedState.setAttribute("glissando", controllers.portamento_gliss_cc ? 1 : 0);

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
        controllers.opSwitch[6] = '\0';
    }
    
    controllers.wheel.parseConfig(root->getStringAttribute("wheelMod").toRawUTF8());
    controllers.foot.parseConfig(root->getStringAttribute("footMod").toRawUTF8());
    controllers.breath.parseConfig(root->getStringAttribute("breathMod").toRawUTF8());
    controllers.at.parseConfig(root->getStringAttribute("aftertouchMod").toRawUTF8());

    setEngineType(root->getIntAttribute("engineType", 1));
    monoMode = root->getIntAttribute("monoMode", 0);
    controllers.masterTune = root->getIntAttribute("masterTune", 0);
    controllers.transpose12AsScale = ( root->getIntAttribute("transpose12AsScale", 1) != 0 );

    controllers.mpePitchBendRange = ( root->getIntAttribute("mpePitchBendRange", 24) );
    controllers.mpeEnabled = ( root->getIntAttribute("mpeEnabled", 0) != 0 );

    controllers.portamento_cc = juce::jlimit(0, 127, root->getIntAttribute("portamento", 0));
    controllers.portamento_enable_cc = controllers.portamento_cc > 1;
    controllers.portamento_gliss_cc = ( root->getIntAttribute("glissando", 0) );
    controllers.refresh();

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
                    if ((cc >> 8) == 0) {
                        // Simple migration logic lets old mappings without channel
                        // work on channel 1.
                        cc |= 1 << 8;
                    }
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
    panic();
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

// ============================================================================
// 4-operator sysex import (DX21/DX27/DX100/TX81Z)
// ============================================================================

// DX100 frequency ratio lookup table (64 entries, index 0-63)
static const float dx100_ratios[] = {
    0.50f,  0.71f,  0.78f,  0.87f,  1.00f,  1.41f,  1.57f,  1.73f,
    2.00f,  2.82f,  3.00f,  3.14f,  3.46f,  4.00f,  4.24f,  4.71f,
    5.00f,  5.19f,  5.65f,  6.00f,  6.28f,  6.92f,  7.00f,  7.07f,
    7.85f,  8.00f,  8.48f,  8.65f,  9.00f,  9.42f,  9.89f, 10.00f,
   10.38f, 10.99f, 11.00f, 11.30f, 12.00f, 12.11f, 12.56f, 12.72f,
   13.00f, 13.84f, 14.00f, 14.10f, 14.13f, 15.00f, 15.55f, 15.37f,
   15.70f, 16.96f, 17.27f, 17.30f, 18.37f, 18.84f, 19.03f, 19.78f,
   20.41f, 20.76f, 21.20f, 21.98f, 22.49f, 23.53f, 24.22f, 25.95f
};

// Map 4-op algorithm (0-7) to DX7 algorithm (0-31)
// 4-op algo 1-8 maps to DX7 algo 1,14,8,7,5,22,31,32 (zero-indexed below)
static const uint8_t algo4to6[] = { 0, 13, 7, 6, 4, 21, 30, 31 };

// Convert a 4-op frequency ratio index to DX7 coarse + fine
static void ratioToDx7Freq(int ratioIdx, uint8_t &coarse, uint8_t &fine) {
    if (ratioIdx < 0 || ratioIdx >= 64) {
        coarse = 1;
        fine = 0;
        return;
    }
    float ratio = dx100_ratios[ratioIdx];

    // DX7 coarse: 0 = 0.5, 1-31 = integer ratio
    if (ratio < 0.75f) {
        coarse = 0;  // 0.5 ratio
        fine = 0;
    } else {
        coarse = (uint8_t)juce::jlimit(1, 31, (int)roundf(ratio));
        // Fine tunes the ratio: freq = coarse * (1 + fine/100)
        // So fine = ((ratio / coarse) - 1) * 100
        if (coarse > 0) {
            float fineF = (ratio / (float)coarse - 1.0f) * 100.0f;
            fine = (uint8_t)juce::jlimit(0, 99, (int)roundf(fineF));
        } else {
            fine = 0;
        }
    }
}

int Cartridge::load4opSysex(const uint8_t *data4op, int size) {
    // Validate minimum size: 6 header + 4096 data + checksum + F7
    if (size < 4104) {
        TRACE("4-op sysex too small: %d bytes", size);
        return 2;
    }

    const uint8_t *voices4op = data4op + 6;  // skip 6-byte header

    // Build DX7 cartridge: 32 voices × 128 bytes packed
    uint8_t dx7packed[4096];
    memset(dx7packed, 0, 4096);

    for (int v = 0; v < 32; v++) {
        const uint8_t *src = voices4op + (v * 128);
        uint8_t *dst = dx7packed + (v * 128);

        // ---- Convert 4 operators ----
        // 4-op order in sysex: op4(bytes 0-9), op3(10-19), op2(20-29), op1(30-39)
        // DX7 packed order: op6(bytes 0-16), op5(17-33), op4(34-50), op3(51-67), op2(68-84), op1(85-101)
        // We map 4-op op4→DX7 op4, op3→op3, op2→op2, op1→op1
        // DX7 op6 and op5 are left silent (output level = 0)

        for (int op4idx = 0; op4idx < 4; op4idx++) {
            const uint8_t *opsrc = src + (op4idx * 10);

            // 4-op operators are in order op4,op3,op2,op1 (indices 0,1,2,3)
            // Map to DX7 operators: op4→DX7_op4(idx 2), op3→DX7_op3(idx 3), op2→DX7_op2(idx 4), op1→DX7_op1(idx 5)
            int dx7opIdx = op4idx + 2;  // skip op6(0) and op5(1)
            uint8_t *opdst = dst + (dx7opIdx * 17);

            // Scale envelope rates from 4-op range (0-31) to DX7 range (0-99)
            uint8_t ar  = (uint8_t)juce::jlimit(0, 99, (int)(opsrc[0] * 99 / 31));
            uint8_t d1r = (uint8_t)juce::jlimit(0, 99, (int)(opsrc[1] * 99 / 31));
            uint8_t d2r = (uint8_t)juce::jlimit(0, 99, (int)(opsrc[2] * 99 / 31));
            uint8_t rr  = (uint8_t)juce::jlimit(0, 99, (int)(opsrc[3] * 99 / 15));
            uint8_t d1l = (uint8_t)juce::jlimit(0, 99, (int)(opsrc[4] * 99 / 15));

            // DX7 packed operator format (17 bytes):
            // 0-3: EG rates R1-R4
            opdst[0] = ar;    // R1 = attack
            opdst[1] = d1r;   // R2 = decay1
            opdst[2] = d2r;   // R3 = decay2
            opdst[3] = rr;    // R4 = release
            // 4-7: EG levels L1-L4
            opdst[4] = 99;    // L1 = attack peak
            opdst[5] = d1l;   // L2 = decay1 level (sustain)
            opdst[6] = d1l;   // L3 = sustain level (same as D1L)
            opdst[7] = 0;     // L4 = release end

            // 8: keyboard level scaling break point
            opdst[8] = 39;  // C3 (middle of keyboard)
            // 9-10: scaling left/right depth
            opdst[9] = 0;
            opdst[10] = 0;

            // 11: left curve (bits 0-1) | right curve (bits 2-3)
            opdst[11] = 0;

            // 12: detune (bits 3-6) | rate scaling (bits 0-2)
            uint8_t rs_dbt = opsrc[9];
            uint8_t rateScaling = (rs_dbt >> 3) & 0x03;
            uint8_t detune4op = rs_dbt & 0x07;
            // 4-op detune is 0-6 (center=3), DX7 detune is 0-14 (center=7)
            uint8_t detune7 = (uint8_t)juce::jlimit(0, 14, (int)(detune4op * 2 + 1));
            opdst[12] = (detune7 << 3) | rateScaling;

            // 13: key velocity sensitivity (bits 2-4) | amp mod sensitivity (bits 0-1)
            uint8_t ame_ebs_kvs = opsrc[6];
            uint8_t kvs = ame_ebs_kvs & 0x07;
            uint8_t ams = (ame_ebs_kvs >> 6) & 0x01;  // amp mod enable → sensitivity
            opdst[13] = (kvs << 2) | ams;

            // 14: output level (0-99)
            opdst[14] = opsrc[7];

            // 15: freq coarse (bits 1-5) | osc mode (bit 0)
            uint8_t coarse, fine;
            ratioToDx7Freq(opsrc[8], coarse, fine);
            opdst[15] = (coarse << 1) | 0;  // mode 0 = ratio

            // 16: freq fine (0-99)
            opdst[16] = fine;
        }

        // ---- Set DX7 op6 and op5 to silent ----
        for (int silentOp = 0; silentOp < 2; silentOp++) {
            uint8_t *opdst = dst + (silentOp * 17);
            // Fast decay, zero output
            opdst[0] = 99; opdst[1] = 99; opdst[2] = 99; opdst[3] = 99;
            opdst[4] = 99; opdst[5] = 99; opdst[6] = 99; opdst[7] = 0;
            opdst[8] = 39; // break point
            opdst[14] = 0; // output level = 0 (silent)
            opdst[15] = 2; // coarse = 1, ratio mode
            opdst[12] = (7 << 3); // detune = 7 (center)
        }

        // ---- Global voice parameters ----
        uint8_t sy_fbl_alg = src[40];
        uint8_t algorithm4op = sy_fbl_alg & 0x07;
        uint8_t feedback = (sy_fbl_alg >> 3) & 0x07;

        // Pitch EG (bytes 102-109 in DX7 packed)
        // 4-op synths don't have per-voice pitch EG, set flat
        dst[102] = 50; dst[103] = 50; dst[104] = 50; dst[105] = 50; // rates
        dst[106] = 50; dst[107] = 50; dst[108] = 50; dst[109] = 50; // levels

        // Algorithm + osc key sync (byte 110)
        dst[110] = algo4to6[algorithm4op & 0x07];

        // Feedback + osc sync (byte 111)
        uint8_t oscSync = (sy_fbl_alg >> 6) & 0x01;
        dst[111] = (oscSync << 3) | feedback;

        // LFO parameters
        dst[112] = src[41]; // LFO speed (0-99)
        dst[113] = src[42]; // LFO delay (0-99)
        dst[114] = src[43]; // LFO pitch mod depth (0-99)
        dst[115] = src[44]; // LFO amp mod depth (0-99)

        // LFO PMS | wave | sync (byte 116)
        uint8_t pms_ams_lfw = src[45];
        uint8_t lfoWave = pms_ams_lfw & 0x03;
        uint8_t pitchModSens = (pms_ams_lfw >> 4) & 0x07;
        // DX7 byte 116: LPMS(bits 4-6) | LFW(bits 1-3) | LKS(bit 0)
        dst[116] = (pitchModSens << 4) | (lfoWave << 1) | 0;

        // Transpose (byte 117)
        // 4-op transpose: 0-48 (24=C3), same as DX7
        dst[117] = src[46];

        // Voice name (bytes 118-127)
        // 4-op name starts at src[57] for 10 bytes
        for (int i = 0; i < 10; i++) {
            uint8_t c = (i + 57 < 128) ? src[57 + i] : ' ';
            dst[118 + i] = c & 0x7F;
        }
    }

    // Store as DX7 cartridge
    uint8_t header[] = SYSEX_HEADER;
    memcpy(voiceData, header, 6);
    memcpy(voiceData + 6, dx7packed, 4096);
    voiceData[4102] = sysexChecksum(voiceData + 6, 4096);
    voiceData[4103] = 0xF7;

    TRACE("4-op to DX7 conversion complete");
    return 0;
}
