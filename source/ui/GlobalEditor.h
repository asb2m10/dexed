/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.11

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "PluginProcessor.h"
#include "component/DXComponents.h"
#include "component/AlgoDisplay.h"
#include "util/AudioComponentContainer.h"

#ifdef IMPLEMENT_MidiMonitor
#include "SysexComm.h"
#endif // IMPLEMENT_MidiMonitor


class GlobalEditor  : public Component,
                      public juce::Slider::Listener,
                      public juce::Button::Listener
{
public:
    //==============================================================================
    GlobalEditor (DexedAudioProcessor &processor);
    ~GlobalEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setSystemMessage(String msg);
    void setParamMessage(String msg);
    void updatePitchPos(int pos);
    void updateVu(float v);
    void updateDisplay();
    ProgramSelector *programs;

#ifdef IMPLEMENT_MidiMonitor
    std::unique_ptr<Component> midiMonitor;
#endif //IMPLEMENT_MidiMonitor

    void mouseDown(const MouseEvent& e) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

    // TODO: put accessor methods here to get to buttons/sliders if needed
    std::unique_ptr<juce::TextButton> initButton;
    std::unique_ptr<juce::TextButton> parmButton;
    std::unique_ptr<juce::TextButton> cartButton;
    std::unique_ptr<juce::TextButton> storeButton;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    DexedAudioProcessor &processor;

    Image background;
    Image imageLight;

    std::unique_ptr<Component> aboutBox;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> transpose;
    std::unique_ptr<juce::ToggleButton> oscSync;
    std::unique_ptr<juce::Slider> pitchModSens;
    std::unique_ptr<juce::ToggleButton> lfoSync;
    std::unique_ptr<PitchEnvDisplay> pitchEnvDisplay;
    std::unique_ptr<AlgoDisplay> algoDisplay;
    std::unique_ptr<juce::Slider> feedback;
    std::unique_ptr<LcdDisplay> lcdDisplay;
    std::unique_ptr<juce::Slider> output;
    std::unique_ptr<VuMeterOutput> vuOutput;
    std::unique_ptr<juce::ToggleButton> monoMode;
    std::unique_ptr<ComboBoxImage> lfoType;
    std::unique_ptr<ProgramSelector> programSelector;
    std::unique_ptr<juce::ImageButton> aboutButton;
    std::unique_ptr<juce::Slider> tune;

    std::unique_ptr<AudioComponentContainer> binder;

    std::unique_ptr<juce::DocumentWindow> debugger;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

