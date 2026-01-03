#pragma once

#include "parameter/DexedApvts.h"
#include "parameter/Model.h"

class MidiCCHandler : public juce::ValueTree::Listener {
    DexedApvts &apvts;
    std::array<juce::RangedAudioParameter*, 127> map;
    juce::RangedAudioParameter *mapInProgress;
    juce::ValueTree vtConfig;
    std::function<void()> callback = [] {};

public:
    MidiCCHandler(DexedApvts &apvts) : apvts(apvts) {
        vtConfig = apvts.rootVt.getChildWithName(IDs::profile).getChildWithName(IDs::midiCCMappings);
        jassert(vtConfig.isValid());
        vtConfig.addListener(this);
        mapInProgress = nullptr;
        map.fill(nullptr);
    }

    ~MidiCCHandler() {
        vtConfig.removeListener(this);
    }

    /**
     * This is call by the UI to start mapping the CC.
     * @param parameter the JUCE RangeAudioParameter reference
     * @param callback the UI callback on the mapping is done
     */
    void startMapping(juce::RangedAudioParameter *parameter, std::function<void()> callback) {
        mapInProgress = parameter;
        this->callback = callback;
    }

    void cancelMapping() {
        callback = [] {};
        mapInProgress = nullptr;
    }

    void removeMapping(int cc) {
        for (auto i : vtConfig) {
            int foundCC = i.getProperty(IDs::midiCC);
            if ( foundCC == cc ) {
                vtConfig.removeChild(i, nullptr);
                return;
            }
        }
    }

    /**
     * Returns the midi CC for the parameter name
     * @param parameterName
     * @return the cc associated of the parameter, or -1 if not found
     */
    int getMapping(juce::String parameterName) {
        for (auto i : vtConfig) {
            if ( i.getProperty(IDs::midiParameterName) == parameterName ) {
                return i.getProperty(IDs::midiCC);
            }
        }
        return -1;
    }

    /**
     * This is callback by the audio thread when a midi CC message is received
     * @param cc the midi CC id to act upon
     * @param value the midi of value of this CC midi event
     */
    void processControlChange(int cc, int value) {
        if ( mapInProgress ) {
            map[cc] = mapInProgress;
            removeMapping(cc);
            juce::ValueTree ccConfig { { IDs::midiCCMapping }};
            ccConfig.setProperty(IDs::midiCC, cc, nullptr);
            ccConfig.setProperty(IDs::midiParameterName, mapInProgress->getParameterID(), nullptr);
            vtConfig.addChild(ccConfig, -1, nullptr);
            mapInProgress = nullptr;
            juce::MessageManager::callAsync(callback);
            callback = [] {};
            return;
        }

        // We are from the process thread, we change the value
        juce::RangedAudioParameter *parameter = map[cc];
        if ( parameter != nullptr ) {
            float normalizedValue = static_cast<float>(value) / 127.0f;
            parameter->beginChangeGesture();
            parameter->setValueNotifyingHost(normalizedValue);
            parameter->endChangeGesture();
        }
    }

    void valueTreeChildAdded(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override {
        if ( mapInProgress != nullptr )
            return;

        int idx = childWhichHasBeenAdded.getProperty(IDs::midiCC);
        juce::String parameterName = childWhichHasBeenAdded.getProperty(IDs::midiParameterName);
        juce::RangedAudioParameter *parameter = apvts.getParameter(parameterName);
        if ( parameter != nullptr ) {
            map[idx]= parameter;
            return;
        }

        // Parameter not found
        jassertfalse;

    }

    void valueTreeChildRemoved(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved,
        int indexFromWhichChildWasRemoved) override {
        if ( mapInProgress != nullptr )
            return;

        int idx = childWhichHasBeenRemoved.getProperty(IDs::midiCC);
        map[idx] = nullptr;
    }
};