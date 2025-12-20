#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "parameter/Model.h"

class DexedApvts : public juce::AudioProcessorValueTreeState {
    std::vector<std::unique_ptr<juce::AudioProcessorParameter::Listener>> callbacks;
public:
    mutable bool pushToParameterInProgress = false;

    DexedApvts(juce::AudioProcessor& processorToConnectTo, juce::UndoManager* undoManagerToUse);

    /**
     * Maps a parameter to a callback function that will be called when the parameter value changes.
     */
    void mapTo(juce::String paramId, const std::function<void(float)> &func);

    /**
     * Attaches a CachedValue to in the ValueTreeState parameter, usually for UI components.
     */
    template <typename T>
    void attachTo(juce::String paramId, juce::CachedValue<T> &cache) {
        for (auto param : state) {
            if (param.getProperty(IDs::id) == paramId) {
                cache.referTo(param, IDs::value, nullptr);
                return;
            }
        }
    }
};
