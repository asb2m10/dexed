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
                      public juce::Slider::Listener,
                      public juce::Button::Listener
{
public:
    //==============================================================================
    GlobalEditor ();
    ~GlobalEditor() override;

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
    //std::unique_ptr<Component> midiMonitor;

    void mouseDown(const MouseEvent& e) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    DexedAudioProcessorEditor *editor;
    DexedAudioProcessor *processor;

    Image background;
    Image imageLight;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> lfoSpeed;
    std::unique_ptr<juce::Slider> lfoAmDepth;
    std::unique_ptr<juce::Slider> lfoPitchDepth;
    std::unique_ptr<juce::Slider> lfoDelay;
    std::unique_ptr<juce::Slider> cutoff;
    std::unique_ptr<juce::Slider> reso;
    std::unique_ptr<juce::Slider> pitchRate2;
    std::unique_ptr<juce::Slider> pitchRate3;
    std::unique_ptr<juce::Slider> pitchRate4;
    std::unique_ptr<juce::Slider> pitchRate1;
    std::unique_ptr<juce::Slider> pitchLevel2;
    std::unique_ptr<juce::Slider> pitchLevel3;
    std::unique_ptr<juce::Slider> pitchLevel4;
    std::unique_ptr<juce::Slider> pitchLevel1;
    std::unique_ptr<juce::Slider> transpose;
    std::unique_ptr<juce::ToggleButton> oscSync;
    std::unique_ptr<juce::Slider> pitchModSens;
    std::unique_ptr<juce::ToggleButton> lfoSync;
    std::unique_ptr<PitchEnvDisplay> pitchEnvDisplay;
    std::unique_ptr<AlgoDisplay> algoDisplay;
    std::unique_ptr<juce::Slider> feedback;
    std::unique_ptr<juce::Slider> algo;
    std::unique_ptr<LcdDisplay> lcdDisplay;
    std::unique_ptr<juce::Slider> output;
    std::unique_ptr<VuMeter> vuOutput;
    std::unique_ptr<juce::TextButton> initButton;
    std::unique_ptr<juce::TextButton> parmButton;
    std::unique_ptr<juce::TextButton> cartButton;
    std::unique_ptr<juce::TextButton> storeButton;
    std::unique_ptr<juce::ToggleButton> monoMode;
    std::unique_ptr<ComboBoxImage> lfoType;
    std::unique_ptr<ProgramSelector> programSelector;
    std::unique_ptr<juce::ImageButton> aboutButton;
    std::unique_ptr<juce::Slider> tune;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

