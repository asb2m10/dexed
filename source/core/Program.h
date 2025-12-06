#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class Program {
    uint8_t pgm[156];

public:
    const unsigned char *getUnpackedData() const {
        return pgm;
    }
};