/**
 *
 * Copyright (c) 2014-2016 Pascal Gauthier.
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

#ifndef PLUGINDATA_H_INCLUDED
#define PLUGINDATA_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include <stdint.h>
#include <string.h>
#include "Dexed.h"

uint8_t sysexChecksum(const uint8_t *sysex, int size);
void exportSysexPgm(uint8_t *dest, uint8_t *src);

#define SYSEX_HEADER { 0xF0, 0x43, 0x00, 0x09, 0x20, 0x00 }
#define SYSEX_SIZE 4104

class Cartridge {
    uint8_t voiceData[SYSEX_SIZE];
    uint8_t perfData[SYSEX_SIZE];
    
    void setHeader() {
        uint8 voiceHeader[] = SYSEX_HEADER;
        memcpy(voiceData, voiceHeader, 6);
        voiceData[4102] = sysexChecksum(voiceData+6, 4096);
        voiceData[4103] = 0xF7;
    }
    
public:
    Cartridge() { }
    
    Cartridge(const Cartridge &cpy) {
        memcpy(voiceData, cpy.voiceData, SYSEX_SIZE);
        memcpy(perfData, cpy.perfData, SYSEX_SIZE);
    }
    
    static String normalizePgmName(const char *sysexName) {
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
    
    int load(File f) {
        std::unique_ptr<juce::FileInputStream> fis = f.createInputStream();
        if ( fis == NULL )
            return -1;
        int rc = load(*fis);
        return rc;
    }
    
    /**
     * Loads sysex stream
     * Returns 0 if it was parsed successfully
     * Returns -1 if it cannot open the stream
     */
    int load(InputStream &fis) {
        uint8 buffer[65535];
        int sz = fis.read(buffer, 65535);
        if ( sz == 0 )
            return -1;
        return load(buffer, sz);
    }
    
    /**
     * Loads sysex buffer
     * Returns 0 if it was parsed successfully
     * Returns 1 if sysex checksum didn't match
     * Returns 2 if no sysex data found, probably random data
     */
    int load(const uint8_t *stream, int size) {
        const uint8 *pos = stream;
        
        if ( size < 4096 ) {
            memcpy(voiceData+6, pos, size);
            TRACE("too small sysex rc=2");
            return 2;
        }
        
        if ( pos[0] != 0xF0 ) {
            // it is not, just copy the first 4096 bytes
            memcpy(voiceData + 6, pos, 4096);
            TRACE("stream is not a sysex rc=2");
            return 2;
        }
        
        // limit the size of the sysex scan
        if ( size > 65535 )
            size = 65535;
        
        // we loop until we find something that looks like a DX7 cartridge (based on size)
        while(size >= 4104) {
            // it was a sysex first, now random data; return random
            if ( pos[0] != 0xF0 ) {
                memcpy(voiceData + 6, stream, 4096);
                TRACE("stream was a sysex, but not anymore rc=2");
                return 2;
            }
            
            // check if this is the size of a DX7 sysex cartridge
            for(int i=0;i<size;i++) {
                if ( pos[i] == 0xF7 ) {
                    if ( i == SYSEX_SIZE - 1 ) {
                        memcpy(voiceData, pos, SYSEX_SIZE);
                        if ( sysexChecksum(voiceData + 6, 4096) == pos[4102] ) {
                            TRACE("valid sysex found!");
                            return 0;
                        } else {
                            TRACE("sysex found, but checksum doesn't match rc=1");
                            return 1;
                        }
                    }
                    size -= i;
                    pos += i;
                    TRACE("end of sysex with wrong DX size... still scanning stream: size=%d", i);
                    break;
                }
            }
            TRACE("sysex stream parsed without any end message, skipping...");
            break;
        }
        
        // it is a sysex, but doesn't seems to be related to any DX series ...
        memcpy(voiceData + 6, stream, 4096);
        TRACE("nothing in the sysex stream was DX related rc=2");
        return 2;
    }
    
    int saveVoice(File f) {
        setHeader();
        
        if ( ! f.existsAsFile() ) {
            // file doesn't exists, create it
            return f.replaceWithData(voiceData, SYSEX_SIZE);
        }
        
        std::unique_ptr<juce::FileInputStream> fis = f.createInputStream();
        if ( fis == NULL )
            return -1;
        
        uint8 buffer[65535];
        int sz = fis->read(buffer, 65535);
        
        // if the file is smaller than 4104, it probably needs to be overridden.
        if ( sz <= 4104 ) {
            return f.replaceWithData(voiceData, SYSEX_SIZE);
        }

        // To avoid to erase the performance data, we skip the sysex stream until
        // we see the header 0xF0, 0x43, 0x00, 0x09, 0x20, 0x00
        
        int pos = 0;
        bool found = 0;
        while(pos < sz) {
            // corrupted sysex, erase everything :
            if ( buffer[pos] != 0xF0 )
                return f.replaceWithData(voiceData, SYSEX_SIZE);
            
            uint8_t header[] = SYSEX_HEADER;
            if ( memcmp(buffer+pos, header, 6) ) {
                found = true;
                memcpy(buffer+pos, voiceData, SYSEX_SIZE);
                break;
            } else {
                for(;pos<sz;pos++) {
                    if ( buffer[pos] == 0xF7 )
                        break;
                }
            }
        }
        
        if ( ! found )
            return -1;
        
        return f.replaceWithData(buffer, sz);
    }
    
    void saveVoice(uint8_t *sysex) {
        setHeader();
        memcpy(sysex, voiceData, SYSEX_SIZE);
    }
    
    char *getRawVoice() {
        return (char *) voiceData + 6;
    }
    
    char *getVoiceSysex() {
        setHeader();
        return (char *) voiceData;
    }
    
    void getProgramNames(StringArray &dest) {
        dest.clear();
        for (int i = 0; i < 32; i++)
            dest.add( normalizePgmName(getRawVoice() + ((i * 128) + 118)) );
    }
    
    Cartridge operator =(const Cartridge other) {
        memcpy(voiceData, other.voiceData, SYSEX_SIZE);
        memcpy(perfData, other.perfData, SYSEX_SIZE);
        return *this;
    }
    
    void unpackProgram(uint8_t *unpackPgm, int idx);
    void packProgram(uint8_t *src, int idx, String name, char *opSwitch);
};

#endif  // PLUGINDATA_H_INCLUDED
