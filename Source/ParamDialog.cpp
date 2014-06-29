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
#include "Dexed.h"
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

    addAndMakeVisible (sysexIn = new ComboBox ("sysexIn"));
    sysexIn->setEditableText (false);
    sysexIn->setJustificationType (Justification::centredLeft);
    sysexIn->setTextWhenNothingSelected (String::empty);
    sysexIn->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    sysexIn->addListener (this);

    addAndMakeVisible (sysexOut = new ComboBox ("sysexOut"));
    sysexOut->setEditableText (false);
    sysexOut->setJustificationType (Justification::centredLeft);
    sysexOut->setTextWhenNothingSelected (String::empty);
    sysexOut->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    sysexOut->addListener (this);

    addAndMakeVisible (sysexChl = new Slider ("sysexChl"));
    sysexChl->setRange (1, 16, 1);
    sysexChl->setSliderStyle (Slider::Rotary);
    sysexChl->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sysexChl->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (280, 300);


    //[Constructor] You can add your own custom stuff here..
    pitchRange->setEnabled(pitchStep->getValue() == 0);

    StringArray input;
    input.add("None");
    input.addArray(MidiInput::getDevices());
    sysexIn->addItemList(input, 2);

    StringArray output;
    output.add("None");
    output.addArray(MidiOutput::getDevices());
    sysexOut->addItemList(output, 2);

    //[/Constructor]
}

ParamDialog::~ParamDialog()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    pitchRange = nullptr;
    pitchStep = nullptr;
    sysexIn = nullptr;
    sysexOut = nullptr;
    sysexChl = nullptr;


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
                19, 21, 205, 23,
                Justification::centredLeft, true);

    g.setColour (Colours::white);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("Pitch Bend Step"),
                19, 61, 229, 23,
                Justification::centredLeft, true);

    g.setColour (Colours::white);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("Sysex In"),
                19, 178, 131, 23,
                Justification::centredLeft, true);

    g.setColour (Colours::white);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("Sysex Out"),
                19, 218, 131, 23,
                Justification::centredLeft, true);

    g.setColour (Colours::white);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("Sysex Channel"),
                19, 258, 245, 23,
                Justification::centredLeft, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ParamDialog::resized()
{
    pitchRange->setBounds (192, 16, 72, 24);
    pitchStep->setBounds (192, 56, 72, 24);
    sysexIn->setBounds (104, 176, 152, 24);
    sysexOut->setBounds (104, 216, 152, 24);
    sysexChl->setBounds (184, 256, 72, 24);
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
        pitchRange->setEnabled(pitchStep->getValue() == 0);
        //[/UserSliderCode_pitchStep]
    }
    else if (sliderThatWasMoved == sysexChl)
    {
        //[UserSliderCode_sysexChl] -- add your slider handling code here..
        //[/UserSliderCode_sysexChl]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void ParamDialog::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == sysexIn)
    {
        //[UserComboBoxCode_sysexIn] -- add your combo box handling code here..
        //[/UserComboBoxCode_sysexIn]
    }
    else if (comboBoxThatHasChanged == sysexOut)
    {
        //[UserComboBoxCode_sysexOut] -- add your combo box handling code here..
        //[/UserComboBoxCode_sysexOut]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ParamDialog::setDialogValues(Controllers &c, SysexComm &mgr) {
    pitchRange->setValue(c.values_[kControllerPitchRange]);
    pitchStep->setValue(c.values_[kControllerPitchStep]);
    sysexChl->setValue(mgr.getChl() + 1);

    StringArray inputs = MidiInput::getDevices();
    int idx = inputs.indexOf(mgr.getInput());
    idx = idx == -1 ? 0 : idx + 1;
    sysexIn->setSelectedItemIndex(idx);

    StringArray outputs = MidiOutput::getDevices();
    idx = outputs.indexOf(mgr.getOutput());
    TRACE("output idx %d output %s", idx, mgr.getOutput().toRawUTF8());
    idx = idx == -1 ? 0 : idx + 1;
    sysexOut->setSelectedItemIndex(idx);
}

void ParamDialog::getDialogValues(Controllers &c, SysexComm &mgr) {
    c.values_[kControllerPitchRange] = pitchRange->getValue();
    c.values_[kControllerPitchStep] = pitchStep->getValue();
    mgr.setInput(sysexIn->getItemText(sysexIn->getSelectedItemIndex()));
    mgr.setOutput(sysexOut->getItemText(sysexOut->getSelectedItemIndex()));
    mgr.setChl(sysexChl->getValue() - 1);
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
                 fixedSize="1" initialWidth="280" initialHeight="300">
  <BACKGROUND backgroundColour="ff4e270d">
    <TEXT pos="19 21 205 23" fill="solid: ffffffff" hasStroke="0" text="Pitch Bend Range"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
    <TEXT pos="19 61 229 23" fill="solid: ffffffff" hasStroke="0" text="Pitch Bend Step"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
    <TEXT pos="19 178 131 23" fill="solid: ffffffff" hasStroke="0" text="Sysex In"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
    <TEXT pos="19 218 131 23" fill="solid: ffffffff" hasStroke="0" text="Sysex Out"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
    <TEXT pos="19 258 245 23" fill="solid: ffffffff" hasStroke="0" text="Sysex Channel"
          fontname="Default font" fontsize="15" bold="0" italic="0" justification="33"/>
  </BACKGROUND>
  <SLIDER name="pitchRange" id="7409be5a8dfaa91" memberName="pitchRange"
          virtualName="" explicitFocusOrder="0" pos="192 16 72 24" min="0"
          max="12" int="1" style="Rotary" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchStep" id="b86af4b792e768ca" memberName="pitchStep"
          virtualName="" explicitFocusOrder="0" pos="192 56 72 24" min="0"
          max="12" int="1" style="Rotary" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="sysexIn" id="3750642d8b5be11" memberName="sysexIn" virtualName=""
            explicitFocusOrder="0" pos="104 176 152 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="sysexOut" id="44730115841c2214" memberName="sysexOut" virtualName=""
            explicitFocusOrder="0" pos="104 216 152 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="sysexChl" id="7fdc8830f90a7c86" memberName="sysexChl" virtualName=""
          explicitFocusOrder="0" pos="184 256 72 24" min="1" max="16" int="1"
          style="Rotary" textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
