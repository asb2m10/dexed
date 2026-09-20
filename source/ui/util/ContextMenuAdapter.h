#pragma once

#include "ui/PluginEditor.h"
#include "midi/MidiCCHandler.h"
#include "../window/MidiCCListenerStatus.h"

class ContextMenuAdapter : public juce::MouseListener {
protected:
    DexedAudioProcessor &processor;
public:
    ContextMenuAdapter(DexedAudioProcessor &processor) : processor(processor) {
    }

    virtual void addSpecificItems(PopupMenu &menu, juce::Component *source) {
    }

    void mouseDown(const MouseEvent &event) override {
        if ( !event.mods.isPopupMenu())
            return;

        PopupMenu popup;
        juce::Component *source = event.eventComponent;
        juce::RangedAudioParameter *parameter = processor.parameters.getParameter(source->getName());

        if ( parameter != nullptr ) {
            int idx = processor.midiCCMapper->getMapping(source->getName());
            if ( idx != -1 ) {
                popup.addItem("Remove parameter [" + source->getTitle() + "] to CC:" + juce::String(idx), [this, idx] {
                    this->processor.midiCCMapper->removeMapping(idx);
                });
            } else {
                popup.addItem("Assign parameter [" + source->getTitle() + "] to Midi CC...", [source, this, parameter] {
                    new MidiCCListenerStatus(source, this->processor.midiCCMapper.get(), parameter);
                });
            }
            popup.addSeparator();
        }

        addSpecificItems(popup, source);
        popup.addItem("Send current program to DX7", [this] {
            processor.sendCurrentSysexProgram();
        });

        if ( processor.getZoomFactor() > 1.0f ) {
            popup.addSeparator();
            popup.addItem("Reset plugin UI scaling factor", [source] {
                if (auto *editor = source->findParentComponentOfClass<DexedAudioProcessorEditor>())
                    editor->resetZoomFactor();
            });
        }

        auto options = juce::PopupMenu::Options();
        popup.showMenuAsync(options);
    }
};
