#pragma once

#include "parameter/DexedApvts.h"

/**
 * Component that shows the parameter name and value of the parameter
 * currently under the mouse pointer.
 */
class ParameterObserver : public juce::Component, juce::ValueTree::Listener {
    const juce::String defaultMsg = "DEXED " DEXED_VERSION;
    DexedApvts &apvts;

    juce::RangedAudioParameter *parameter = nullptr;
    juce::Atomic<bool> dirty { false };

public:
    ParameterObserver(juce::Component *topComponent, DexedApvts &apvts) : apvts(apvts) {
        topComponent->addMouseListener(this, true);
        apvts.state.addListener(this);
    }

    ~ParameterObserver() override {
        apvts.state.removeListener(this);
    }

    void refresh() {
        repaint();
    }

    void mouseEnter(const juce::MouseEvent &event) override {
        juce::Component *comp = event.eventComponent;
        if ( comp != nullptr ) {
            juce::String name = comp->getName();
            if ( name.isEmpty() )
                return;
            juce::RangedAudioParameter *param = apvts.getParameter(name);
            if ( param != nullptr ) {
                parameter = param;
                dirty.set(true);
                repaint();
            }
        }
    }

    void paint(juce::Graphics &g) override {
        g.setColour (Colours::white);
        juce::RangedAudioParameter *param = parameter;
        if ( param != nullptr ) {
            g.drawFittedText(param->getName(100) + ": " + param->getCurrentValueAsText(),
                        0, 0, getWidth(), getHeight(),
                        Justification::centred, false);
        } else {
            g.drawFittedText(defaultMsg,
                        0, 0, getWidth(), getHeight(),
                        Justification::centred, false);
        }
    }

    void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override {
        juce::RangedAudioParameter *param = parameter;
        if ( apvts.pushToParameterInProgress )
            return;

        dirty.set(true);
        repaint();

    }
};
