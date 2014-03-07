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
    s_egl1->setRange (0, 99, 1);
    s_egl1->setSliderStyle (Slider::Rotary);
    s_egl1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl1->addListener (this);

    addAndMakeVisible (s_egl2 = new Slider ("egl2"));
    s_egl2->setRange (0, 99, 1);
    s_egl2->setSliderStyle (Slider::Rotary);
    s_egl2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl2->addListener (this);

    addAndMakeVisible (s_egl3 = new Slider ("egl3"));
    s_egl3->setRange (0, 99, 1);
    s_egl3->setSliderStyle (Slider::Rotary);
    s_egl3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl3->addListener (this);

    addAndMakeVisible (s_egl4 = new Slider ("egl4"));
    s_egl4->setRange (0, 99, 1);
    s_egl4->setSliderStyle (Slider::Rotary);
    s_egl4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl4->addListener (this);

    addAndMakeVisible (s_egv1 = new Slider ("egr1"));
    s_egv1->setRange (0, 99, 1);
    s_egv1->setSliderStyle (Slider::Rotary);
    s_egv1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv1->addListener (this);

    addAndMakeVisible (s_egv2 = new Slider ("egr3"));
    s_egv2->setRange (0, 99, 1);
    s_egv2->setSliderStyle (Slider::Rotary);
    s_egv2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv2->addListener (this);

    addAndMakeVisible (s_egv3 = new Slider ("egr3"));
    s_egv3->setRange (0, 99, 1);
    s_egv3->setSliderStyle (Slider::Rotary);
    s_egv3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv3->addListener (this);

    addAndMakeVisible (s_egv4 = new Slider ("egr4"));
    s_egv4->setRange (0, 99, 1);
    s_egv4->setSliderStyle (Slider::Rotary);
    s_egv4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv4->addListener (this);

    addAndMakeVisible (opMode = new ComboBox ("opMode"));
    opMode->setEditableText (false);
    opMode->setJustificationType (Justification::centredLeft);
    opMode->setTextWhenNothingSelected (String::empty);
    opMode->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    opMode->addItem (TRANS("RATIO"), 1);
    opMode->addItem (TRANS("FIXED"), 2);
    opMode->addListener (this);

    addAndMakeVisible (opLevel = new Slider ("opLevel"));
    opLevel->setRange (0, 99, 1);
    opLevel->setSliderStyle (Slider::Rotary);
    opLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    opLevel->addListener (this);

    addAndMakeVisible (opFine = new Slider ("opFine"));
    opFine->setRange (0, 99, 1);
    opFine->setSliderStyle (Slider::Rotary);
    opFine->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    opFine->addListener (this);

    addAndMakeVisible (opCoarse = new Slider ("opCoarse"));
    opCoarse->setRange (0, 31, 1);
    opCoarse->setSliderStyle (Slider::Rotary);
    opCoarse->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    opCoarse->addListener (this);

    addAndMakeVisible (khzDisplay = new Label ("khz",
                                               TRANS("1,000 kHz")));
    khzDisplay->setFont (Font (11.00f, Font::plain));
    khzDisplay->setJustificationType (Justification::centred);
    khzDisplay->setEditable (false, false, false);
    khzDisplay->setColour (Label::backgroundColourId, Colour (0x6a000000));
    khzDisplay->setColour (Label::textColourId, Colours::white);
    khzDisplay->setColour (Label::outlineColourId, Colour (0x00000000));
    khzDisplay->setColour (TextEditor::textColourId, Colours::black);
    khzDisplay->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (detune = new Slider ("detune"));
    detune->setRange (-7, 7, 1);
    detune->setSliderStyle (Slider::LinearHorizontal);
    detune->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    detune->addListener (this);

    addAndMakeVisible (envDisplay = new EnvDisplay());
    envDisplay->setName ("envDisplay");

    addAndMakeVisible (sclLeftLevel = new Slider ("sclLeftLevel"));
    sclLeftLevel->setTooltip (TRANS("Keyboard Scale Level Left Depth "));
    sclLeftLevel->setRange (0, 99, 1);
    sclLeftLevel->setSliderStyle (Slider::Rotary);
    sclLeftLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sclLeftLevel->addListener (this);

    addAndMakeVisible (sclRightLevel = new Slider ("sclRightLevel"));
    sclRightLevel->setTooltip (TRANS("Keyboard Scale Level Right Depth "));
    sclRightLevel->setRange (0, 99, 1);
    sclRightLevel->setSliderStyle (Slider::Rotary);
    sclRightLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sclRightLevel->addListener (this);

    addAndMakeVisible (kbdLeftCurve = new ComboBox ("kbdLeftCurve"));
    kbdLeftCurve->setEditableText (false);
    kbdLeftCurve->setJustificationType (Justification::centredLeft);
    kbdLeftCurve->setTextWhenNothingSelected (String::empty);
    kbdLeftCurve->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    kbdLeftCurve->addItem (TRANS("-LN"), 1);
    kbdLeftCurve->addItem (TRANS("-EX"), 2);
    kbdLeftCurve->addItem (TRANS("+EX"), 3);
    kbdLeftCurve->addItem (TRANS("+LN"), 4);
    kbdLeftCurve->addListener (this);

    addAndMakeVisible (kbdRightCurve = new ComboBox ("kbdRightCurve"));
    kbdRightCurve->setEditableText (false);
    kbdRightCurve->setJustificationType (Justification::centredLeft);
    kbdRightCurve->setTextWhenNothingSelected (String::empty);
    kbdRightCurve->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    kbdRightCurve->addItem (TRANS("-LN"), 1);
    kbdRightCurve->addItem (TRANS("-EX"), 2);
    kbdRightCurve->addItem (TRANS("+EX"), 3);
    kbdRightCurve->addItem (TRANS("+LN"), 4);
    kbdRightCurve->addListener (this);

    addAndMakeVisible (sclLvlBrkPt = new Slider ("sclLvlBrkPt"));
    sclLvlBrkPt->setTooltip (TRANS("Scale Level Breakpoint"));
    sclLvlBrkPt->setRange (0, 99, 1);
    sclLvlBrkPt->setSliderStyle (Slider::Rotary);
    sclLvlBrkPt->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sclLvlBrkPt->addListener (this);

    addAndMakeVisible (sclRateScaling = new Slider ("sclRateScaling"));
    sclRateScaling->setTooltip (TRANS("Keyboard Rate Scaling"));
    sclRateScaling->setRange (0, 7, 1);
    sclRateScaling->setSliderStyle (Slider::Rotary);
    sclRateScaling->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sclRateScaling->addListener (this);

    addAndMakeVisible (keyVelSens = new Slider ("keyVelSens"));
    keyVelSens->setRange (0, 7, 1);
    keyVelSens->setSliderStyle (Slider::Rotary);
    keyVelSens->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    keyVelSens->addListener (this);

    addAndMakeVisible (ampModSens = new Slider ("ampModSens"));
    ampModSens->setRange (0, 4, 1);
    ampModSens->setSliderStyle (Slider::Rotary);
    ampModSens->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    ampModSens->addListener (this);

    addAndMakeVisible (vu = new VuMeter());
    vu->setName ("vu");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (280, 90);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

