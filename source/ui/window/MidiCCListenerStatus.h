#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "midi/MidiCCHandler.h"
#include "Dexed.h"

class MidiCCListenerStatus: public juce::AlertWindow, juce::ModalComponentManager::Callback {
    MidiCCHandler *handler;
public :
    MidiCCListenerStatus(juce::Component *parent, MidiCCHandler *handler, juce::RangedAudioParameter *parameter) :
            AlertWindow("","", AlertWindow::InfoIcon, parent), handler(handler) {
        setMessage("Mapping: " + parameter->getName(127) + "\n\nWaiting for midi controller change (CC) message...");
        addButton("CANCEL", -1);
        handler->startMapping(parameter, [this] {
            this->setVisible(false);
        });
        enterModalState(true, this);
    }

    void modalStateFinished(int returnValue) override {
    }

    ~MidiCCListenerStatus() {
        handler->cancelMapping();
    }
};