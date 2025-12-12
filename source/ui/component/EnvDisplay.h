#pragma once

#include "parameter/DexedApvts.h"
#include "parameter/Model.h"

class PitchEnvDisplay : public juce::Component {
    juce::CachedValue<int> rates[4];
    juce::CachedValue<int> levels[4];
public:
    char vPos = 0;

    PitchEnvDisplay(DexedApvts &apvts) {
        for (int i=0; i<4; i++) {
            apvts.attachTo(IDs::pitchEgRate.idx(i).name, rates[i]);
            apvts.attachTo(IDs::pitchEgRate.idx(i).name, levels[i]);
        }
    }

    void paint(juce::Graphics &g);
};