OperatorEditor::~OperatorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    s_egl1 = nullptr;
    s_egl2 = nullptr;
    s_egl3 = nullptr;
    s_egl4 = nullptr;
    s_egv1 = nullptr;
    s_egv2 = nullptr;
    s_egv3 = nullptr;
    s_egv4 = nullptr;
    opMode = nullptr;
    opLevel = nullptr;
    opFine = nullptr;
    opCoarse = nullptr;
    khzDisplay = nullptr;
    detune = nullptr;
    envDisplay = nullptr;
    sclLeftLevel = nullptr;
    sclRightLevel = nullptr;
    kbdLeftCurve = nullptr;
    kbdRightCurve = nullptr;
    sclLvlBrkPt = nullptr;
    sclRateScaling = nullptr;
    keyVelSens = nullptr;
    ampModSens = nullptr;
    vu = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OperatorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colour (0x32000000));
    g.fillRect (-5, -7, 293, 100);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OperatorEditor::resized()
{
    s_egl1->setBounds (136, 40, 24, 24);
    s_egl2->setBounds (160, 40, 24, 24);
    s_egl3->setBounds (184, 40, 24, 24);
    s_egl4->setBounds (208, 40, 24, 24);
    s_egv1->setBounds (136, 64, 24, 24);
    s_egv2->setBounds (160, 64, 24, 24);
    s_egv3->setBounds (184, 64, 24, 24);
    s_egv4->setBounds (208, 64, 24, 24);
    opMode->setBounds (24, 48, 104, 16);
    opLevel->setBounds (232, 56, 32, 32);
    opFine->setBounds (104, 24, 24, 24);
    opCoarse->setBounds (80, 24, 24, 24);
    khzDisplay->setBounds (32, 8, 88, 16);
    detune->setBounds (24, 24, 56, 24);
    envDisplay->setBounds (136, 5, 96, 32);
    sclLeftLevel->setBounds (0, 64, 24, 24);
    sclRightLevel->setBounds (64, 64, 24, 24);
    kbdLeftCurve->setBounds (24, 68, 40, 16);
    kbdRightCurve->setBounds (88, 68, 40, 16);
    sclLvlBrkPt->setBounds (0, 40, 24, 24);
    sclRateScaling->setBounds (0, 16, 24, 24);
    keyVelSens->setBounds (240, 24, 24, 24);
    ampModSens->setBounds (240, 0, 24, 24);
    vu->setBounds (268, 0, 12, 88);
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
        envDisplay->repaint();
        //[/UserSliderCode_s_egl1]
    }
    else if (sliderThatWasMoved == s_egl2)
    {
        //[UserSliderCode_s_egl2] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egl2]
    }
    else if (sliderThatWasMoved == s_egl3)
    {
        //[UserSliderCode_s_egl3] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egl3]
    }
    else if (sliderThatWasMoved == s_egl4)
    {
        //[UserSliderCode_s_egl4] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egl4]
    }
    else if (sliderThatWasMoved == s_egv1)
    {
        //[UserSliderCode_s_egv1] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egv1]
    }
    else if (sliderThatWasMoved == s_egv2)
    {
        //[UserSliderCode_s_egv2] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egv2]
    }
    else if (sliderThatWasMoved == s_egv3)
    {
        //[UserSliderCode_s_egv3] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egv3]
    }
    else if (sliderThatWasMoved == s_egv4)
    {
        //[UserSliderCode_s_egv4] -- add your slider handling code here..
        envDisplay->repaint();
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
        updateDisplay();
        //[/UserSliderCode_opFine]
    }
    else if (sliderThatWasMoved == opCoarse)
    {
        //[UserSliderCode_opCoarse] -- add your slider handling code here..
        updateDisplay();
        //[/UserSliderCode_opCoarse]
    }
    else if (sliderThatWasMoved == detune)
    {
        //[UserSliderCode_detune] -- add your slider handling code here..
        updateDisplay();
        //[/UserSliderCode_detune]
    }
    else if (sliderThatWasMoved == sclLeftLevel)
    {
        //[UserSliderCode_sclLeftLevel] -- add your slider handling code here..
        //[/UserSliderCode_sclLeftLevel]
    }
    else if (sliderThatWasMoved == sclRightLevel)
    {
        //[UserSliderCode_sclRightLevel] -- add your slider handling code here..
        //[/UserSliderCode_sclRightLevel]
    }
    else if (sliderThatWasMoved == sclLvlBrkPt)
    {
        //[UserSliderCode_sclLvlBrkPt] -- add your slider handling code here..
        //[/UserSliderCode_sclLvlBrkPt]
    }
    else if (sliderThatWasMoved == sclRateScaling)
    {
        //[UserSliderCode_sclRateScaling] -- add your slider handling code here..
        //[/UserSliderCode_sclRateScaling]
    }
    else if (sliderThatWasMoved == keyVelSens)
    {
        //[UserSliderCode_keyVelSens] -- add your slider handling code here..
        //[/UserSliderCode_keyVelSens]
    }
    else if (sliderThatWasMoved == ampModSens)
    {
        //[UserSliderCode_ampModSens] -- add your slider handling code here..
        //[/UserSliderCode_ampModSens]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void OperatorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == opMode)
    {
        //[UserComboBoxCode_opMode] -- add your combo box handling code here..
        updateDisplay();
        //[/UserComboBoxCode_opMode]
    }
    else if (comboBoxThatHasChanged == kbdLeftCurve)
    {
        //[UserComboBoxCode_kbdLeftCurve] -- add your combo box handling code here..
        //[/UserComboBoxCode_kbdLeftCurve]
    }
    else if (comboBoxThatHasChanged == kbdRightCurve)
    {
        //[UserComboBoxCode_kbdRightCurve] -- add your combo box handling code here..
        //[/UserComboBoxCode_kbdRightCurve]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OperatorEditor::bind(DexedAudioProcessor *parent, int op) {
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
    parent->opCtrl[op].detune->bind(detune);
    parent->opCtrl[op].sclBrkPt->bind(sclLvlBrkPt);
    parent->opCtrl[op].sclLeftCurve->bind(kbdLeftCurve);
    parent->opCtrl[op].sclRightCurve->bind(kbdRightCurve);
    parent->opCtrl[op].sclLeftDepth->bind(sclLeftLevel);
    parent->opCtrl[op].sclRightDepth->bind(sclRightLevel);
    parent->opCtrl[op].sclRate->bind(sclRateScaling);
    parent->opCtrl[op].ampModSens->bind(ampModSens);
    parent->opCtrl[op].velModSens->bind(keyVelSens);

    int offset = parent->opCtrl[op].egRate[0]->getOffset();
    envDisplay->pvalues = &(parent->data[offset]);
}

void OperatorEditor::updateGain(float v) {
    vu->v = v;
    vu->repaint();
}

void OperatorEditor::updateDisplay() {
    float freq = opCoarse->getValue();
    float fine = opFine->getValue();
    String txtFreq;

    if (opMode->getSelectedItemIndex() == 0) {
        if ( freq == 0 )
            freq = 0.5;
        txtFreq << "f = " << (freq + ((freq*2) * (fine/100)));
    } else {
        freq = pow(10,((int)freq)&3);
        freq = freq + ((freq*10) * (fine/100));
        txtFreq << freq << " Hz";
    }

    int det = detune->getValue();
    if ( det != 0 ) {
        if ( det > 0 )
            txtFreq << " +" << det;
        else
            txtFreq << " " << det;
    }
    khzDisplay->setText(txtFreq, NotificationType::dontSendNotification);
    envDisplay->repaint();
}


void OperatorEditor::updateEnvPos(char pos) {
    envDisplay->vPos = pos;
    envDisplay->repaint();
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
                 fixedSize="1" initialWidth="280" initialHeight="90">
  <BACKGROUND backgroundColour="ffffff">
    <RECT pos="-5 -7 293 100" fill="solid: 32000000" hasStroke="0"/>
  </BACKGROUND>
  <SLIDER name="egl1" id="dc070cc41347df47" memberName="s_egl1" virtualName=""
          explicitFocusOrder="0" pos="136 40 24 24" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egl2" id="66f5195e9c374029" memberName="s_egl2" virtualName=""
          explicitFocusOrder="0" pos="160 40 24 24" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egl3" id="9d57bd53203dcdb4" memberName="s_egl3" virtualName=""
          explicitFocusOrder="0" pos="184 40 24 24" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egl4" id="4f7c3ece3ea2cf9c" memberName="s_egl4" virtualName=""
          explicitFocusOrder="0" pos="208 40 24 24" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr1" id="2ca8137d80da46fb" memberName="s_egv1" virtualName=""
          explicitFocusOrder="0" pos="136 64 24 24" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr3" id="4ad6d0c532d15973" memberName="s_egv2" virtualName=""
          explicitFocusOrder="0" pos="160 64 24 24" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr3" id="8a2027f9ede16b4f" memberName="s_egv3" virtualName=""
          explicitFocusOrder="0" pos="184 64 24 24" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr4" id="8c04f1c943d837e8" memberName="s_egv4" virtualName=""
          explicitFocusOrder="0" pos="208 64 24 24" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="opMode" id="2cf8156bb94cdc40" memberName="opMode" virtualName=""
            explicitFocusOrder="0" pos="24 48 104 16" editable="0" layout="33"
            items="RATIO&#10;FIXED" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="opLevel" id="f8521c8214fb8993" memberName="opLevel" virtualName=""
          explicitFocusOrder="0" pos="232 56 32 32" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="opFine" id="e445aa61bd6cddcb" memberName="opFine" virtualName=""
          explicitFocusOrder="0" pos="104 24 24 24" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="opCoarse" id="4eec63d30d7488d2" memberName="opCoarse" virtualName=""
          explicitFocusOrder="0" pos="80 24 24 24" min="0" max="31" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <LABEL name="khz" id="eb961eed8902a6fc" memberName="khzDisplay" virtualName=""
         explicitFocusOrder="0" pos="32 8 88 16" bkgCol="6a000000" textCol="ffffffff"
         outlineCol="0" edTextCol="ff000000" edBkgCol="0" labelText="1,000 kHz"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="11" bold="0" italic="0" justification="36"/>
  <SLIDER name="detune" id="f093ec8defca2fc2" memberName="detune" virtualName=""
          explicitFocusOrder="0" pos="24 24 56 24" min="-7" max="7" int="1"
          style="LinearHorizontal" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <GENERICCOMPONENT name="envDisplay" id="b18856de924c6340" memberName="envDisplay"
                    virtualName="" explicitFocusOrder="0" pos="136 5 96 32" class="EnvDisplay"
                    params=""/>
  <SLIDER name="sclLeftLevel" id="bd6f338ae68e454f" memberName="sclLeftLevel"
          virtualName="" explicitFocusOrder="0" pos="0 64 24 24" tooltip="Keyboard Scale Level Left Depth "
          min="0" max="99" int="1" style="Rotary" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="sclRightLevel" id="b9e23ed5187fc7e5" memberName="sclRightLevel"
          virtualName="" explicitFocusOrder="0" pos="64 64 24 24" tooltip="Keyboard Scale Level Right Depth "
          min="0" max="99" int="1" style="Rotary" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <COMBOBOX name="kbdLeftCurve" id="a4b068d60be648ca" memberName="kbdLeftCurve"
            virtualName="" explicitFocusOrder="0" pos="24 68 40 16" editable="0"
            layout="33" items="-LN&#10;-EX&#10;+EX&#10;+LN" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="kbdRightCurve" id="e8b5c5cb5e36b46b" memberName="kbdRightCurve"
            virtualName="" explicitFocusOrder="0" pos="88 68 40 16" editable="0"
            layout="33" items="-LN&#10;-EX&#10;+EX&#10;+LN" textWhenNonSelected=""
            textWhenNoItems="(no choices)"/>
  <SLIDER name="sclLvlBrkPt" id="c563ac3116923bbc" memberName="sclLvlBrkPt"
          virtualName="" explicitFocusOrder="0" pos="0 40 24 24" tooltip="Scale Level Breakpoint"
          min="0" max="99" int="1" style="Rotary" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="sclRateScaling" id="f0d17c8e09aa4c49" memberName="sclRateScaling"
          virtualName="" explicitFocusOrder="0" pos="0 16 24 24" tooltip="Keyboard Rate Scaling"
          min="0" max="7" int="1" style="Rotary" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="keyVelSens" id="21795d045d07602b" memberName="keyVelSens"
          virtualName="" explicitFocusOrder="0" pos="240 24 24 24" min="0"
          max="7" int="1" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="ampModSens" id="634ceaa7b0f81a6c" memberName="ampModSens"
          virtualName="" explicitFocusOrder="0" pos="240 0 24 24" min="0"
          max="4" int="1" style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <GENERICCOMPONENT name="vu" id="6f952594ea99dc1e" memberName="vu" virtualName=""
                    explicitFocusOrder="0" pos="268 0 12 88" class="VuMeter" params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
