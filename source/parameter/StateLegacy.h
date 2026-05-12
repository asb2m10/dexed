// Used for parsing old Dexed 1.0 legacy format

#pragma once

#include "../PluginProcessor.h"
#include "Model.h"

static void setApvtsParam(DexedApvts& apvts, const juce::String& paramId, float value) {
    auto* param = dynamic_cast<juce::RangedAudioParameter*>(apvts.getParameter(paramId));
    if (param)
        param->setValueNotifyingHost(param->convertTo0to1(value));
}

static void setApvtsParamBool(DexedApvts& apvts, const juce::String& paramId, bool value) {
    auto* param = apvts.getParameter(paramId);
    if (param)
        param->setValueNotifyingHost(value ? 1.0f : 0.0f);
}

void setStateInformation10(DexedAudioProcessor* proc, XmlElement* root) {
    auto& apvts = proc->parameters;

    setApvtsParam(apvts, IDs::cutoff.name, (float)root->getDoubleAttribute("cutoff", 1.0));
    setApvtsParam(apvts, IDs::resonance.name, (float)root->getDoubleAttribute("reso", 0.0));
    setApvtsParam(apvts, IDs::output.name, (float)root->getDoubleAttribute("gain", 1.0));
    setApvtsParam(apvts, IDs::engineType.name, (float)root->getIntAttribute("engineType", 0));
    setApvtsParamBool(apvts, IDs::monoMode.name, root->getIntAttribute("monoMode", 0) != 0);
    setApvtsParamBool(apvts, IDs::transposeTuningScale.name, root->getIntAttribute("transpose12AsScale", 1) != 0);
    setApvtsParamBool(apvts, IDs::glissando.name, root->getIntAttribute("glissando", 0) != 0);

    // Old format: portamento 0-127, new APVTS: portamentoTm 0-99
    int oldPortamento = root->getIntAttribute("portamento", 0);
    setApvtsParam(apvts, IDs::portamentoTm.name, oldPortamento * 99.0f / 127.0f);

    // Load program data from dexedBlob
    XmlElement* dexedBlob = root->getChildByName("dexedBlob");
    if (dexedBlob != nullptr) {
        NamedValueSet blobSet;
        blobSet.setFromXmlAttributes(*dexedBlob);
        var program = blobSet["program"];
        if (!program.isVoid() && program.getBinaryData() != nullptr) {
            const auto* data = static_cast<const uint8_t*>(program.getBinaryData()->getData());
            proc->applyProgram(Program(data));
        }
    }

    // Load tuning from dexedTuning child
    XmlElement* tuningParent = root->getChildByName("dexedTuning");
    if (tuningParent != nullptr) {
        proc->retuneToStandard();
        auto* sclx = tuningParent->getChildByName("scl");
        if (sclx && sclx->getFirstChildElement() && sclx->getFirstChildElement()->isTextElement()) {
            std::string s = sclx->getFirstChildElement()->getText().toStdString();
            if (s.size() > 1)
                proc->applySCLTuning(s);
        }
        auto* kbmx = tuningParent->getChildByName("kbm");
        if (kbmx && kbmx->getFirstChildElement() && kbmx->getFirstChildElement()->isTextElement()) {
            std::string k = kbmx->getFirstChildElement()->getText().toStdString();
            if (k.size() > 1)
                proc->applyKBMMapping(k);
        }
    }
}
