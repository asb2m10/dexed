#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Cheap imitation of CachedValue but for AudioProcessorValueTreeState parameters.
 * Uses ParameterAttachment for callback distpaching on the message thread if needed.
 * *Callback* std::function<void()> can be assigned if value changes for UI updates.
 *
 * @tparam Type
 */
template <typename Type>
class CachedParameter {
public:
    CachedParameter() : lastValue(0) {
    }

    operator Type() const noexcept {
        return lastValue;
    }

    void referTo(juce::AudioProcessorValueTreeState &apvts, const juce::String &paramID) {
        parameter = apvts.getParameter(paramID);
        if ( parameter != nullptr ) {
            updateValue(parameter->getValue());
            attachment = std::make_unique<juce::ParameterAttachment>(*parameter, [this](float f) {
                updateValue(parameter->getValue());
            });
            return;
        }

        // No parameters found
        jassertfalse;
   }

    /**
     * Add a callback to be called when the parameter value changes.
     */
    std::function<void()> callback = []() {};
private:
    std::atomic<Type> lastValue;

    void updateValue(float newValue) {
        lastValue.store(parameter->convertFrom0to1(newValue));
        callback();
    }
    juce::RangedAudioParameter *parameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> attachment;
};
