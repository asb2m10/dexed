#pragma once

#include "parameter/DexedApvts.h"

class Program {
    static constexpr int PROGRAM_SIZE = 161;
    uint8_t target_data[PROGRAM_SIZE];
    uint8_t *data;
public:
    Program() {
        data = target_data;
        memset(data, 0, sizeof(target_data));
    }

    juce::String getProgramName() const {
        return normalizePgmName(reinterpret_cast<const char *>(data + 145));
    }

    static juce::String normalizePgmName(const char *sysexName);

    /**
     * Push current program data into the given AudioProcessorValueTreeState
     * @param apvts
     */
    void pushToParameters(const DexedApvts &apvts) const;

    /**
     * This will apply the initial program data to preset (sine wave)
     */
    void applyInitialProgram();


    // The stuff here is there to support dexed 1.0.x code migration
    [[deprecated]]
    Program(uint8_t *data) {
        this->data = data;
    }

    [[deprecated]]
    const uint8_t *getUnpackedData() const {
        return data;
    }
};