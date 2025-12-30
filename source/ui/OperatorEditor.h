#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "component/VUMeter.h"
#include "component/EnvDisplay.h"
#include "util/AudioComponentContainer.h"

class OperatorEditor : public Component {
public:
    OperatorEditor (DexedAudioProcessor &processor, int num);
    ~OperatorEditor() override;

    void updateGain(float v);
    void updateDisplay();
    void updateEnvPos(char pos);
    void mouseDown(const MouseEvent& e) override;
    void toggleOpSwitch();

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    DexedAudioProcessor &processor;
    int internalOp;

    String opNum;
    Image light;
    Image background;

    std::unique_ptr<EnvDisplay> envDisplay;
    std::unique_ptr<ToggleButton> opSwitch;
    std::unique_ptr<ToggleButton> opMode;
    std::unique_ptr<VuMeter> vu;
    std::unique_ptr<juce::MouseListener> contextMenuAdapter;
    std::unique_ptr<AudioComponentContainer> binder;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OperatorEditor)
};
