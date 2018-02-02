/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

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
    ScopedPointer<Slider> pitchRange;
    ScopedPointer<Slider> pitchStep;
    ScopedPointer<ComboBox> sysexIn;
    ScopedPointer<ComboBox> sysexOut;
    ScopedPointer<Slider> sysexChl;
    ScopedPointer<ComboBox> engineReso;
    ScopedPointer<ToggleButton> showKeyboard;
    ScopedPointer<Slider> whlRange;
    ScopedPointer<Slider> ftRange;
    ScopedPointer<Slider> brRange;
    ScopedPointer<Slider> atRange;
    ScopedPointer<ToggleButton> whlEg;
    ScopedPointer<ToggleButton> ftEg;
    ScopedPointer<ToggleButton> brEg;
    ScopedPointer<ToggleButton> atEg;
    ScopedPointer<ToggleButton> whlAmp;
    ScopedPointer<ToggleButton> ftAmp;
    ScopedPointer<ToggleButton> brAmp;
    ScopedPointer<ToggleButton> atAmp;
    ScopedPointer<ToggleButton> whlPitch;
    ScopedPointer<ToggleButton> ftPitch;
    ScopedPointer<ToggleButton> brPitch;
    ScopedPointer<ToggleButton> atPitch;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamDialog)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
