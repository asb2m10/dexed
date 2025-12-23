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
    IDs::ampModeSens.op(i),      \
    IDs::keyVelocity.op(i),      \
    IDs::outputLevel.op(i),      \
    IDs::mode.op(i),             \
    IDs::frequencyCoarse.op(i),  \
    IDs::frequencyFine.op(i),    \
    IDs::detune.op(i)

const std::vector<MetaParameterID> dxParameters = {
    APPLY_OP(0),
    APPLY_OP(1),
    APPLY_OP(2),
    APPLY_OP(3),
    APPLY_OP(4),
    APPLY_OP(5),
    IDs::pitchEgRate.idx(0),            // pos: 126
    IDs::pitchEgRate.idx(1),
    IDs::pitchEgRate.idx(2),
    IDs::pitchEgRate.idx(3),
    IDs::algorithm,                  // pos: 134
    IDs::feedback,                   // pos: 135
    IDs::lfoRate,                    // pos: 137
    IDs::lfoDelay,                   // pos: 138
    IDs::lfoPmDepth,                 // pos: 139
    IDs::lfoAmpDepth,                // pos: 140
    IDs::lfoKeySync,                 // pos: 141
    IDs::lfoWaveform,                // pos: 142
    IDs::pitchModSens,               // pos: 143
    IDs::transpose                   // pos: 144
};

void DexedAudioProcessor::mapParameters() {
    for (const auto &param : dxParameters) {
        parameters.mapTo(param.name, [this, param](float newValue) {
            if ( parameters.pushToParameterInProgress )
                return;
            uint8_t value = static_cast<uint8_t>(newValue) + param.displayOffset;
            setDxValue(param.pos, value);
        });
    }
}
