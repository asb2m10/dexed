#pragma once

#include "PluginProcessor.h"

class ContextMenuAdapter : public juce::MouseListener {
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

        if ( processor.parameters.getParameter(source->getName()) != nullptr ) {
                popup.addItem(7, "Assign parameter to Midi CC...");
                popup.addSeparator();
        }

        addSpecificItems(popup, source);

        popup.addItem(4, "Send current program to DX7");

            if ( processor.getZoomFactor() > 1.0f ) {
                popup.addSeparator();
                popup.addItem(5, "Reset plugin UI scaling factor");
            }
        }
};


//
// switch(popup.show()) {
//     case 1:
//         processor.copyToClipboard(internalOp);
//         break;
//
//     case 2:
//         processor.pasteEnvFromClipboard(internalOp);
//         break;
//
//     case 3:
//         processor.pasteOpFromClipboard(internalOp);
//         break;
//
//     case 4:
//         processor.sendCurrentSysexProgram();
//         break;
//
//     case 5:
//         // auto *editor = dynamic_cast<DexedAudioProcessorEditor*>(getParentComponent()->getParentComponent());
//         // if ( editor != nullptr ) {
//         //     editor->resetZoomFactor();
//         // }
//         break;
// }
