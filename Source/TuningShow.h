/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
#include "Tunings.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class TuningShow  : public Component
{
public:
    //==============================================================================
    TuningShow ();
    ~TuningShow() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setTuning( const Tunings::Tuning &t );
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    Tunings::Tuning tuning;
    TextEditor *sclt, *kbmt;
    TableListBox *table;
    std::unique_ptr<surgesynthteam::TuningTableListBoxModel> mod;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<TabbedComponent> tabbedComponent;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TuningShow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

