// Used for parsing old Dexed 1.0 legacy format

#pragma once

#include "../PluginProcessor.h"
#include "Model.h"
#include <map>

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

/**
 * Parse a 1.0 modulation config ("range pitch amp eg", ex: "99 1 0 1") into the
 * matching modulation parameters.
 */
static void setLegacyMod(DexedApvts& apvts, const juce::String& cfg,
                         const juce::String& rangeId, const juce::String& pitchId,
                         const juce::String& ampId, const juce::String& egId) {
    juce::StringArray tokens = juce::StringArray::fromTokens(cfg, " ", "");
    tokens.removeEmptyStrings();
    if (tokens.size() < 4)
        return;

    int range = tokens[0].getIntValue();
    setApvtsParam(apvts, rangeId, (range < 0 || range > 127) ? 0.0f : (float) range);
    setApvtsParamBool(apvts, pitchId, tokens[1].getIntValue() != 0);
    setApvtsParamBool(apvts, ampId, tokens[2].getIntValue() != 0);
    setApvtsParamBool(apvts, egId, tokens[3].getIntValue() != 0);
}

/**
 * Maps the 1.0 controller labels (used by the midiCC mappings) to the APVTS parameter ids
 */
static const std::map<juce::String, juce::String>& getLegacyLabels() {
    static const std::map<juce::String, juce::String> labels = [] {
        std::map<juce::String, juce::String> m;
        m["Cutoff"] = IDs::cutoff.name;
        m["Resonance"] = IDs::resonance.name;
        m["Output"] = IDs::output.name;
        m["MASTER TUNE ADJ"] = IDs::masterTuneAdj.name;
        m["ALGORITHM"] = IDs::algorithm.name;
        m["FEEDBACK"] = IDs::feedback.name;
        m["OSC KEY SYNC"] = IDs::oscKeySync.name;
        m["LFO SPEED"] = IDs::lfoRate.name;
        m["LFO DELAY"] = IDs::lfoDelay.name;
        m["LFO PM DEPTH"] = IDs::lfoPmDepth.name;
        m["LFO AM DEPTH"] = IDs::lfoAmpDepth.name;
        m["LFO KEY SYNC"] = IDs::lfoKeySync.name;
        m["LFO WAVE"] = IDs::lfoWaveform.name;
        m["TRANSPOSE"] = IDs::transpose.name;
        m["P MODE SENS."] = IDs::pitchModSens.name;

        for (int i = 0; i < 4; i++) {
            m["PITCH EG RATE " + juce::String(i + 1)] = IDs::pitchEgRate.idx(i).name;
            m["PITCH EG LEVEL " + juce::String(i + 1)] = IDs::pitchEgLevel.idx(i).name;
        }

        for (int op = 0; op < 6; op++) {
            juce::String prefix = "OP" + juce::String(op + 1) + " ";
            for (int j = 0; j < 4; j++) {
                m[prefix + "EG RATE " + juce::String(j + 1)] = IDs::egRate.op(op).idx(j).name;
                m[prefix + "EG LEVEL " + juce::String(j + 1)] = IDs::egLevel.op(op).idx(j).name;
            }
            m[prefix + "OUTPUT LEVEL"] = IDs::outputLevel.op(op).name;
            m[prefix + "MODE"] = IDs::mode.op(op).name;
            m[prefix + "F COARSE"] = IDs::frequencyCoarse.op(op).name;
            m[prefix + "F FINE"] = IDs::frequencyFine.op(op).name;
            m[prefix + "OSC DETUNE"] = IDs::detune.op(op).name;
            m[prefix + "BREAK POINT"] = IDs::breakpoint.op(op).name;
            m[prefix + "L SCALE DEPTH"] = IDs::lScaleDepth.op(op).name;
            m[prefix + "R SCALE DEPTH"] = IDs::rScaleDepth.op(op).name;
            m[prefix + "L KEY SCALE"] = IDs::lKeyScale.op(op).name;
            m[prefix + "R KEY SCALE"] = IDs::rKeyScale.op(op).name;
            m[prefix + "RATE SCALING"] = IDs::rateScaling.op(op).name;
            m[prefix + "A MOD SENS."] = IDs::ampModeSens.op(op).name;
            m[prefix + "KEY VELOCITY"] = IDs::keyVelocity.op(op).name;
            m[prefix + "SWITCH"] = IDs::on.op(op).name;
        }
        return m;
    }();
    return labels;
}

/**
 * Import the 1.0 <midiCC> mappings (cc -> controller label) into the profile mappings
 */
