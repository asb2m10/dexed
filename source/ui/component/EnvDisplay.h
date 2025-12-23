#pragma once

#include "parameter/DexedApvts.h"
#include "parameter/Model.h"

class EnvDisplay : public juce::Component {
    juce::CachedValue<int> rates[4];
    juce::CachedValue<int> levels[4];
public:
    char vPos = 0;

    EnvDisplay(DexedApvts &apvts, int op);
    void paint(juce::Graphics &g);
};

class PitchEnvDisplay : public juce::Component {
    juce::CachedValue<int> rates[4];
    juce::CachedValue<int> levels[4];
public:
    char vPos = 0;

    PitchEnvDisplay(DexedApvts &apvts);
    void paint(juce::Graphics &g);
};
