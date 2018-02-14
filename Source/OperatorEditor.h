/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_F21C2CC3FAD5D98E__
#define __JUCE_HEADER_F21C2CC3FAD5D98E__

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
                        public Slider::Listener,
                        public Button::Listener
{
public:
    //==============================================================================
    OperatorEditor ();
    ~OperatorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void bind(DexedAudioProcessor *processor, int num);
    void updateGain(float v);
    void updateDisplay();
    void updateEnvPos(char pos);
    void mouseDown(const MouseEvent& e) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    String opNum;
    int internalOp;
    Image light;
    DexedAudioProcessor *processor;
    Image background;
    ScopedPointer<ToggleButton> opSwitch;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> s_egl1;
    ScopedPointer<Slider> s_egl2;
    ScopedPointer<Slider> s_egl3;
    ScopedPointer<Slider> s_egl4;
    ScopedPointer<Slider> s_egv1;
    ScopedPointer<Slider> s_egv2;
    ScopedPointer<Slider> s_egv3;
    ScopedPointer<Slider> s_egv4;
    ScopedPointer<Slider> opLevel;
    ScopedPointer<Slider> opFine;
    ScopedPointer<Slider> opCoarse;
    ScopedPointer<Label> khzDisplay;
    ScopedPointer<Slider> detune;
    ScopedPointer<EnvDisplay> envDisplay;
    ScopedPointer<Slider> sclLeftLevel;
    ScopedPointer<Slider> sclRightLevel;
    ScopedPointer<Slider> sclLvlBrkPt;
    ScopedPointer<Slider> sclRateScaling;
    ScopedPointer<Slider> keyVelSens;
    ScopedPointer<Slider> ampModSens;
    ScopedPointer<VuMeter> vu;
    ScopedPointer<ToggleButton> opMode;
    ScopedPointer<ComboBoxImage> kbdLeftCurve;
    ScopedPointer<ComboBoxImage> kbdRightCurve;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OperatorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_F21C2CC3FAD5D98E__
