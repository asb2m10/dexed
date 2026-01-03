
#include "Model.h"
#include "PluginProcessor.h"
#include "Dexed.h"

class ParameterDx : public juce::AudioParameterInt {
public:
    ParameterDx(const MetaParameterID &paramID, int steps)
        : juce::AudioParameterInt(paramID.parameter(), paramID.displayName(), 0, steps, 0) {
    }
};

class ParameterCallback : public juce::AudioProcessorParameter::Listener {
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

juce::String getBreakpointLabel(int value) {
    juce::StringArray breakNames = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
    String ret;
    ret << breakNames[value%12] << (value+9) / 12 - 1;
    return ret;
}

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
        0.0f));

    // DX7 EMULATED PARAMETERS
    // --------------------------------------------------------------------------------------------------
    // GLOBAL PARAMETERS
    // -------------------------------------------

    params.add(std::make_unique<AudioParameterInt>(IDs::algorithm.parameter(), IDs::algorithm.displayName(), 1, 32, 1));
    params.add(std::make_unique<ParameterDx>(IDs::feedback, 7));
    params.add(std::make_unique<ParameterDx>(IDs::lfoRate, 99));
    params.add(std::make_unique<ParameterDx>(IDs::lfoDelay, 99));
    params.add(std::make_unique<ParameterDx>(IDs::lfoPmDepth, 99));
    params.add(std::make_unique<ParameterDx>(IDs::lfoAmpDepth, 99));
    params.add(std::make_unique<AudioParameterBool>(IDs::lfoKeySync.parameter(), IDs::lfoKeySync.displayName(), false));
    params.add(std::make_unique<AudioParameterBool>(IDs::oscKeySync.parameter(), IDs::oscKeySync.displayName(), false));

    juce::StringArray lfoWaveformChoices = { "TRIANGLE", "SAW DOWN", "SAW UP", "SQUARE", "SINE", "S&HOLD" };
    params.add(std::make_unique<AudioParameterChoice>(IDs::lfoWaveform.parameter(), IDs::lfoWaveform.displayName(), lfoWaveformChoices, 0));
    params.add(std::make_unique<ParameterDx>(IDs::transpose, 48));
    params.add(std::make_unique<ParameterDx>(IDs::pitchModSens, 7));

    params.add(std::make_unique<AudioParameterBool>(IDs::monoMode.parameter(), IDs::monoMode.displayName(), false));
    params.add(std::make_unique<juce::AudioParameterFloat> (
        IDs::masterTuneAdj.parameter(),
        "Master Tune Adj",
        juce::NormalisableRange<float>(-1.0f, 1.0f, 0.001f),
        0.0f));

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

        group->addChild(std::make_unique<AudioParameterBool>(IDs::mode.op(i).parameter(), IDs::mode.op(i).displayName(), false));
        group->addChild(std::make_unique<ParameterDx>(IDs::frequencyCoarse.op(i), 31));
        group->addChild(std::make_unique<ParameterDx>(IDs::frequencyFine.op(i), 99));
        group->addChild(std::make_unique<ParameterDx>(IDs::detune.op(i), 14));
        group->addChild(std::make_unique<AudioParameterInt>(IDs::breakpoint.op(i).parameter(),
            IDs::breakpoint.op(i).displayName(), 0, 99, 0, "",
            [](int value, int maximumStringLength) {
                    return getBreakpointLabel(value);
            }, nullptr));
        group->addChild(std::make_unique<ParameterDx>(IDs::lScaleDepth.op(i), 99));
        group->addChild(std::make_unique<ParameterDx>(IDs::rScaleDepth.op(i), 99));

        juce::StringArray keyScaleChoices = { "-LN", "-EX", "+EX", "+LN" };
        group->addChild(std::make_unique<AudioParameterChoice>(IDs::lKeyScale.op(i).parameter(), IDs::lKeyScale.op(i).displayName(), keyScaleChoices, 0));
        group->addChild(std::make_unique<AudioParameterChoice>(IDs::rKeyScale.op(i).parameter(), IDs::rKeyScale.op(i).displayName(), keyScaleChoices, 0));

        group->addChild(std::make_unique<ParameterDx>(IDs::rateScaling.op(i), 7));
        group->addChild(std::make_unique<ParameterDx>(IDs::ampModeSens.op(i), 3));
        group->addChild(std::make_unique<ParameterDx>(IDs::keyVelocity.op(i), 7));
        group->addChild(std::make_unique<AudioParameterBool>(IDs::on.op(i).parameter(), opName + " switch", true));
        group->addChild(std::make_unique<ParameterDx>(IDs::outputLevel.op(i), 99));

        params.add(std::move(group));
    }

    params.add(std::make_unique<ParameterDx>(IDs::pitchBendUp, 48));
    params.add(std::make_unique<ParameterDx>(IDs::pitchBendDown, 48));
    params.add(std::make_unique<ParameterDx>(IDs::pitchBendStep, 12));

    return params;
}

DexedApvts::DexedApvts(juce::AudioProcessor& processorToConnectTo, juce::UndoManager* undoManagerToUse) :
 juce::AudioProcessorValueTreeState (processorToConnectTo, undoManagerToUse, IDs::parameters,
        createParameterLayout()) {
    rootVt = ValueTree(IDs::root);
    rootVt.addChild(state, -1, nullptr);
    rootVt.setProperty(IDs::version, "1.1.0", nullptr);

    juce::ValueTree profile { IDs::profile, {} };
    juce::ValueTree midiCCMappings { IDs::midiCCMappings, {} };
    profile.addChild(midiCCMappings, -1, nullptr);

    rootVt.addChild(profile, -1, nullptr);
}

void DexedApvts::mapTo(juce::String paramId, const std::function<void(float)> &func) {
    juce::RangedAudioParameter *parameter = getParameter(paramId);
    if ( parameter != nullptr ) {
        callbacks.push_back(std::make_unique<ParameterCallback>(*parameter, func));
    } else {
        // Parameter not found, should not happen
        jassert (false);
    }
}

