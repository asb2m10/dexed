/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_F21C2CC3FAD5D98E__
#define __JUCE_HEADER_F21C2CC3FAD5D98E__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OperatorEditor  : public Component,
                        public SliderListener,
                        public ButtonListener,
                        public ComboBoxListener
{
public:
    //==============================================================================
    OperatorEditor ();
    ~OperatorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void bind(DexedAudioProcessor *processor, int num);
    void updateGain(float v);

    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> s_egl1;
    ScopedPointer<ToggleButton> toggleButton;
    ScopedPointer<Slider> s_egl2;
    ScopedPointer<Slider> s_egl3;
    ScopedPointer<Slider> s_egl4;
    ScopedPointer<Slider> s_egv1;
    ScopedPointer<Slider> s_egv2;
    ScopedPointer<Slider> s_egv3;
    ScopedPointer<Slider> s_egv4;
    ScopedPointer<ComboBox> opMode;
    ScopedPointer<Label> opId;
    ScopedPointer<Slider> opLevel;
    ScopedPointer<Slider> opFine;
    ScopedPointer<Slider> opCoarse;
    ScopedPointer<Slider> gain;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OperatorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_F21C2CC3FAD5D98E__
