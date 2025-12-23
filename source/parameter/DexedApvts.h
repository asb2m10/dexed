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
     * Use this for the audio thread.
     */
    void mapTo(juce::String paramId, const std::function<void(float)> &func);

    /**
     * This holds a ValueTree for each parameter name to add persistent listeners if needed.
     * Use this for the UI thread.
     */
    std::unordered_map<juce::String, juce::ValueTree> nameMapping;
};
