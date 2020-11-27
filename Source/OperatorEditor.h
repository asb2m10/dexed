/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "DXComponents.h"
#include "DXLookNFeel.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OperatorEditor  : public Component,
                        public juce::Slider::Listener,
                        public juce::Button::Listener
{
public:
    //==============================================================================
    OperatorEditor ();
    ~OperatorEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void bind(DexedAudioProcessor *processor, int num);
    void updateGain(float v);
    void updateDisplay();
    void updateEnvPos(char pos);
    void mouseDown(const MouseEvent& e) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    String opNum;
    int internalOp;
    Image light;
    DexedAudioProcessor *processor;
    Image background;
    std::unique_ptr<ToggleButton> opSwitch;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> s_egl1;
    std::unique_ptr<juce::Slider> s_egl2;
    std::unique_ptr<juce::Slider> s_egl3;
    std::unique_ptr<juce::Slider> s_egl4;
    std::unique_ptr<juce::Slider> s_egv1;
    std::unique_ptr<juce::Slider> s_egv2;
    std::unique_ptr<juce::Slider> s_egv3;
    std::unique_ptr<juce::Slider> s_egv4;
    std::unique_ptr<juce::Slider> opLevel;
    std::unique_ptr<juce::Slider> opFine;
    std::unique_ptr<juce::Slider> opCoarse;
    std::unique_ptr<juce::Label> khzDisplay;
    std::unique_ptr<juce::Slider> detune;
    std::unique_ptr<EnvDisplay> envDisplay;
    std::unique_ptr<juce::Slider> sclLeftLevel;
    std::unique_ptr<juce::Slider> sclRightLevel;
    std::unique_ptr<juce::Slider> sclLvlBrkPt;
    std::unique_ptr<juce::Slider> sclRateScaling;
    std::unique_ptr<juce::Slider> keyVelSens;
    std::unique_ptr<juce::Slider> ampModSens;
    std::unique_ptr<VuMeter> vu;
    std::unique_ptr<juce::ToggleButton> opMode;
    std::unique_ptr<ComboBoxImage> kbdLeftCurve;
    std::unique_ptr<ComboBoxImage> kbdRightCurve;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OperatorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

