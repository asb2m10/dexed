
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

    // DX7 EMULATED PARAMETERS
    // --------------------------------------------------------------------------------------------------
    // GLOBAL PARAMETERS
    // -------------------------------------------

    params.add(std::make_unique<AudioParameterInt>(IDs::algorithm.parameter(), IDs::algorithm.displayName(), 1, 32, 1));
    params.add(std::make_unique<ParameterDx>(IDs::feedback, 8));
    params.add(std::make_unique<ParameterDx>(IDs::lfoRate, 99));
    params.add(std::make_unique<ParameterDx>(IDs::lfoDelay, 99));
    params.add(std::make_unique<ParameterDx>(IDs::lfoPitchDepth, 1));
    params.add(std::make_unique<ParameterDx>(IDs::lfoAmpDepth, 1));
    params.add(std::make_unique<AudioParameterBool>(IDs::lfoKeySync.parameter(), IDs::lfoKeySync.displayName(), false));

    juce::StringArray lfoWaveformChoices = { "TRIANGLE", "SAW DOWN", "SAW UP", "SQUARE", "SINE" };
    params.add(std::make_unique<AudioParameterChoice>(IDs::lfoWaveform.parameter(), IDs::lfoWaveform.displayName(), lfoWaveformChoices, 0));

    // OPERATOR PARAMETERS
    // -------------------------------------------

    for (int i=0;i<6;i++) {
        juce::String opName = "OP" + juce::String(i+1);
        auto group = std::make_unique<juce::AudioProcessorParameterGroup>(opName, opName, "|");
        int opTarget = (5-i) * 21;

        group->addChild(std::make_unique<ParameterDx>(IDs::outputLevel.op(i), 99));

        for (int j=0; j<4; j++) {
            group->addChild(std::make_unique<ParameterDx>(IDs::egRate.op(i).idx(j), 99));
            group->addChild(std::make_unique<ParameterDx>(IDs::egLevel.op(i).idx(j), 99));
        }
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
