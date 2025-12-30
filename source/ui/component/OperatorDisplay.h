#pragma once

#include "../util/CachedParameter.h"
#include "parameter/DexedApvts.h"

class OperatorDisplay : public juce::Component {
    CachedParameter<int> opFine, opCoarse, detune, opMode;
    String message;
public:
    OperatorDisplay(DexedApvts &apvts, int opNum) {
        opFine.referTo(apvts, IDs::frequencyFine.op(opNum).name);
        opFine.callback = [this]() {
            updateDisplay();
        };

        opCoarse.referTo(apvts, IDs::frequencyCoarse.op(opNum).name);
        opCoarse.callback = [this]() {
            updateDisplay();
        };

        detune.referTo(apvts, IDs::detune.op(opNum).name);
        detune.callback = [this]() {
            updateDisplay();
        };

        opMode.referTo(apvts, IDs::mode.op(opNum).name);
        opMode.callback = [this]() {
            updateDisplay();
        };

        updateDisplay();
    }

    void updateDisplay() {
        float freq = opCoarse;
        float fine = opFine;
        String txtFreq;

        if (opMode == 0) {
            if (freq == 0)
                freq = 0.5;
            txtFreq << "f = " << (freq + (freq * (fine / 100)));
        } else {
            freq = pow(10, ((int) freq) & 3);
            freq = freq * exp(M_LN10 * (fine / 100));
            txtFreq << freq << " Hz";
        }

        if (detune != 0) {
            if (detune > 0)
                txtFreq << " +" << detune;
            else
                txtFreq << " " << detune;
        }
        message = txtFreq;
        repaint();
    }

    void paint(juce::Graphics &g) override {
        g.setColour(juce::Colours::white);
        g.drawFittedText(message, 0, 0, getWidth(), getHeight(),
                         juce::Justification::centred, false);
    }
};