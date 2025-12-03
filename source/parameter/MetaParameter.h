#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

struct MetaParameterID {
    const juce::String name;
    MetaParameterID(juce::String name) : name(name) {}

    juce::ParameterID parameter() const {
        // For now it is all parameterID with 0 as the version
        return juce::ParameterID(name, 0);
    }

    MetaParameterID op(int op) const {
        return MetaParameterID(juce::String("op" + juce::String(op+1) + name));
    }

    MetaParameterID op(int op, int idx) const {
        return MetaParameterID(juce::String("op" + juce::String(op+1) + name + juce::String(idx+1)));
    }

    MetaParameterID idx(int idx) const {
        return MetaParameterID(name + juce::String(idx+1));
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