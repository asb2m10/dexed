#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

struct MetaParameterID {
    juce::String name;
    const int pos = 0;
    const int displayOffset = 0;

    MetaParameterID(juce::String name) : name(name), pos(0), displayOffset(0) {}
    MetaParameterID(juce::String name, int pos, int displayOffset) : name(name), pos(pos), displayOffset(displayOffset) {}

    juce::ParameterID parameter() const {
        // For now it is all parameterID with 0 as the version
        return juce::ParameterID(name, 0);
    }

    MetaParameterID op(int op) const {
        return MetaParameterID(juce::String("op" + juce::String(op+1) + name), pos + (5-op) * 21, displayOffset);
    }

    MetaParameterID idx(int idx) const {
        return MetaParameterID(name + juce::String(idx), pos + idx, displayOffset);
    }

    juce::String displayName() const {
        juce::String ret = name;
        for (int i = 0; i < name.length(); ++i) {
            if (i > 0 && name[i] >= 'A' && name[i] <= 'Z') {
                ret += " ";
            }
            ret += name[i];
        }
        return ret.toUpperCase();
    }
};