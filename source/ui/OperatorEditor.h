/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.9

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "component/DXComponents.h"
#include "component/VUMeter.h"
#include "component/EnvDisplay.h"
#include "util/AudioComponentContainer.h"
#include "component/ParameterObserver.h"



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/

class OperatorEditor  : public Component {
public:
    //==============================================================================
    OperatorEditor (DexedAudioProcessor &processor, int num);
    ~OperatorEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void bind(DexedAudioProcessor *processor, int num);
    void updateGain(float v);
    void updateDisplay();
    void updateEnvPos(char pos);
    void mouseDown(const MouseEvent& e) override;
    void toggleOpSwitch();
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    DexedAudioProcessor &processor;
    int internalOp;

    std::unique_ptr<juce::MouseListener> contextCallback;
    String opNum;
    Image light;
    Image background;
    std::unique_ptr<ToggleButton> opSwitch;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> khzDisplay;
    std::unique_ptr<EnvDisplay> envDisplay;
    std::unique_ptr<VuMeter> vu;

    std::unique_ptr<AudioComponentContainer> binder;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OperatorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

