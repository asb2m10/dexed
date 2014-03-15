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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "ParamDialog.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ParamDialog::ParamDialog ()
{
    addAndMakeVisible (pitchRange = new Slider ("pitchRange"));
    pitchRange->setRange (0, 12, 1);
    pitchRange->setSliderStyle (Slider::Rotary);
    pitchRange->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    pitchRange->addListener (this);

    addAndMakeVisible (pitchStep = new Slider ("pitchStep"));
    pitchStep->setRange (0, 12, 1);
    pitchStep->setSliderStyle (Slider::Rotary);
    pitchStep->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    pitchStep->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (280, 200);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ParamDialog::~ParamDialog()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    pitchRange = nullptr;
    pitchStep = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ParamDialog::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff4e270d));

    g.setColour (Colours::white);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("Pitch Bend Range"),
                28, 20, 131, 23,
                Justification::centredLeft, true);

    g.setColour (Colours::white);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("Pitch Bend Step"),
                28, 52, 128, 23,
                Justification::centredLeft, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ParamDialog::resized()
{
    pitchRange->setBounds (184, 16, 72, 24);
    pitchStep->setBounds (184, 56, 72, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ParamDialog::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == pitchRange)
    {
        //[UserSliderCode_pitchRange] -- add your slider handling code here..
        //[/UserSliderCode_pitchRange]
    }
    else if (sliderThatWasMoved == pitchStep)
    {
        //[UserSliderCode_pitchStep] -- add your slider handling code here..
        //[/UserSliderCode_pitchStep]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ParamDialog::setDialogValues(Controllers &c) {
    pitchRange->setValue(c.values_[kControllerPitchRange]);
    pitchStep->setValue(c.values_[kControllerPitchStep]);
}

void ParamDialog::getDialogValues(Controllers &c) {
    c.values_[kControllerPitchRange] = pitchRange->getValue();
    c.values_[kControllerPitchStep] = pitchStep->getValue();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ParamDialog" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="280" initialHeight="200">
  <BACKGROUND backgroundColour="ff4e270d">
    <TEXT pos="28 20 131 23" fill="solid: ffffffff" hasStroke="0" text="Pitch Bend Range"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
    <TEXT pos="28 52 128 23" fill="solid: ffffffff" hasStroke="0" text="Pitch Bend Step"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  </BACKGROUND>
  <SLIDER name="pitchRange" id="7409be5a8dfaa91" memberName="pitchRange"
          virtualName="" explicitFocusOrder="0" pos="184 16 72 24" min="0"
          max="12" int="1" style="Rotary" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchStep" id="b86af4b792e768ca" memberName="pitchStep"
          virtualName="" explicitFocusOrder="0" pos="184 56 72 24" min="0"
          max="12" int="1" style="Rotary" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
