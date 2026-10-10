#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class DexedApvts : public juce::AudioProcessorValueTreeState {
    std::vector<std::unique_ptr<juce::AudioProcessorParameter::Listener>> callbacks;
public:
    static constexpr char MODEL_VERSION[] = "1.1.0";

    mutable bool pushToParameterInProgress = false;
    juce::ValueTree rootVt;

    DexedApvts(juce::AudioProcessor& processorToConnectTo, juce::UndoManager* undoManagerToUse);

    /**
     * Maps a parameter to a callback function that will be called when the parameter value changes.
     * Use this for the audio thread.
     */
    void mapTo(juce::String paramId, const std::function<void(float)> &func);
};
