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

#include "PluginData.h"
#include "Dexed.h"
#include "PluginParam.h"
#include "PluginProcessor.h"

#include <fstream>

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
    /** TODO: reset parameters */

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
    activeProgram.applyInitialProgram();
    unpackOpSwitch(0x3F);
    panic();
    /** TODO: reset parameters */
}

void DexedAudioProcessor::copyToClipboard(int srcOp) {
    DexedClipboard clipboard(data + (srcOp *21), 21);
    clipboard.write(String("Program: '") + getProgramName(getCurrentProgram()) + "' operator: " + String(6-srcOp));
}

void DexedAudioProcessor::pasteOpFromClipboard(int destOp) {
    DexedClipboard clipboard;

    jassert(clipboard.isOperatorData());

    memcpy(data+(destOp*21), clipboard.getRawData(), 21);
    /** TODO: reset parameters */
}

void DexedAudioProcessor::pasteEnvFromClipboard(int destOp) {
    DexedClipboard clipboard;

    jassert(clipboard.isOperatorData());

    memcpy(data+(destOp*21), clipboard.getRawData(), 8);
    /** TODO: reset parameters */
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
