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

#ifndef __JUCE_HEADER_10638A2B49B704D0__
#define __JUCE_HEADER_10638A2B49B704D0__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "DXComponents.h"
#include "AlgoDisplay.h"

class DexedAudioProcessorEditor;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GlobalEditor  : public Component,
                      public Slider::Listener,
                      public Button::Listener
{
public:
    //==============================================================================
    GlobalEditor ();
    ~GlobalEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void bind(DexedAudioProcessorEditor *processor);
    void setSystemMessage(String msg);
    void setParamMessage(String msg);
    void updatePitchPos(int pos);
    void updateVu(float v);
    void updateDisplay();
    void repaintMsg();

    void setMonoState(bool state);
    ProgramSelector *programs;
    ScopedPointer<Component> midiMonitor;

    void mouseDown(const MouseEvent& e) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    DexedAudioProcessorEditor *editor;
    DexedAudioProcessor *processor;
    
    Image background;
    Image imageLight;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> lfoSpeed;
    ScopedPointer<Slider> lfoAmDepth;
    ScopedPointer<Slider> lfoPitchDepth;
    ScopedPointer<Slider> lfoDelay;
    ScopedPointer<Slider> cutoff;
    ScopedPointer<Slider> reso;
    ScopedPointer<Slider> pitchRate2;
    ScopedPointer<Slider> pitchRate3;
    ScopedPointer<Slider> pitchRate4;
    ScopedPointer<Slider> pitchRate1;
    ScopedPointer<Slider> pitchLevel2;
    ScopedPointer<Slider> pitchLevel3;
    ScopedPointer<Slider> pitchLevel4;
    ScopedPointer<Slider> pitchLevel1;
    ScopedPointer<Slider> transpose;
    ScopedPointer<ToggleButton> oscSync;
    ScopedPointer<Slider> pitchModSens;
    ScopedPointer<ToggleButton> lfoSync;
    ScopedPointer<PitchEnvDisplay> pitchEnvDisplay;
    ScopedPointer<AlgoDisplay> algoDisplay;
    ScopedPointer<Slider> feedback;
    ScopedPointer<Slider> algo;
    ScopedPointer<LcdDisplay> lcdDisplay;
    ScopedPointer<Slider> output;
    ScopedPointer<VuMeter> vuOutput;
    ScopedPointer<TextButton> initButton;
    ScopedPointer<TextButton> parmButton;
    ScopedPointer<TextButton> cartButton;
    ScopedPointer<TextButton> storeButton;
    ScopedPointer<ToggleButton> monoMode;
    ScopedPointer<ComboBoxImage> lfoType;
    ScopedPointer<ProgramSelector> programSelector;
    ScopedPointer<ImageButton> aboutButton;
    ScopedPointer<Slider> tune;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_10638A2B49B704D0__
