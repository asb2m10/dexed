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

#ifndef __JUCE_HEADER_10638A2B49B704D0__
#define __JUCE_HEADER_10638A2B49B704D0__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "DXLookNFeel.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GlobalEditor  : public Component,
                      public SliderListener,
                      public ComboBoxListener
{
public:
    //==============================================================================
    GlobalEditor ();
    ~GlobalEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

    void bind(DexedAudioProcessor *processor);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    DexedAudioProcessor *processor;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> algo;
    ScopedPointer<ComboBox> lfoType;
    ScopedPointer<Slider> lfoSpeed;
    ScopedPointer<Slider> lfoAmDepth;
    ScopedPointer<Slider> lfoPitchDepth;
    ScopedPointer<Slider> lfoDelay;
    ScopedPointer<Slider> cutoff;
    ScopedPointer<Slider> reso;
    ScopedPointer<AlgoDisplay> algoDisplay;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_10638A2B49B704D0__
