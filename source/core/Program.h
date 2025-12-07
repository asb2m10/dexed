#pragma once

#include "parameter/DexedApvts.h"

class Program {
    uint8_t data[156];

public:
    [[deprecated]]
    const uint8_t *getUnpackedData() const {
        return data;
    }

    void applyToParameters(const juce::AudioProcessorValueTreeState &apvts) const;
};