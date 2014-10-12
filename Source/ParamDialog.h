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

#ifndef __JUCE_HEADER_D00F97AD887A66__
#define __JUCE_HEADER_D00F97AD887A66__

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
                     public SliderListener,
                     public ComboBoxListener
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

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);



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


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamDialog)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_D00F97AD887A66__
