#pragma once

#include <juce_core/juce_core.h>
#include "Dexed.h"

#define SYSEX_HEADER { 0xF0, 0x43, 0x00, 0x09, 0x20, 0x00 }
#define SYSEX_SIZE 4104

uint8_t sysexChecksum(const uint8_t *sysex, int size);
void exportSysexPgm(uint8_t *dest, uint8_t *src);

class Cartridge {
    uint8_t voiceData[SYSEX_SIZE];
    uint8_t perfData[SYSEX_SIZE];

    void setHeader() {
        uint8_t voiceHeader[] = SYSEX_HEADER;
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

    static juce::String normalizePgmName(const char *sysexName);

    int load(juce::File f) {
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
    int load(juce::InputStream &fis) {
        uint8_t buffer[65535];
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
    int load(const uint8_t *stream, int size);
    int saveVoice(juce::File f);

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

    void getProgramNames(juce::StringArray &dest) {
        dest.clear();
        for (int i = 0; i < 32; i++)
            dest.add( normalizePgmName(getRawVoice() + ((i * 128) + 118)) );
    }

    juce::String getProgramName(int idx) {
        jassert(idx >= 0 && idx < 32);
        return normalizePgmName(getRawVoice() + ((idx * 128) + 118));
    }

    Cartridge operator =(const Cartridge other) {
        memcpy(voiceData, other.voiceData, SYSEX_SIZE);
        memcpy(perfData, other.perfData, SYSEX_SIZE);
        return *this;
    }

    void replaceProgram(int idx, char *src) {
        memcpy(getRawVoice() + (idx * 128), src, 128);
    }

    void unpackProgram(uint8_t *unpackPgm, int idx);
    void packProgram(uint8_t *src, int idx, juce::String name, char *opSwitch);
};
