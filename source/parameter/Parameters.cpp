
#include "Model.h"
#include "PluginProcessor.h"

using Attributes = std::vector<std::pair<juce::Identifier, int>>;

struct ParamAttributes {
    Attributes attributes;
    ParamAttributes(const Attributes &attributes) :
        attributes(attributes) {
    }
};

class AttrParameterInt : public juce::AudioParameterInt, public ParamAttributes {
public:
    AttrParameterInt(const juce::ParameterID& paramID, const juce::String& paramName, int minValue,
                   int maxValue, const Attributes &attributes)
        : juce::AudioParameterInt(paramID, paramName, minValue, maxValue, minValue),
            ParamAttributes(attributes) {}
};

class AttrParameterDx : public juce::AudioParameterInt, public ParamAttributes {
public:
    AttrParameterDx(const juce::ParameterID& paramID, const juce::String& paramName, int steps, int offset)
        : juce::AudioParameterInt(paramID, paramName, 0, steps, 0),
            ParamAttributes(Attributes { { IDs::offset, offset }} ) {}

    AttrParameterDx(const MetaParameterID &paramID, int steps, int offset)
        : juce::AudioParameterInt(paramID.parameter(), paramID.displayName(), 0, steps, 0),
            ParamAttributes(Attributes { { IDs::offset, offset }} ) {}
};


class AttrParameterDxSwitch : public juce::AudioParameterBool, public ParamAttributes {
public:
    AttrParameterDxSwitch(const MetaParameterID &paramID, int offset)
        : juce::AudioParameterBool(paramID.parameter(), paramID.displayName(), false),
            ParamAttributes(Attributes { { IDs::offset, offset } } ) {}
};

class AttrParameterDxChoice : public juce::AudioParameterChoice, public ParamAttributes {
public:
    AttrParameterDxChoice(const MetaParameterID &paramID, const juce::StringArray &choices, int offset)
        : juce::AudioParameterChoice(paramID.parameter(), paramID.displayName(), choices, 0),
            ParamAttributes(Attributes { { IDs::offset, offset } } ) {}
};

juce::AudioProcessorValueTreeState::ParameterLayout DexedAudioProcessor::createParameterLayout() {
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

    params.add(std::make_unique<AttrParameterInt>(IDs::algorithm.parameter(), IDs::algorithm.displayName(), 1, 32,
        Attributes{ { IDs::displayValue, -1 }, { IDs::offset, 134 } } ));

    params.add(std::make_unique<AttrParameterDx>(IDs::feedback, 8, 135));
    params.add(std::make_unique<AttrParameterDx>(IDs::lfoRate, 99, 137));
    params.add(std::make_unique<AttrParameterDx>(IDs::lfoDelay, 99, 138));
    params.add(std::make_unique<AttrParameterDx>(IDs::lfoPitchDepth, 1, 139));
    params.add(std::make_unique<AttrParameterDx>(IDs::lfoAmpDepth, 1, 140));
    params.add(std::make_unique<AttrParameterDxSwitch>(IDs::lfoKeySync, 141));

    juce::StringArray lfoWaveformChoices = { "TRIANGLE", "SAW DOWN", "SAW UP", "SQUARE", "SINE" };
    params.add(std::make_unique<AttrParameterDxChoice>(IDs::lfoWaveform, lfoWaveformChoices, 142));

    // OPERATOR PARAMETERS
    // -------------------------------------------

    for (int i=0;i<6;i++) {
        juce::String opName = "OP" + juce::String(i+1);
        auto group = std::make_unique<juce::AudioProcessorParameterGroup>(opName, opName, "|");
        int opTarget = (5-i) * 21;

        group->addChild(std::make_unique<AttrParameterDx>(IDs::outputLevel.op(i), 99, opTarget + 16));

        for (int j=0; j<4; j++) {
            group->addChild(std::make_unique<AttrParameterDx>(IDs::egRate.op(i, j), 99, opTarget + j));
            group->addChild(std::make_unique<AttrParameterDx>(IDs::egLevel.op(i, j), 99, opTarget + j + 4));
        }
        params.add(std::move(group));
    }

    return params;
}

void DexedAudioProcessor::applyValueTreeAttributes() {
    for (auto child : parameters.state ) {
        juce::String name = child.getProperty(IDs::id).toString();

        ParamAttributes *attrParam = dynamic_cast<ParamAttributes*>(parameters.getParameter(name));
        if (attrParam != nullptr) {
            for (auto &attr : attrParam->attributes) {
                juce::Identifier attrID = attr.first;
                int attrValue = attr.second;
                child.setProperty(attrID, attrValue, nullptr);
            }
        }
    }
}