static void setLegacyMidiCC(DexedApvts& apvts, const XmlElement* midiCC) {
    juce::ValueTree mappings = apvts.rootVt.getChildWithName(IDs::profile).getChildWithName(IDs::midiCCMappings);
    if (!mappings.isValid())
        return;

    mappings.removeAllChildren(nullptr);

    const auto& labels = getLegacyLabels();
    for (const auto* mapping : midiCC->getChildIterator()) {
        int cc = mapping->getIntAttribute("cc", -1);
        auto found = labels.find(mapping->getStringAttribute("target"));

        // MidiCCHandler only handles 0-126
        if (cc < 0 || cc > 126 || found == labels.end() || apvts.getParameter(found->second) == nullptr)
            continue;

        // One parameter per cc, last one wins
        for (auto child : mappings) {
            if (static_cast<int>(child.getProperty(IDs::midiCC)) == cc) {
                mappings.removeChild(child, nullptr);
                break;
            }
        }

        juce::ValueTree ccConfig { IDs::midiCCMapping };
        ccConfig.setProperty(IDs::midiCC, cc, nullptr);
        ccConfig.setProperty(IDs::midiParameterName, found->second, nullptr);
        mappings.addChild(ccConfig, -1, nullptr);
    }
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

    // Old master tune was stored as controllers.masterTune (see ParametersMapper for the forward conversion)
    if (root->hasAttribute("masterTune")) {
        int64_t tune = ((int64_t) root->getIntAttribute("masterTune", 0) * 12) >> 11;
        setApvtsParam(apvts, IDs::masterTuneAdj.name, juce::jlimit(-1.0f, 1.0f, (float) tune / 0x2000));
    }

    // Old format: portamento 0-127, new APVTS: portamentoTm 0-99
    int oldPortamento = root->getIntAttribute("portamento", 0);
    setApvtsParam(apvts, IDs::portamentoTm.name, oldPortamento * 99.0f / 127.0f);

    setLegacyMod(apvts, root->getStringAttribute("wheelMod"), IDs::modWheel.name, IDs::modWheelPitch.name,
                 IDs::modWheelAmp.name, IDs::modWheelEgBias.name);
    setLegacyMod(apvts, root->getStringAttribute("footMod"), IDs::modFoot.name, IDs::modFootPitch.name,
                 IDs::modFootAmp.name, IDs::modFootEgBias.name);
    setLegacyMod(apvts, root->getStringAttribute("breathMod"), IDs::modBreath.name, IDs::modBreathPitch.name,
                 IDs::modBreathAmp.name, IDs::modBreathEgBias.name);
    setLegacyMod(apvts, root->getStringAttribute("aftertouchMod"), IDs::modAftertouch.name, IDs::modAftertouchPitch.name,
                 IDs::modAftertouchAmp.name, IDs::modAftertouchEgBias.name);

    // Load the original cartridge and program data from dexedBlob
    XmlElement* dexedBlob = root->getChildByName("dexedBlob");
    if (dexedBlob != nullptr) {
        NamedValueSet blobSet;
        blobSet.setFromXmlAttributes(*dexedBlob);

        var sysex = blobSet["sysex"];
        if (!sysex.isVoid() && sysex.getBinaryData() != nullptr && sysex.getBinaryData()->getSize() == SYSEX_SIZE) {
            Cartridge cart;
            cart.load(static_cast<const uint8_t*>(sysex.getBinaryData()->getData()), SYSEX_SIZE);
            proc->loadCartridge(cart);
            // Select the stored slot; the edited program blob below overrides its content
            proc->setCurrentProgram(root->getIntAttribute("currentProgram", 0));
        }

        var program = blobSet["program"];
        if (!program.isVoid() && program.getBinaryData() != nullptr) {
            const auto* data = static_cast<const uint8_t*>(program.getBinaryData()->getData());
            Program edited(data);
            proc->applyProgram(edited);

            // The legacy edited program is the raw voice; if it is identical to a cartridge slot (the
            // stored index can be stale), reselect that slot so the UI doesn't show "**".
            int match = -1;
            int stored = proc->getCurrentProgram();
            for (int i = 0; i < 32 && match < 0; i++) {
                int slot = (stored + i) % 32;   // prefer the stored slot
                Program candidate = proc->currentCart.unpackProgram(slot);
                if (memcmp(candidate.getRawData(), edited.getRawData(), Program::PROGRAM_SIZE) == 0)
                    match = slot;
            }
            if (match >= 0 && match != stored)
                proc->setCurrentProgram(match);
        }
    }

    // Operator switches, one char per operator, the last char is OP1 ("111111" = all on). This needs to
    // be applied after the program, as it contains its own operator switch state.
    juce::String opSwitch = root->getStringAttribute("opSwitch");
    if (opSwitch.length() == 6) {
        for (int op = 0; op < 6; op++)
            setApvtsParamBool(apvts, IDs::on.op(op).name, opSwitch[5 - op] == '1');
    }

    // Last cartridge file used (only restored if it still exists, like the 1.0 behavior)
    juce::String cartPath = root->getStringAttribute("activeFileCartridge");
    if (cartPath.isNotEmpty()) {
        juce::File cartFile(cartPath);
        if (cartFile.existsAsFile())
            proc->activeFileCartridge = cartFile;
    }

    const XmlElement* midiCC = root->getChildByName("midiCC");
    if (midiCC != nullptr)
        setLegacyMidiCC(apvts, midiCC);

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
