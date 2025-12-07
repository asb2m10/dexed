#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class ParameterCallback final : private juce::AudioProcessorParameter::Listener {
    RangedAudioParameter &parameter;
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

    void mapTo(juce::String paramId, const std::function<void(float)> &func) {
        RangedAudioParameter *parameter = getParameter(paramId);
        if ( parameter != nullptr ) {
            callbacks.push_back(std::make_unique<ParameterCallback>(*parameter, func));
        } else {
            // Parameter not found, should not happen
            jassert (false);
        }
    }
};