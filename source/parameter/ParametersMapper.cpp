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
    // We set the 154 parameters
    for (const auto &param : dxParameters) {
        parameters.mapTo(param.name, [this, param](float newValue) {
            if ( parameters.pushToParameterInProgress )
                return;
            uint8_t value = static_cast<uint8_t>(newValue) + param.displayOffset;
            setDxValue(param.pos, value);
        });
    }

    // Handle operator ON switches
    for (int i=0; i<6; i++) {
        parameters.mapTo( IDs::on.op(i).name, [this, i](float newValue) {
            if ( newValue > 0.5f ) {
                activeProgram[IDs::on.pos] |= (1U << (5-i));
            } else {
                activeProgram[IDs::on.pos] &= ~(1U << (5-i));
            }
            refreshVoice.set(true);
        });
    }

    parameters.mapTo(IDs::cutoff.name, [this](float newValue) {
        fx.uiCutoff = newValue;
    });

    parameters.mapTo(IDs::resonance.name, [this](float newValue) {
        fx.uiReso = newValue;
    });

    parameters.mapTo(IDs::output.name, [this](float newValue) {
        fx.uiGain = newValue;
    });

    parameters.mapTo(IDs::masterTuneAdj.name, [this](float newValue) {
        int32_t tune = ( (newValue/2+0.5) * 0x4000) - 0x2000;
        controllers.masterTune = ((float) (tune << 11)) * (1.0/12);
    });

    parameters.mapTo(IDs::monoMode.name, [this](float newValue) {
        command.push([](DexedAudioProcessor &proc) {
            proc.resetMonoMode();
        });
    });
}


int DexedAudioProcessor::getNumPrograms() {
    return 32;
}

int DexedAudioProcessor::getCurrentProgram() {
    return currentProgram;
}

void DexedAudioProcessor::setCurrentProgram(int index) {
    index = index > 31 ? 31 : index;
    Program program = currentCart.unpackProgram(index);
    applyProgram(program);
    currentProgram = index;
}

const String DexedAudioProcessor::getProgramName(int index) {
    if (index >= 32)
        index = 31;
    return programNames[index];
}

void DexedAudioProcessor::changeProgramName(int index, const String& newName) {
}

void DexedAudioProcessor::setDxValue(int offset, int v) {
    if (offset < 0)
        return;

    if ( activeProgram[offset] != v ) {
        activeProgram[offset] = v;
    } else {
        return;
    }

    refreshVoice.set(true);

    // MIDDLE C (transpose)
    if (offset == 144)
        panic();

    if (!sendSysexChange)
        return;

    uint8 msg[7] = { 0xF0, 0x43, 0x10, offset > 127, 0, (uint8) v, 0xF7 };
    msg[2] = 0x10 | sysexComm.getChl();
    msg[4] = offset & 0x7F;

    if ( sysexComm.isOutputActive() ) {
        //TRACE("SENDING SYSEX: %.2X%.2X %.2X%.2X %.2X%.2X %.2X", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6]);
        sysexComm.send(MidiMessage(msg,7));
    }
}