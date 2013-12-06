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

#include "OperatorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OperatorEditor::OperatorEditor ()
{
    addAndMakeVisible (s_egl1 = new Slider ("egl1"));
    s_egl1->setRange (0, 10, 0);
    s_egl1->setSliderStyle (Slider::Rotary);
    s_egl1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl1->addListener (this);

    addAndMakeVisible (toggleButton = new ToggleButton ("new toggle button"));
    toggleButton->setButtonText (String::empty);
    toggleButton->addListener (this);

    addAndMakeVisible (s_egl2 = new Slider ("egl2"));
    s_egl2->setRange (0, 10, 0);
    s_egl2->setSliderStyle (Slider::Rotary);
    s_egl2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl2->addListener (this);

    addAndMakeVisible (s_egl3 = new Slider ("egl3"));
    s_egl3->setRange (0, 10, 0);
    s_egl3->setSliderStyle (Slider::Rotary);
    s_egl3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl3->addListener (this);

    addAndMakeVisible (s_egl4 = new Slider ("egl4"));
    s_egl4->setRange (0, 10, 0);
    s_egl4->setSliderStyle (Slider::Rotary);
    s_egl4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl4->addListener (this);

    addAndMakeVisible (s_egv1 = new Slider ("egr1"));
    s_egv1->setRange (0, 10, 0);
    s_egv1->setSliderStyle (Slider::Rotary);
    s_egv1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv1->addListener (this);

    addAndMakeVisible (s_egv2 = new Slider ("egr3"));
    s_egv2->setRange (0, 10, 0);
    s_egv2->setSliderStyle (Slider::Rotary);
    s_egv2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv2->addListener (this);

    addAndMakeVisible (s_egv3 = new Slider ("egr3"));
    s_egv3->setRange (0, 10, 0);
    s_egv3->setSliderStyle (Slider::Rotary);
    s_egv3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv3->addListener (this);

    addAndMakeVisible (s_egv4 = new Slider ("egr4"));
    s_egv4->setRange (0, 10, 0);
    s_egv4->setSliderStyle (Slider::Rotary);
    s_egv4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv4->addListener (this);

    addAndMakeVisible (opMode = new ComboBox ("opMode"));
    opMode->setEditableText (false);
    opMode->setJustificationType (Justification::centredLeft);
    opMode->setTextWhenNothingSelected (String::empty);
    opMode->setTextWhenNoChoicesAvailable ("(no choices)");
    opMode->addItem ("FIXED", 1);
    opMode->addItem ("COARSE", 2);
    opMode->addListener (this);

    addAndMakeVisible (opId = new Label ("new label",
                                         "OP1"));
    opId->setFont (Font (19.60f, Font::plain));
    opId->setJustificationType (Justification::centredLeft);
    opId->setEditable (false, false, false);
    opId->setColour (TextEditor::textColourId, Colours::black);
    opId->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (opLevel = new Slider ("opLevel"));
    opLevel->setRange (0, 10, 0);
    opLevel->setSliderStyle (Slider::Rotary);
    opLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    opLevel->addListener (this);

    addAndMakeVisible (opFine = new Slider ("opFine"));
    opFine->setRange (0, 10, 0);
    opFine->setSliderStyle (Slider::Rotary);
    opFine->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    opFine->addListener (this);

    addAndMakeVisible (opCoarse = new Slider ("opCoarse"));
    opCoarse->setRange (0, 10, 0);
    opCoarse->setSliderStyle (Slider::Rotary);
    opCoarse->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    opCoarse->addListener (this);

    addAndMakeVisible (gain = new Slider ("new slider"));
    gain->setRange (0, 1, 0);
    gain->setSliderStyle (Slider::LinearVertical);
    gain->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    gain->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (250, 70);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

OperatorEditor::~OperatorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    s_egl1 = nullptr;
    toggleButton = nullptr;
    s_egl2 = nullptr;
    s_egl3 = nullptr;
    s_egl4 = nullptr;
    s_egv1 = nullptr;
    s_egv2 = nullptr;
    s_egv3 = nullptr;
    s_egv4 = nullptr;
    opMode = nullptr;
    opId = nullptr;
    opLevel = nullptr;
    opFine = nullptr;
    opCoarse = nullptr;
    gain = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OperatorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OperatorEditor::resized()
{
    s_egl1->setBounds (128, 8, 24, 24);
    toggleButton->setBounds (0, 0, 24, 24);
    s_egl2->setBounds (152, 8, 24, 24);
    s_egl3->setBounds (176, 8, 24, 24);
    s_egl4->setBounds (200, 8, 24, 24);
    s_egv1->setBounds (128, 40, 24, 24);
    s_egv2->setBounds (152, 40, 24, 24);
    s_egv3->setBounds (176, 40, 24, 24);
    s_egv4->setBounds (200, 40, 24, 24);
    opMode->setBounds (48, 8, 72, 24);
    opId->setBounds (0, 8, 47, 24);
    opLevel->setBounds (8, 40, 24, 24);
    opFine->setBounds (96, 40, 24, 24);
    opCoarse->setBounds (72, 40, 24, 24);
    gain->setBounds (224, 0, 24, 64);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OperatorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == s_egl1)
    {
        //[UserSliderCode_s_egl1] -- add your slider handling code here..
        //[/UserSliderCode_s_egl1]
    }
    else if (sliderThatWasMoved == s_egl2)
    {
        //[UserSliderCode_s_egl2] -- add your slider handling code here..
        //[/UserSliderCode_s_egl2]
    }
    else if (sliderThatWasMoved == s_egl3)
    {
        //[UserSliderCode_s_egl3] -- add your slider handling code here..
        //[/UserSliderCode_s_egl3]
    }
    else if (sliderThatWasMoved == s_egl4)
    {
        //[UserSliderCode_s_egl4] -- add your slider handling code here..
        //[/UserSliderCode_s_egl4]
    }
    else if (sliderThatWasMoved == s_egv1)
    {
        //[UserSliderCode_s_egv1] -- add your slider handling code here..
        //[/UserSliderCode_s_egv1]
    }
    else if (sliderThatWasMoved == s_egv2)
    {
        //[UserSliderCode_s_egv2] -- add your slider handling code here..
        //[/UserSliderCode_s_egv2]
    }
    else if (sliderThatWasMoved == s_egv3)
    {
        //[UserSliderCode_s_egv3] -- add your slider handling code here..
        //[/UserSliderCode_s_egv3]
    }
    else if (sliderThatWasMoved == s_egv4)
    {
        //[UserSliderCode_s_egv4] -- add your slider handling code here..
        //[/UserSliderCode_s_egv4]
    }
    else if (sliderThatWasMoved == opLevel)
    {
        //[UserSliderCode_opLevel] -- add your slider handling code here..
        //[/UserSliderCode_opLevel]
    }
    else if (sliderThatWasMoved == opFine)
    {
        //[UserSliderCode_opFine] -- add your slider handling code here..
        //[/UserSliderCode_opFine]
    }
    else if (sliderThatWasMoved == opCoarse)
    {
        //[UserSliderCode_opCoarse] -- add your slider handling code here..
        //[/UserSliderCode_opCoarse]
    }
    else if (sliderThatWasMoved == gain)
    {
        //[UserSliderCode_gain] -- add your slider handling code here..
        //[/UserSliderCode_gain]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void OperatorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleButton)
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..
        //[/UserButtonCode_toggleButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void OperatorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == opMode)
    {
        //[UserComboBoxCode_opMode] -- add your combo box handling code here..
        //[/UserComboBoxCode_opMode]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void OperatorEditor::bind(DexedAudioProcessor *parent, int op) {
  // TODO: set operator number

	int targetNum = op+1;

    String text = "op";
    text << "OP";
    text << targetNum;

    String opName;
    opName << "OP" << targetNum;

	opId->setText(opName, NotificationType::dontSendNotification);

	parent->opCtrl[op].egLevel[0]->bind(s_egl1);
	parent->opCtrl[op].egLevel[1]->bind(s_egl2);
	parent->opCtrl[op].egLevel[2]->bind(s_egl3);
	parent->opCtrl[op].egLevel[3]->bind(s_egl4);
	parent->opCtrl[op].egRate[0]->bind(s_egv1);
	parent->opCtrl[op].egRate[1]->bind(s_egv2);
	parent->opCtrl[op].egRate[2]->bind(s_egv3);
	parent->opCtrl[op].egRate[3]->bind(s_egv4);
	parent->opCtrl[op].level->bind(opLevel);
	parent->opCtrl[op].opMode->bind(opMode);
	parent->opCtrl[op].fine->bind(opFine);
	parent->opCtrl[op].coarse->bind(opCoarse);
}


void OperatorEditor::updateGain(float v) {
    gain->setValue(v);

}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OperatorEditor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="250" initialHeight="70">
  <BACKGROUND backgroundColour="ffffff"/>
  <SLIDER name="egl1" id="dc070cc41347df47" memberName="s_egl1" virtualName=""
          explicitFocusOrder="0" pos="128 8 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="new toggle button" id="fcdf1076330e4ef6" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="0 0 24 24" buttonText=""
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="egl2" id="66f5195e9c374029" memberName="s_egl2" virtualName=""
          explicitFocusOrder="0" pos="152 8 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egl3" id="9d57bd53203dcdb4" memberName="s_egl3" virtualName=""
          explicitFocusOrder="0" pos="176 8 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egl4" id="4f7c3ece3ea2cf9c" memberName="s_egl4" virtualName=""
          explicitFocusOrder="0" pos="200 8 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr1" id="2ca8137d80da46fb" memberName="s_egv1" virtualName=""
          explicitFocusOrder="0" pos="128 40 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr3" id="4ad6d0c532d15973" memberName="s_egv2" virtualName=""
          explicitFocusOrder="0" pos="152 40 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr3" id="8a2027f9ede16b4f" memberName="s_egv3" virtualName=""
          explicitFocusOrder="0" pos="176 40 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr4" id="8c04f1c943d837e8" memberName="s_egv4" virtualName=""
          explicitFocusOrder="0" pos="200 40 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="opMode" id="2cf8156bb94cdc40" memberName="opMode" virtualName=""
            explicitFocusOrder="0" pos="48 8 72 24" editable="0" layout="33"
            items="FIXED&#10;COARSE" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="75765097f6c5c142" memberName="opId" virtualName=""
         explicitFocusOrder="0" pos="0 8 47 24" edTextCol="ff000000" edBkgCol="0"
         labelText="OP1" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="19.600000000000001421"
         bold="0" italic="0" justification="33"/>
  <SLIDER name="opLevel" id="f8521c8214fb8993" memberName="opLevel" virtualName=""
          explicitFocusOrder="0" pos="8 40 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="opFine" id="e445aa61bd6cddcb" memberName="opFine" virtualName=""
          explicitFocusOrder="0" pos="96 40 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="opCoarse" id="4eec63d30d7488d2" memberName="opCoarse" virtualName=""
          explicitFocusOrder="0" pos="72 40 24 24" min="0" max="10" int="0"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="new slider" id="21f21cc5fae8e54b" memberName="gain" virtualName=""
          explicitFocusOrder="0" pos="224 0 24 64" min="0" max="1" int="0"
          style="LinearVertical" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
