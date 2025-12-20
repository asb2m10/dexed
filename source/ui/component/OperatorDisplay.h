#include <juce_audio_processors/juce_audio_processors.h>

class OperatorDisplay : public juce::Label {
    float opFine, opCoarse;
    int detune, opMode;

public:
    OperatorDisplay() {
        setFont(juce::Font(12.60f, juce::Font::plain).withTypefaceStyle("Regular"));
        setJustificationType(juce::Justification::centred);
        setEditable(false, false, false);
        setColour(juce::Label::backgroundColourId, juce::Colour(0x6a000000));
        setColour(juce::Label::textColourId, juce::Colours::white);
        setColour(juce::Label::outlineColourId, juce::Colour(0x00000000));
        setColour(juce::TextEditor::textColourId, juce::Colours::black);
        setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

        opFine = opCoarse = detune = opMode = 0;
    }

    void setOpFine(uint8_t val) {
        opFine = val;
        updateDisplay();
    }

    void setOpCoarse(uint8_t val) {
        opCoarse = val;
        updateDisplay();
    }

    void setDetune(uint8_t val) {
        detune = val;
        updateDisplay();
    }

    void setOpMode(uint8_t val) {
        opMode = val;
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
        setText(txtFreq, dontSendNotification);
        repaint();
    }
};