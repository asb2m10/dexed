#include "Model.h"
#include "PluginProcessor.h"
#include "core/Program.h"
#include <vector>

#define APPLY_OP(i)              \
    IDs::egRate.op(i).idx(0),    \
    IDs::egRate.op(i).idx(1),    \
    IDs::egRate.op(i).idx(2),    \
    IDs::egRate.op(i).idx(3),    \
    IDs::egLevel.op(i).idx(0),   \
    IDs::egLevel.op(i).idx(1),   \
    IDs::egLevel.op(i).idx(2),   \
    IDs::egLevel.op(i).idx(3),   \
    IDs::breakpoint.op(i),       \
    IDs::lScaleDepth.op(i),      \
    IDs::rScaleDepth.op(i),      \
    IDs::rateScaling.op(i),      \
    IDs::aModSens.op(i),         \
    IDs::keyVelocity.op(i),      \
    IDs::outputLevel.op(i),      \
    IDs::mode.op(i),             \
    IDs::frequencyCoarse.op(i),  \
    IDs::frequencyFine.op(i),    \
    IDs::detune.op(i),           \
    IDs::on.op(i)                \

static const std::vector<MetaParameterID> dxParameters = {
    APPLY_OP(0),
    APPLY_OP(1),
    APPLY_OP(2),
    APPLY_OP(3),
    APPLY_OP(4),
    APPLY_OP(5),
    IDs::output,
    IDs::algorithm,
    IDs::feedback,
    IDs::lfoRate,
    IDs::lfoDelay,
    IDs::lfoPmDepth,
    IDs::lfoAmpDepth,
    IDs::lfoKeySync,
    IDs::lfoWaveform,
    IDs::pitchEgLevel.idx(0),
    IDs::pitchEgLevel.idx(1),
    IDs::pitchEgLevel.idx(2),
    IDs::pitchEgLevel.idx(3),
    IDs::pitchEgRate.idx(0),
    IDs::pitchEgRate.idx(1),
    IDs::pitchEgRate.idx(2),
    IDs::pitchEgRate.idx(3),
};

void DexedAudioProcessor::mapParameters() {
    for (const auto &param : dxParameters) {
        parameters.mapTo(param.name, [this, param](float newValue) {
            uint8_t value = static_cast<uint8_t>(newValue) + param.displayOffset;
            data[param.pos] = value;
        });
    }
}

void Program::applyToParameters(const juce::AudioProcessorValueTreeState &apvts) const {

    for (const auto &param : dxParameters) {
        RangedAudioParameter *parameter = apvts.getParameter(param.name);
        if ( parameter != nullptr ) {
            uint8_t value = data[param.pos];
            float displayValue = static_cast<float>(value - param.displayOffset);
            parameter->beginChangeGesture();
            parameter->setValueNotifyingHost(displayValue);
            parameter->endChangeGesture();
        }
    }
}
