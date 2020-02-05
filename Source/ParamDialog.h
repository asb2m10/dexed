/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "msfa/controllers.h"
#include "SysexComm.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ParamDialog  : public Component,
                     public Slider::Listener,
                     public ComboBox::Listener,
                     public Button::Listener
{
public:
    //==============================================================================
    ParamDialog ();
    ~ParamDialog();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setDialogValues(Controllers &c, SysexComm &mgr, int reso, bool showKeyboard);
    bool getDialogValues(Controllers &c, SysexComm &mgr, int *reso, bool *showKeyboard);
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<Slider> pitchRange;
    std::unique_ptr<Slider> pitchStep;
    std::unique_ptr<ComboBox> sysexIn;
    std::unique_ptr<ComboBox> sysexOut;
    std::unique_ptr<Slider> sysexChl;
    std::unique_ptr<ComboBox> engineReso;
    std::unique_ptr<ToggleButton> showKeyboard;
    std::unique_ptr<Slider> whlRange;
    std::unique_ptr<Slider> ftRange;
    std::unique_ptr<Slider> brRange;
    std::unique_ptr<Slider> atRange;
    std::unique_ptr<ToggleButton> whlEg;
    std::unique_ptr<ToggleButton> ftEg;
    std::unique_ptr<ToggleButton> brEg;
    std::unique_ptr<ToggleButton> atEg;
    std::unique_ptr<ToggleButton> whlAmp;
    std::unique_ptr<ToggleButton> ftAmp;
    std::unique_ptr<ToggleButton> brAmp;
    std::unique_ptr<ToggleButton> atAmp;
    std::unique_ptr<ToggleButton> whlPitch;
    std::unique_ptr<ToggleButton> ftPitch;
    std::unique_ptr<ToggleButton> brPitch;
    std::unique_ptr<ToggleButton> atPitch;
    std::unique_ptr<TextButton> sclButton;
    std::unique_ptr<TextButton> kbmButton;
    std::unique_ptr<TextButton> showTunButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamDialog)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

