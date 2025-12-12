#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "parameter/Model.h"

class ParameterCallback final : private juce::AudioProcessorParameter::Listener {
    juce::RangedAudioParameter &parameter;
    const std::function<void(float)> func;
public:
    ParameterCallback(juce::RangedAudioParameter &parameter, const std::function<void(float)> funcIn) :
        parameter(parameter), func(funcIn) {
        parameter.addListener(this);
    }

    ~ParameterCallback() override {
        parameter.removeListener(this);
    }

    void parameterValueChanged (int parameterIndex, float newValue) override {
        func(parameter.convertFrom0to1(newValue));
    }

    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override {
    }
};

class DexedApvts : public juce::AudioProcessorValueTreeState {
    std::vector<std::unique_ptr<ParameterCallback>> callbacks;

public:
    DexedApvts(juce::AudioProcessor& processorToConnectTo, juce::UndoManager* undoManagerToUse);

    /**
     * Maps a parameter to a callback function that will be called when the parameter value changes.
     */
    void mapTo(juce::String paramId, const std::function<void(float)> &func) {
        juce::RangedAudioParameter *parameter = getParameter(paramId);
        if ( parameter != nullptr ) {
            callbacks.push_back(std::make_unique<ParameterCallback>(*parameter, func));
        } else {
            // Parameter not found, should not happen
            jassert (false);
        }
    }

    /**
     * Attaches a CachedValue to in the ValueTreeState parameter, usually for UI components.
     */
    template <typename T>
    void attachTo(juce::String paramId, juce::CachedValue<T> &cache) {
        for  (auto param : state) {
            if ( param.getProperty(IDs::id) == paramId ) {
                cache.referTo(param, IDs::value, nullptr);
                return;
            }
        }
    }
};