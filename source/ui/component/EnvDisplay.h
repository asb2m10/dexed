#pragma once

#include "parameter/DexedApvts.h"
#include "../util/CachedParameter.h"

class EnvDisplay : public juce::Component {
    CachedParameter<int> rates[4];
    CachedParameter<int> levels[4];
public:
    char vPos = 0;

    EnvDisplay(DexedApvts &apvts, int op);
    void paint(juce::Graphics &g);
};

class PitchEnvDisplay : public juce::Component {
    CachedParameter<int> rates[4];
    CachedParameter<int> levels[4];
public:
    char vPos = 0;

    PitchEnvDisplay(DexedApvts &apvts);
    void paint(juce::Graphics &g);
};
