
#include "Model.h"
#include "PluginProcessor.h"

class ParameterDx : public juce::AudioParameterInt {
public:
    ParameterDx(const MetaParameterID &paramID, int steps)
        : juce::AudioParameterInt(paramID.parameter(), paramID.displayName(), 0, steps, 0) {
    }
};

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    params.add(std::make_unique<juce::AudioParameterFloat> (
        IDs::output.parameter(),
        "Output",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f),
        1.0f));

    params.add(std::make_unique<juce::AudioParameterFloat> (
        IDs::cutoff.parameter(),
        "Cutoff",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f),
        1.0f));

    params.add(std::make_unique<juce::AudioParameterFloat> (
        IDs::resonance.parameter(),
        "Resonance",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f),
        1.0f));

    params.add(std::make_unique<juce::AudioParameterFloat> (
    IDs::tune.parameter(),
    "Tune",
    juce::NormalisableRange<float>(0.0f, 1.0f, 0.5f),
    1.0f));

    // DX7 EMULATED PARAMETERS
    // --------------------------------------------------------------------------------------------------
    // GLOBAL PARAMETERS
    // -------------------------------------------

    params.add(std::make_unique<AudioParameterInt>(IDs::algorithm.parameter(), IDs::algorithm.displayName(), 1, 32, 1));
    params.add(std::make_unique<ParameterDx>(IDs::feedback, 8));
    params.add(std::make_unique<ParameterDx>(IDs::lfoRate, 99));
    params.add(std::make_unique<ParameterDx>(IDs::lfoDelay, 99));
    params.add(std::make_unique<ParameterDx>(IDs::lfoPmDepth, 1));
    params.add(std::make_unique<ParameterDx>(IDs::lfoAmpDepth, 1));
    params.add(std::make_unique<AudioParameterBool>(IDs::lfoKeySync.parameter(), IDs::lfoKeySync.displayName(), false));
    params.add(std::make_unique<AudioParameterBool>(IDs::oscKeySync.parameter(), IDs::oscKeySync.displayName(), false));

    juce::StringArray lfoWaveformChoices = { "TRIANGLE", "SAW DOWN", "SAW UP", "SQUARE", "SINE" };
    params.add(std::make_unique<AudioParameterChoice>(IDs::lfoWaveform.parameter(), IDs::lfoWaveform.displayName(), lfoWaveformChoices, 0));
    params.add(std::make_unique<ParameterDx>(IDs::transpose, 1));
    params.add(std::make_unique<ParameterDx>(IDs::pitchModSens, 1));

    params.add(std::make_unique<AudioParameterBool>(IDs::monoMode.parameter(), IDs::monoMode.displayName(), false));
    params.add(std::make_unique<juce::AudioParameterFloat> (
        IDs::masteTuneAdj.parameter(),
        "Master Tune Adj",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f),
        1.0f));

    for (int i=0;i<4;i++) {
        params.add(std::make_unique<ParameterDx>(IDs::pitchEgRate.idx(i), 99));
        params.add(std::make_unique<ParameterDx>(IDs::pitchEgLevel.idx(i), 99));
    }

    // OPERATOR PARAMETERS
    // -------------------------------------------

    for (int i=0;i<6;i++) {
        juce::String opName = "Op" + juce::String(i+1);
        auto group = std::make_unique<juce::AudioProcessorParameterGroup>(opName, opName, "|");
        int opTarget = (5-i) * 21;

        for (int j=0; j<4; j++) {
            group->addChild(std::make_unique<ParameterDx>(IDs::egRate.op(i).idx(j), 99));
            group->addChild(std::make_unique<ParameterDx>(IDs::egLevel.op(i).idx(j), 99));
        }

        group->addChild(std::make_unique<AudioParameterBool>(IDs::mode.op(i).parameter(), IDs::mode.op(i).name, false));
        group->addChild(std::make_unique<ParameterDx>(IDs::frequencyCoarse.op(i), 31));
        group->addChild(std::make_unique<ParameterDx>(IDs::frequencyFine.op(i), 99));
        group->addChild(std::make_unique<ParameterDx>(IDs::detune.op(i), 14));
        group->addChild(std::make_unique<ParameterDx>(IDs::breakpoint.op(i), 8));
        group->addChild(std::make_unique<ParameterDx>(IDs::lScaleDepth.op(i), 99));
        group->addChild(std::make_unique<ParameterDx>(IDs::rScaleDepth.op(i), 99));
        group->addChild(std::make_unique<ParameterDx>(IDs::lKeyScale.op(i), 3));
        group->addChild(std::make_unique<ParameterDx>(IDs::rKeyScale.op(i), 3));
        group->addChild(std::make_unique<ParameterDx>(IDs::rateScaling.op(i), 7));
        group->addChild(std::make_unique<ParameterDx>(IDs::ampModeSens.op(i), 3));
        group->addChild(std::make_unique<ParameterDx>(IDs::keyVelocity.op(i), 7));
        group->addChild(std::make_unique<AudioParameterBool>(IDs::on.op(i).parameter(), opName + "switch", true));
        group->addChild(std::make_unique<ParameterDx>(IDs::outputLevel.op(i), 99));

        params.add(std::move(group));
    }

    return params;
}

DexedApvts::DexedApvts(juce::AudioProcessor& processorToConnectTo, juce::UndoManager* undoManagerToUse) :
 juce::AudioProcessorValueTreeState (processorToConnectTo, undoManagerToUse, IDs::parameters,
        createParameterLayout()) {
}

void DexedAudioProcessor::applyValueTreeAttributes() {
//     for (auto child : parameters.state ) {
//         juce::String name = child.getProperty(IDs::id).toString();
//
//         ParamAttributes *attrParam = dynamic_cast<ParamAttributes*>(parameters.getParameter(name));
//         if (attrParam != nullptr) {
//             for (auto &attr : attrParam->attributes) {
//                 juce::Identifier attrID = attr.first;
//                 int attrValue = attr.second;
//                 child.setProperty(attrID, attrValue, nullptr);
//             }
//         }
//     }
}
