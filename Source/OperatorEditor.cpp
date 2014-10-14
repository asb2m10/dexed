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
    s_egl1->setSliderStyle (Slider::RotaryVerticalDrag);
    s_egl1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl1->addListener (this);

    addAndMakeVisible (s_egl2 = new Slider ("egl2"));
    s_egl2->setRange (0, 99, 1);
    s_egl2->setSliderStyle (Slider::RotaryVerticalDrag);
    s_egl2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl2->addListener (this);

    addAndMakeVisible (s_egl3 = new Slider ("egl3"));
    s_egl3->setRange (0, 99, 1);
    s_egl3->setSliderStyle (Slider::RotaryVerticalDrag);
    s_egl3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl3->addListener (this);

    addAndMakeVisible (s_egl4 = new Slider ("egl4"));
    s_egl4->setRange (0, 99, 1);
    s_egl4->setSliderStyle (Slider::RotaryVerticalDrag);
    s_egl4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egl4->addListener (this);

    addAndMakeVisible (s_egv1 = new Slider ("egr1"));
    s_egv1->setRange (0, 99, 1);
    s_egv1->setSliderStyle (Slider::RotaryVerticalDrag);
    s_egv1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv1->addListener (this);

    addAndMakeVisible (s_egv2 = new Slider ("egr3"));
    s_egv2->setRange (0, 99, 1);
    s_egv2->setSliderStyle (Slider::RotaryVerticalDrag);
    s_egv2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv2->addListener (this);

    addAndMakeVisible (s_egv3 = new Slider ("egr3"));
    s_egv3->setRange (0, 99, 1);
    s_egv3->setSliderStyle (Slider::RotaryVerticalDrag);
    s_egv3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv3->addListener (this);

    addAndMakeVisible (s_egv4 = new Slider ("egr4"));
    s_egv4->setRange (0, 99, 1);
    s_egv4->setSliderStyle (Slider::RotaryVerticalDrag);
    s_egv4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    s_egv4->addListener (this);

    addAndMakeVisible (opLevel = new Slider ("opLevel"));
    opLevel->setRange (0, 99, 1);
    opLevel->setSliderStyle (Slider::RotaryVerticalDrag);
    opLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    opLevel->addListener (this);

    addAndMakeVisible (opFine = new Slider ("opFine"));
    opFine->setRange (0, 99, 1);
    opFine->setSliderStyle (Slider::RotaryVerticalDrag);
    opFine->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    opFine->addListener (this);

    addAndMakeVisible (opCoarse = new Slider ("opCoarse"));
    opCoarse->setRange (0, 31, 1);
    opCoarse->setSliderStyle (Slider::RotaryVerticalDrag);
    opCoarse->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    opCoarse->addListener (this);

    addAndMakeVisible (khzDisplay = new Label ("khz",
                                               TRANS("1,000 kHz")));
    khzDisplay->setFont (Font (12.60f, Font::plain));
    khzDisplay->setJustificationType (Justification::centred);
    khzDisplay->setEditable (false, false, false);
    khzDisplay->setColour (Label::backgroundColourId, Colour (0x6a000000));
    khzDisplay->setColour (Label::textColourId, Colours::white);
    khzDisplay->setColour (Label::outlineColourId, Colour (0x00000000));
    khzDisplay->setColour (TextEditor::textColourId, Colours::black);
    khzDisplay->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (detune = new Slider ("detune"));
    detune->setRange (-7, 7, 1);
    detune->setSliderStyle (Slider::RotaryVerticalDrag);
    detune->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    detune->addListener (this);

    addAndMakeVisible (envDisplay = new EnvDisplay());
    envDisplay->setName ("envDisplay");

    addAndMakeVisible (sclLeftLevel = new Slider ("sclLeftLevel"));
    sclLeftLevel->setTooltip (TRANS("Keyboard Scale Level Left Depth "));
    sclLeftLevel->setRange (0, 99, 1);
    sclLeftLevel->setSliderStyle (Slider::RotaryVerticalDrag);
    sclLeftLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sclLeftLevel->addListener (this);

    addAndMakeVisible (sclRightLevel = new Slider ("sclRightLevel"));
    sclRightLevel->setTooltip (TRANS("Keyboard Scale Level Right Depth "));
    sclRightLevel->setRange (0, 99, 1);
    sclRightLevel->setSliderStyle (Slider::RotaryVerticalDrag);
    sclRightLevel->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sclRightLevel->addListener (this);

    addAndMakeVisible (sclLvlBrkPt = new Slider ("sclLvlBrkPt"));
    sclLvlBrkPt->setTooltip (TRANS("Scale Level Breakpoint"));
    sclLvlBrkPt->setRange (0, 99, 1);
    sclLvlBrkPt->setSliderStyle (Slider::LinearHorizontal);
    sclLvlBrkPt->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sclLvlBrkPt->addListener (this);

    addAndMakeVisible (sclRateScaling = new Slider ("sclRateScaling"));
    sclRateScaling->setTooltip (TRANS("Keyboard Rate Scaling"));
    sclRateScaling->setRange (0, 7, 1);
    sclRateScaling->setSliderStyle (Slider::RotaryVerticalDrag);
    sclRateScaling->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    sclRateScaling->addListener (this);

    addAndMakeVisible (keyVelSens = new Slider ("keyVelSens"));
    keyVelSens->setRange (0, 7, 1);
    keyVelSens->setSliderStyle (Slider::RotaryVerticalDrag);
    keyVelSens->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    keyVelSens->addListener (this);

    addAndMakeVisible (ampModSens = new Slider ("ampModSens"));
    ampModSens->setRange (0, 3, 1);
    ampModSens->setSliderStyle (Slider::RotaryVerticalDrag);
    ampModSens->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    ampModSens->addListener (this);

    addAndMakeVisible (vu = new VuMeter());
    vu->setName ("vu");

    addAndMakeVisible (opMode = new ToggleButton ("opMode"));
    opMode->setButtonText (String::empty);
    opMode->addListener (this);

    addAndMakeVisible (kbdLeftCurve = new ComboBoxImage());
    kbdLeftCurve->setName ("kbdLeftCurve");

    addAndMakeVisible (kbdRightCurve = new ComboBoxImage());
    kbdRightCurve->setName ("kbdRightCurve");

    cachedImage_operatorEditor_287x218_png = ImageCache::getFromMemory (operatorEditor_287x218_png, operatorEditor_287x218_pngSize);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (287, 218);


    //[Constructor] You can add your own custom stuff here..
    Image tmp = ImageCache::getFromMemory(BinaryData::Scaling_36_26_png, BinaryData::Scaling_36_26_pngSize);

    kbdLeftCurve->addItem("-LN", 1);
    kbdLeftCurve->addItem("-EX", 2);
    kbdLeftCurve->addItem("+EX", 3);
    kbdLeftCurve->addItem("+LN", 4);
    int posLeft[] = {0,5,4,3};
    kbdLeftCurve->setImage(tmp, posLeft);

    kbdRightCurve->addItem("-LN", 1);
    kbdRightCurve->addItem("-EX", 2);
    kbdRightCurve->addItem("+EX", 3);
    kbdRightCurve->addItem("+LN", 4);
    int posRight[] = {3,2,1,0};
    kbdRightCurve->setImage(tmp, posRight);
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
    opLevel = nullptr;
    opFine = nullptr;
    opCoarse = nullptr;
    khzDisplay = nullptr;
    detune = nullptr;
    envDisplay = nullptr;
    sclLeftLevel = nullptr;
    sclRightLevel = nullptr;
    sclLvlBrkPt = nullptr;
    sclRateScaling = nullptr;
    keyVelSens = nullptr;
    ampModSens = nullptr;
    vu = nullptr;
    opMode = nullptr;
    kbdLeftCurve = nullptr;
    kbdRightCurve = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OperatorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colours::black);
    g.drawImage (cachedImage_operatorEditor_287x218_png,
                 0, 0, 287, 218,
                 0, 0, cachedImage_operatorEditor_287x218_png.getWidth(), cachedImage_operatorEditor_287x218_png.getHeight());

    //[UserPaint] Add your own custom painting code here..
    g.setColour (Colours::white);
    g.setFont(Font (30.00f, Font::plain));
    g.drawText(opNum, 242, 10, 30, 30, Justification::centred, true);

    Image myStrip = ImageCache::getFromMemory(BinaryData::Light_14x14_png, BinaryData::Light_14x14_pngSize);

    bool state = opMode->getToggleState();

    // 129 x 24
    g.drawImage(myStrip, 127, 24, 14, 14, 0, state ? 0 : 14, 14, 14);
    // 199 x 24
    g.drawImage(myStrip, 199, 24, 14, 14, 0, !state ? 0 : 14, 14, 14);

    //[/UserPaint]
}

void OperatorEditor::resized()
{
    s_egl1->setBounds (5, 128, 34, 34);
    s_egl2->setBounds (33, 129, 34, 34);
    s_egl3->setBounds (61, 128, 34, 34);
    s_egl4->setBounds (89, 128, 34, 34);
    s_egv1->setBounds (5, 169, 34, 34);
    s_egv2->setBounds (33, 169, 34, 34);
    s_egv3->setBounds (61, 169, 34, 34);
    s_egv4->setBounds (89, 169, 34, 34);
    opLevel->setBounds (244, 74, 34, 34);
    opFine->setBounds (78, 24, 34, 34);
    opCoarse->setBounds (41, 24, 34, 34);
    khzDisplay->setBounds (15, 10, 95, 10);
    detune->setBounds (6, 24, 34, 34);
    envDisplay->setBounds (16, 83, 94, 30);
    sclLeftLevel->setBounds (131, 115, 34, 34);
    sclRightLevel->setBounds (241, 115, 34, 34);
    sclLvlBrkPt->setBounds (178, 130, 54, 24);
    sclRateScaling->setBounds (186, 179, 34, 34);
    keyVelSens->setBounds (204, 74, 34, 34);
    ampModSens->setBounds (141, 75, 34, 34);
    vu->setBounds (132, 52, 140, 8);
    opMode->setBounds (147, 19, 48, 26);
    kbdLeftCurve->setBounds (129, 170, 36, 26);
    kbdRightCurve->setBounds (241, 170, 36, 26);
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

void OperatorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == opMode)
    {
        //[UserButtonCode_opMode] -- add your button handler code here..
        repaint();
        //[/UserButtonCode_opMode]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
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

    opNum << op + 1;
}

void OperatorEditor::updateGain(float v) {
    vu->v = v;
    vu->repaint();
}

void OperatorEditor::updateDisplay() {
    float freq = opCoarse->getValue();
    float fine = opFine->getValue();
    String txtFreq;

    if (opMode->getToggleState() == 0) {
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
    khzDisplay->setText(txtFreq, dontSendNotification);
    repaint();
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
                 snapPixels="2" snapActive="0" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="287" initialHeight="218">
  <BACKGROUND backgroundColour="ffffff">
    <IMAGE pos="0 0 287 218" resource="operatorEditor_287x218_png" opacity="1"
           mode="0"/>
  </BACKGROUND>
  <SLIDER name="egl1" id="dc070cc41347df47" memberName="s_egl1" virtualName=""
          explicitFocusOrder="0" pos="5 128 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egl2" id="66f5195e9c374029" memberName="s_egl2" virtualName=""
          explicitFocusOrder="0" pos="33 129 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egl3" id="9d57bd53203dcdb4" memberName="s_egl3" virtualName=""
          explicitFocusOrder="0" pos="61 128 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egl4" id="4f7c3ece3ea2cf9c" memberName="s_egl4" virtualName=""
          explicitFocusOrder="0" pos="89 128 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr1" id="2ca8137d80da46fb" memberName="s_egv1" virtualName=""
          explicitFocusOrder="0" pos="5 169 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr3" id="4ad6d0c532d15973" memberName="s_egv2" virtualName=""
          explicitFocusOrder="0" pos="33 169 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr3" id="8a2027f9ede16b4f" memberName="s_egv3" virtualName=""
          explicitFocusOrder="0" pos="61 169 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="egr4" id="8c04f1c943d837e8" memberName="s_egv4" virtualName=""
          explicitFocusOrder="0" pos="89 169 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="opLevel" id="f8521c8214fb8993" memberName="opLevel" virtualName=""
          explicitFocusOrder="0" pos="244 74 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="opFine" id="e445aa61bd6cddcb" memberName="opFine" virtualName=""
          explicitFocusOrder="0" pos="78 24 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="opCoarse" id="4eec63d30d7488d2" memberName="opCoarse" virtualName=""
          explicitFocusOrder="0" pos="41 24 34 34" min="0" max="31" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <LABEL name="khz" id="eb961eed8902a6fc" memberName="khzDisplay" virtualName=""
         explicitFocusOrder="0" pos="15 10 95 10" bkgCol="6a000000" textCol="ffffffff"
         outlineCol="0" edTextCol="ff000000" edBkgCol="0" labelText="1,000 kHz"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.6" bold="0" italic="0" justification="36"/>
  <SLIDER name="detune" id="f093ec8defca2fc2" memberName="detune" virtualName=""
          explicitFocusOrder="0" pos="6 24 34 34" min="-7" max="7" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <GENERICCOMPONENT name="envDisplay" id="b18856de924c6340" memberName="envDisplay"
                    virtualName="" explicitFocusOrder="0" pos="16 83 94 30" class="EnvDisplay"
                    params=""/>
  <SLIDER name="sclLeftLevel" id="bd6f338ae68e454f" memberName="sclLeftLevel"
          virtualName="" explicitFocusOrder="0" pos="131 115 34 34" tooltip="Keyboard Scale Level Left Depth "
          min="0" max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="sclRightLevel" id="b9e23ed5187fc7e5" memberName="sclRightLevel"
          virtualName="" explicitFocusOrder="0" pos="241 115 34 34" tooltip="Keyboard Scale Level Right Depth "
          min="0" max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="sclLvlBrkPt" id="c563ac3116923bbc" memberName="sclLvlBrkPt"
          virtualName="" explicitFocusOrder="0" pos="178 130 54 24" tooltip="Scale Level Breakpoint"
          min="0" max="99" int="1" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="sclRateScaling" id="f0d17c8e09aa4c49" memberName="sclRateScaling"
          virtualName="" explicitFocusOrder="0" pos="186 179 34 34" tooltip="Keyboard Rate Scaling"
          min="0" max="7" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="keyVelSens" id="21795d045d07602b" memberName="keyVelSens"
          virtualName="" explicitFocusOrder="0" pos="204 74 34 34" min="0"
          max="7" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="ampModSens" id="634ceaa7b0f81a6c" memberName="ampModSens"
          virtualName="" explicitFocusOrder="0" pos="141 75 34 34" min="0"
          max="3" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <GENERICCOMPONENT name="vu" id="6f952594ea99dc1e" memberName="vu" virtualName=""
                    explicitFocusOrder="0" pos="132 52 140 8" class="VuMeter" params=""/>
  <TOGGLEBUTTON name="opMode" id="15db8e5c9dd13966" memberName="opMode" virtualName=""
                explicitFocusOrder="0" pos="147 19 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="kbdLeftCurve" id="ee4e60dfcd6cb790" memberName="kbdLeftCurve"
                    virtualName="" explicitFocusOrder="0" pos="129 170 36 26" class="ComboBoxImage"
                    params=""/>
  <GENERICCOMPONENT name="kbdRightCurve" id="f5ca44d67593488a" memberName="kbdRightCurve"
                    virtualName="" explicitFocusOrder="0" pos="241 170 36 26" class="ComboBoxImage"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: operatorEditor_287x218_png, 15614, "../Resources/ui/OperatorEditor_287x218.png"
static const unsigned char resource_OperatorEditor_operatorEditor_287x218_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,31,0,0,0,218,8,6,0,0,0,159,5,70,254,0,0,0,9,112,72,89,115,0,0,14,
195,0,0,14,195,1,199,111,168,100,0,0,0,7,116,73,77,69,7,222,9,16,3,31,30,140,250,111,181,0,0,0,29,105,84,88,116,67,111,109,109,101,110,116,0,0,0,0,0,67,114,101,97,116,101,100,32,119,105,116,104,32,71,
73,77,80,100,46,101,7,0,0,32,0,73,68,65,84,120,218,237,157,123,124,19,85,250,255,223,147,164,147,82,18,46,77,87,155,254,190,75,139,210,214,181,173,75,91,86,145,149,155,34,119,81,89,92,117,215,239,42,42,
138,171,130,178,224,34,184,32,10,138,11,11,138,178,162,40,136,43,138,138,168,220,4,111,8,42,23,191,189,32,109,193,94,118,105,171,54,181,144,172,154,64,201,180,73,126,127,76,50,77,218,180,180,64,41,109,
207,251,245,138,210,201,204,153,147,121,206,124,230,57,207,121,230,28,9,136,244,127,76,64,119,255,167,27,208,213,191,61,2,208,3,18,2,129,64,208,52,62,192,3,212,2,39,128,99,128,19,248,201,255,113,1,213,
128,98,240,11,76,15,32,6,56,63,241,130,248,254,125,122,39,12,176,198,158,127,193,47,255,231,255,37,136,107,41,16,8,78,133,111,191,251,190,236,200,81,123,121,105,249,183,217,121,135,138,118,0,149,192,209,
128,72,73,192,47,129,243,205,166,174,23,13,236,255,155,91,127,147,153,62,204,102,179,1,16,248,191,64,32,16,180,20,171,213,170,253,255,224,161,111,254,111,219,39,59,151,59,143,29,255,198,47,66,63,74,64,
42,144,48,126,204,136,135,99,44,61,7,8,193,17,8,4,173,33,68,110,165,166,112,205,186,119,102,2,135,129,74,61,144,152,116,97,194,240,139,47,74,188,201,86,105,83,35,59,226,35,62,226,35,62,103,240,227,114,
185,232,209,163,123,12,248,220,182,31,142,124,3,28,51,0,221,146,19,47,188,188,41,143,199,235,245,114,226,132,155,154,90,15,62,159,15,165,166,70,72,185,64,112,138,24,244,122,244,122,61,70,57,2,89,150,209,
233,58,199,88,142,205,102,227,162,164,62,191,201,254,186,224,29,160,139,1,232,218,37,210,152,88,125,188,182,193,206,62,159,15,167,235,56,39,220,110,209,98,4,130,51,68,173,199,67,173,199,131,91,81,128,
99,116,137,52,210,53,42,170,83,136,80,132,193,112,1,16,5,200,6,32,242,68,245,241,88,73,146,26,8,207,207,174,99,40,74,13,245,191,19,8,4,103,142,19,110,133,154,218,90,122,118,239,134,78,167,235,208,191,
213,225,176,71,2,70,32,66,135,154,199,211,128,99,199,171,81,20,209,189,18,8,206,6,30,143,151,159,126,118,117,154,158,39,160,51,0,186,250,249,131,94,175,151,227,213,110,154,202,43,220,155,253,181,104,49,
109,76,255,204,95,159,181,115,37,38,37,83,86,122,24,69,81,58,204,245,139,181,198,113,253,239,38,160,40,10,91,54,109,68,81,220,56,236,246,211,42,51,218,98,225,129,105,51,152,51,123,102,139,143,173,169,
245,224,118,215,96,52,202,29,189,233,234,0,201,16,78,97,154,235,241,116,49,26,59,188,155,120,78,90,78,167,163,91,183,110,173,122,142,248,132,4,220,110,133,74,91,5,0,55,222,252,7,254,181,102,53,101,165,
165,29,230,58,62,244,240,108,62,223,185,131,252,188,60,50,50,251,161,40,110,62,254,112,251,105,149,41,203,70,82,211,210,78,249,248,106,183,187,51,136,15,1,241,105,216,7,109,198,211,77,47,73,24,141,70,
12,6,131,80,131,179,109,53,255,181,111,41,38,147,25,75,140,5,167,211,73,124,66,111,10,242,14,160,40,10,41,169,105,200,70,35,197,133,133,184,92,78,100,89,230,142,73,147,41,200,207,163,168,168,144,220,236,
44,222,125,103,61,182,138,10,173,172,192,49,101,165,135,79,219,91,104,43,175,39,41,41,153,127,173,89,141,221,126,20,103,182,19,69,113,99,50,153,137,79,72,160,32,63,79,243,100,204,102,179,38,186,41,169,
170,176,4,190,15,144,158,217,15,32,228,26,157,90,247,203,211,121,30,162,225,54,214,214,214,138,59,188,3,146,152,156,204,35,115,31,99,233,178,229,254,39,189,194,232,177,227,24,54,124,4,25,153,253,120,245,
245,117,196,90,227,72,73,187,132,248,132,4,18,147,146,200,240,223,84,143,63,177,144,104,75,12,0,51,102,206,230,129,191,204,96,208,224,33,44,127,225,37,250,95,62,160,221,93,139,49,215,140,67,54,202,100,
100,246,195,98,137,97,204,53,227,232,127,249,111,81,20,55,15,252,101,6,233,153,253,144,101,153,167,22,47,197,100,50,3,48,117,218,116,254,247,182,137,12,27,62,130,53,107,215,33,203,170,135,114,207,125,
83,120,96,218,116,6,13,30,194,67,15,207,62,237,216,79,103,10,252,52,192,235,245,137,59,181,131,18,159,144,192,248,107,198,224,114,57,1,216,186,121,99,80,151,65,102,240,144,161,188,249,198,90,138,139,10,
217,178,121,19,185,217,89,13,60,134,193,67,134,50,118,228,48,0,118,237,252,140,235,199,79,96,239,158,221,237,234,58,188,252,226,10,198,140,189,134,151,95,92,1,192,160,33,67,253,33,7,133,249,243,230,176,
112,241,82,114,178,178,216,187,103,55,5,249,121,218,239,30,63,110,140,38,56,195,134,143,100,235,230,141,140,25,59,142,241,227,70,163,40,10,131,134,12,229,129,105,211,69,67,107,174,248,212,15,250,136,129,
245,118,210,253,58,133,253,247,238,222,205,49,151,83,59,118,216,240,17,12,28,60,20,139,197,130,53,46,142,173,155,55,105,223,73,245,206,33,1,214,184,56,242,243,15,104,219,247,103,103,241,212,226,37,237,
182,205,212,255,173,18,80,94,90,202,190,221,95,50,250,154,113,140,24,58,8,9,72,72,72,64,150,141,44,95,177,82,59,246,152,203,73,82,82,50,118,251,81,106,20,5,9,40,46,44,60,237,123,72,234,84,226,83,47,143,
71,164,245,180,151,216,79,203,27,181,162,184,181,227,76,38,51,83,167,205,96,194,181,234,83,123,250,204,217,33,229,26,141,114,200,57,36,9,42,109,21,36,37,37,107,219,19,147,146,41,200,203,107,183,109,166,
254,239,147,36,213,187,235,63,224,183,124,252,225,118,238,184,107,50,171,86,174,96,127,78,22,46,151,147,251,239,153,20,114,188,44,203,88,44,49,90,57,113,113,113,167,125,15,117,150,188,58,17,45,238,196,
40,138,27,69,113,51,108,248,72,76,102,51,137,73,201,236,243,119,159,246,231,100,115,203,173,19,81,220,10,185,57,117,93,175,74,91,5,57,217,89,204,154,51,143,131,249,121,92,59,126,2,171,86,190,208,97,174,
137,44,203,204,91,176,144,69,79,46,160,32,255,0,207,174,120,137,220,236,44,114,115,178,200,201,206,226,201,69,75,248,248,163,237,152,77,106,16,58,55,39,139,189,123,190,228,201,69,75,216,183,103,55,233,
25,253,68,195,106,38,122,32,227,162,62,189,199,5,111,60,118,188,250,164,7,126,111,251,1,163,24,106,111,163,167,181,58,218,101,138,138,108,209,113,46,167,147,226,162,66,126,250,233,71,64,29,89,217,181,
243,51,18,147,147,177,85,84,176,102,213,75,148,149,149,226,114,169,251,185,156,46,18,147,147,57,240,245,126,10,242,243,248,254,187,111,241,120,60,236,219,179,155,234,234,106,226,19,122,179,246,213,213,
33,226,212,158,40,200,207,227,72,85,21,0,54,91,5,101,101,135,1,137,60,255,239,245,120,60,236,254,98,23,6,131,1,135,221,206,238,47,62,7,9,226,227,123,83,117,228,7,190,57,116,144,234,234,106,62,223,249,
25,149,182,10,44,150,24,222,90,183,150,221,95,124,126,90,35,128,93,163,162,58,116,251,253,166,228,240,38,160,74,2,238,188,110,228,149,43,131,191,172,58,122,242,11,183,47,251,0,102,179,89,12,181,183,145,
248,152,205,102,98,99,186,139,139,209,1,57,47,198,210,161,127,223,123,219,62,157,4,228,27,180,96,128,64,32,56,71,158,46,157,227,103,138,62,147,64,32,104,19,78,185,207,100,52,26,137,140,140,20,49,159,182,
120,98,232,116,90,130,155,64,208,233,196,39,45,45,141,232,232,104,33,62,109,36,62,17,17,17,84,126,119,88,92,12,65,251,22,159,6,121,5,205,200,51,208,83,195,79,142,31,196,21,108,211,216,128,8,214,117,76,
179,118,14,187,10,183,69,32,16,8,241,17,8,4,66,124,4,2,129,64,136,143,64,32,16,226,35,16,8,4,66,124,4,2,65,251,197,63,159,79,189,41,53,196,251,22,2,65,155,209,89,238,63,225,249,8,4,130,54,244,124,26,76,
38,38,60,31,129,160,205,60,31,145,100,40,16,8,4,66,124,4,2,129,16,31,129,64,32,16,226,35,16,8,132,248,8,4,2,129,16,31,129,160,93,162,215,119,158,91,82,136,143,64,112,14,33,71,68,116,154,223,218,200,100,
98,162,17,8,4,109,65,151,200,200,78,182,104,160,16,27,129,160,205,137,48,24,144,141,157,199,243,17,221,46,129,224,28,64,146,36,186,117,51,117,170,223,44,86,252,19,8,218,218,3,208,233,232,217,163,27,17,
157,108,1,78,33,62,2,65,91,117,179,34,12,68,26,141,68,117,137,236,148,239,83,10,241,233,164,46,190,209,40,19,41,203,232,13,250,78,247,196,21,156,67,226,83,95,115,69,252,185,227,138,142,169,107,23,186,
70,69,137,139,33,16,158,143,224,236,9,79,143,238,102,140,98,165,83,193,57,130,24,237,234,36,116,55,155,132,240,8,206,193,110,151,152,76,172,67,163,215,235,233,210,37,50,236,119,74,77,13,110,183,66,77,
109,45,138,82,35,46,150,224,148,145,229,8,12,6,3,70,57,162,89,15,58,209,237,234,4,116,137,52,54,216,230,245,122,57,118,188,154,99,199,171,197,5,18,156,17,20,165,6,69,169,225,248,241,106,186,68,26,49,155,
186,162,211,233,78,38,62,34,228,220,145,137,136,136,104,96,83,215,177,106,142,87,159,16,182,22,180,10,213,39,20,188,94,31,61,123,116,111,116,31,17,243,233,12,221,174,122,79,31,69,169,241,11,143,64,208,
122,184,149,26,170,79,156,16,226,211,153,49,212,203,227,57,161,184,197,69,17,156,21,78,184,21,33,62,130,80,207,71,32,104,235,182,38,196,167,19,82,91,235,17,23,65,112,86,240,249,124,141,123,228,103,226,
4,151,246,191,156,232,104,11,219,182,110,22,87,219,79,108,172,149,113,215,143,71,81,20,182,109,217,140,162,40,56,28,118,113,97,4,130,51,233,249,68,71,91,136,181,90,155,220,71,150,101,250,166,103,116,154,
11,59,237,161,153,84,86,218,248,98,215,78,250,102,100,210,55,35,83,180,54,129,224,76,120,62,38,147,137,62,137,73,40,138,18,86,140,122,197,199,83,82,92,132,203,229,2,96,194,141,55,211,43,62,30,128,131,
5,249,97,143,59,25,178,44,115,113,74,42,0,149,54,27,149,149,54,0,46,78,73,69,150,101,202,203,202,66,188,139,216,88,43,177,86,107,200,190,177,177,86,20,69,65,150,101,162,45,22,14,22,228,135,148,187,63,
55,71,59,190,87,124,60,209,209,150,22,215,55,54,214,74,159,164,36,94,255,215,26,28,118,59,251,157,217,218,241,125,18,147,40,47,43,229,226,148,84,20,69,225,96,65,126,200,177,129,122,212,223,46,16,116,52,
244,64,70,74,114,159,113,193,27,93,199,142,159,84,4,86,175,93,135,201,108,38,237,215,191,230,242,223,94,65,121,89,25,251,115,115,232,147,152,196,188,39,22,162,40,10,19,110,188,25,189,222,128,203,233,228,
198,63,222,130,201,108,6,160,240,155,67,167,36,62,207,174,88,73,239,11,46,68,54,26,49,153,205,148,20,23,49,243,145,57,92,61,114,20,93,186,116,225,142,187,239,161,240,155,67,28,57,82,69,159,196,36,254,
240,167,91,49,153,205,76,184,233,102,0,74,138,139,152,112,227,205,92,61,114,36,131,135,94,133,82,83,67,165,205,198,223,151,46,195,227,241,208,43,62,129,95,156,119,62,37,197,69,140,28,61,150,123,238,159,
138,162,40,220,115,255,84,74,138,139,56,114,164,170,89,245,156,112,227,205,164,253,186,47,14,135,157,35,71,170,184,242,234,225,116,239,209,131,146,226,34,230,60,190,128,203,127,123,5,177,214,56,250,102,
100,112,253,132,223,107,221,213,153,143,204,33,243,55,151,242,139,243,206,227,174,63,223,199,182,173,155,241,120,78,63,62,99,234,26,250,34,233,201,236,43,16,156,73,234,183,191,130,194,146,77,64,213,41,
121,62,23,167,164,82,82,84,196,194,249,143,105,55,77,128,43,135,93,205,43,47,175,228,139,93,59,137,142,182,240,247,165,207,176,109,235,102,246,231,100,3,240,202,203,43,57,213,115,58,236,118,30,153,57,
35,196,251,234,155,145,201,77,227,175,85,99,79,251,246,50,238,250,241,28,44,200,167,164,184,174,126,151,22,228,51,238,186,241,218,77,30,107,141,227,207,147,110,7,160,111,122,6,38,147,153,109,91,54,107,
222,81,224,119,60,244,224,84,28,14,59,229,101,101,140,28,61,182,217,222,200,43,47,175,100,228,152,177,218,111,189,98,208,224,144,239,75,138,139,180,239,94,125,227,109,162,163,45,184,92,78,122,197,39,104,
245,154,54,99,38,87,12,26,194,167,31,127,40,90,175,64,116,187,52,33,72,77,163,164,184,72,251,251,96,65,62,209,209,22,245,166,189,122,56,125,51,50,153,112,163,234,109,4,186,93,167,75,223,140,204,144,115,
2,92,218,127,0,37,69,65,245,200,207,211,132,240,226,148,84,38,220,120,147,191,251,148,16,82,143,128,16,6,186,89,175,188,188,146,191,47,93,70,73,113,33,47,254,115,57,14,135,157,190,25,153,204,121,124,65,
157,96,20,21,158,177,139,30,44,98,14,135,93,141,135,101,100,18,107,181,242,244,242,21,97,247,19,8,58,164,248,248,8,29,14,171,255,119,125,202,202,74,185,106,216,213,218,126,191,236,21,207,49,151,11,31,
62,114,115,178,249,106,239,158,6,79,108,69,81,232,106,50,157,180,236,198,40,200,207,227,134,27,111,14,57,190,172,236,48,125,146,146,180,109,23,38,38,177,63,39,27,31,62,238,123,96,26,207,62,253,15,14,22,
228,115,229,176,225,92,123,253,120,109,63,69,81,66,202,249,228,227,237,124,242,241,118,174,189,254,119,76,123,104,38,51,166,77,161,188,172,148,39,31,127,52,196,27,106,41,161,191,213,231,255,219,135,219,
237,14,250,78,221,94,92,84,136,203,233,98,234,189,119,159,113,35,183,212,190,2,65,107,182,191,211,242,124,190,218,187,155,187,239,185,143,137,119,220,5,192,101,253,7,104,98,179,241,221,119,120,248,145,
71,177,90,227,40,43,43,197,106,181,242,246,155,111,144,155,147,197,220,199,159,96,127,110,54,7,243,243,90,236,17,29,44,200,35,218,114,31,247,77,157,198,183,229,101,148,149,150,114,176,32,143,242,210,82,
102,62,50,151,67,254,174,213,11,207,63,167,122,20,118,59,87,12,26,66,124,124,2,151,246,191,188,209,114,47,237,127,57,41,41,105,228,230,100,211,55,61,131,175,246,237,81,127,199,123,27,88,240,212,98,214,
191,249,6,0,118,135,157,175,246,238,105,53,3,57,28,118,74,138,11,89,176,112,49,31,108,221,140,197,98,33,55,39,155,242,178,82,209,122,5,29,146,83,10,56,123,60,30,182,108,122,143,243,206,59,15,187,195,206,
139,207,63,75,121,89,41,46,151,139,35,71,170,200,206,250,138,248,248,4,122,244,232,65,214,255,125,165,5,94,75,138,138,184,228,215,125,57,88,144,215,226,128,179,122,206,247,65,2,179,217,76,225,161,131,
252,244,211,79,236,220,241,9,213,213,213,196,199,39,176,250,229,149,90,87,229,171,189,187,233,209,163,7,178,209,200,218,127,189,194,193,130,124,28,14,59,149,182,138,144,81,184,234,227,213,120,188,30,46,
249,117,95,62,219,241,169,38,162,133,223,28,162,164,184,136,139,126,117,49,74,141,210,98,193,60,152,159,167,5,168,43,109,21,148,151,149,81,93,93,77,73,113,17,229,101,165,218,239,47,41,46,162,178,210,134,
199,227,97,231,142,79,1,184,224,130,11,169,170,170,162,164,184,240,148,2,243,39,11,248,53,101,95,89,150,233,125,193,133,103,53,39,233,210,254,151,51,100,232,48,242,14,236,23,119,100,7,164,126,251,59,88,
248,239,77,64,149,4,220,121,195,184,17,33,81,224,170,163,14,188,94,175,184,106,29,132,216,243,98,66,254,174,172,58,218,232,190,163,70,143,101,218,67,15,51,102,248,208,51,34,124,205,97,212,232,177,252,
58,35,147,133,243,231,157,211,215,177,79,98,18,151,245,31,192,218,127,189,210,102,117,184,111,234,52,53,133,163,5,15,135,214,168,119,75,234,81,191,253,189,189,113,251,36,32,63,108,146,161,193,160,23,119,
108,71,113,109,245,45,179,229,132,27,111,230,131,45,155,24,53,230,154,70,247,185,56,37,85,29,105,76,207,212,242,146,122,197,39,208,55,61,19,147,41,116,237,169,192,246,62,137,73,161,79,195,122,199,55,229,
137,245,77,207,164,111,122,102,216,114,235,223,100,64,216,114,27,43,167,69,79,112,147,153,95,250,115,213,218,138,196,164,36,228,22,206,72,217,26,245,62,149,122,132,141,249,212,159,209,37,66,175,167,6,
241,242,97,71,32,82,142,104,246,108,77,125,18,147,112,216,237,188,243,214,58,230,60,182,128,141,239,190,19,118,191,185,143,63,65,73,145,218,125,236,155,145,73,165,173,2,147,73,205,187,90,240,212,98,238,
252,211,31,168,172,180,113,105,255,203,185,111,234,95,248,98,215,103,244,205,200,228,171,189,123,120,229,229,23,1,88,180,244,89,28,118,187,150,98,80,94,86,218,160,94,106,170,198,50,190,216,181,19,147,
201,132,213,106,101,219,214,205,140,28,61,150,145,99,198,178,63,39,135,187,255,124,31,75,254,254,36,37,197,69,220,255,192,52,20,183,162,214,199,108,38,218,98,225,129,123,39,55,40,199,104,148,79,41,126,
39,5,125,2,34,28,8,55,168,73,183,201,148,151,149,226,112,216,49,153,76,196,90,227,66,70,104,131,247,15,8,112,83,251,244,138,79,32,58,218,194,254,220,236,176,245,56,213,122,183,180,174,167,91,143,198,246,
9,235,249,68,134,153,249,78,208,78,197,199,216,124,91,6,114,161,2,49,169,250,222,74,48,95,236,250,140,23,159,127,142,231,158,254,7,87,12,26,194,35,51,167,243,226,243,207,177,241,221,119,184,98,240,16,
127,121,191,99,201,223,159,228,197,231,159,227,129,123,239,102,220,245,227,145,101,217,159,25,239,230,145,153,211,89,56,127,30,7,243,243,194,123,88,169,105,184,92,78,182,109,217,196,115,207,44,209,242,
180,38,220,120,51,143,252,117,58,175,188,252,34,235,223,124,131,145,163,199,106,199,124,181,111,15,75,22,45,228,177,57,179,233,21,159,160,229,130,185,92,78,94,255,215,43,60,247,204,146,51,50,112,112,219,
29,119,49,238,250,223,225,114,185,136,142,182,240,244,242,23,184,180,255,229,76,123,232,97,254,240,191,183,161,40,10,243,159,90,172,165,160,168,2,248,108,72,87,86,81,20,254,190,116,153,230,45,70,71,91,
152,255,212,98,20,69,97,228,232,177,76,123,104,38,125,51,50,121,105,205,90,122,197,39,156,145,246,208,88,93,207,118,61,26,21,31,57,34,162,193,28,48,130,246,135,193,96,64,150,155,183,246,183,201,100,98,
228,152,107,152,112,227,205,60,189,124,5,209,22,11,227,174,27,223,232,254,129,192,190,195,110,167,164,184,72,187,169,20,69,193,100,82,51,217,47,237,127,185,246,180,84,20,133,242,178,82,250,36,38,169,57,
91,65,249,89,245,243,183,130,5,238,211,143,63,226,233,127,190,192,204,71,230,170,79,232,88,43,189,226,19,152,255,212,98,158,94,190,130,113,215,135,214,49,56,135,43,80,151,79,63,254,144,47,118,237,228,
213,117,235,181,114,78,87,120,98,173,86,45,70,117,197,224,33,108,124,111,3,47,62,255,28,143,205,153,165,137,225,167,31,125,200,149,87,15,7,96,228,152,107,216,182,101,83,3,241,81,247,25,1,168,57,114,159,
126,244,161,246,118,64,64,96,55,190,183,129,43,135,13,63,35,109,226,15,255,123,43,219,182,110,110,164,174,103,175,30,90,183,43,28,61,187,155,113,252,248,243,25,73,239,23,156,125,36,73,162,123,11,214,254,
30,57,230,26,54,190,251,14,207,61,179,68,139,145,108,216,188,157,231,158,89,18,54,240,172,184,221,65,55,81,248,201,201,2,98,19,16,151,94,241,9,148,20,23,33,203,198,144,216,75,172,53,174,209,122,109,124,
247,29,54,190,251,14,183,221,113,23,211,30,122,152,199,230,204,198,225,176,243,216,223,102,135,13,118,186,156,206,176,229,172,127,243,13,214,191,249,6,183,221,113,23,119,221,115,31,75,22,45,60,165,235,
218,55,35,19,147,201,28,18,28,191,114,216,213,152,76,102,174,28,118,181,42,200,142,163,152,76,102,54,190,251,14,243,253,233,26,35,71,143,229,177,57,179,26,148,183,109,235,102,166,61,244,48,27,223,125,
135,43,135,13,103,225,252,121,33,2,171,9,116,81,209,25,105,23,234,107,61,153,90,214,125,160,174,103,187,30,154,248,132,155,115,67,167,211,209,179,187,153,31,127,118,82,83,83,43,238,230,118,68,96,237,111,
131,94,31,214,182,225,182,141,28,61,150,39,31,127,84,251,206,237,118,243,201,71,219,25,57,122,44,239,215,143,253,248,252,169,145,62,127,226,164,47,184,76,159,118,142,245,111,190,193,204,71,230,178,109,
203,102,250,102,100,176,111,207,110,220,110,55,185,57,89,76,123,104,38,147,38,223,75,77,141,194,165,253,251,83,86,90,218,160,94,125,51,50,73,73,77,163,32,63,143,139,83,82,249,106,239,30,181,220,117,111,
48,255,169,69,108,124,119,3,0,78,167,147,47,118,125,22,82,175,224,122,14,29,118,53,86,107,156,86,206,167,31,127,216,228,60,51,141,225,195,71,73,81,33,207,62,253,15,158,89,254,34,197,69,223,80,105,179,
177,63,71,125,113,120,237,171,175,52,56,166,210,86,193,111,7,14,70,81,220,20,135,201,146,47,46,42,68,150,101,117,31,183,155,178,210,195,154,71,57,239,145,89,13,5,182,254,111,108,102,189,125,62,245,115,
176,32,143,131,5,121,13,234,106,183,31,109,181,122,52,182,79,80,158,143,68,253,143,78,167,35,170,75,23,34,34,34,208,73,58,60,94,31,106,57,146,248,156,131,159,136,136,8,186,70,69,209,163,155,217,63,202,
21,126,63,215,177,234,144,191,101,217,72,225,161,67,20,126,243,77,200,246,194,67,135,112,56,28,252,244,211,79,33,219,15,230,231,241,253,119,223,225,241,120,81,20,133,194,111,212,253,64,162,210,102,211,
242,168,74,138,139,56,152,159,79,124,66,111,14,28,248,154,127,189,178,74,43,227,221,245,111,211,189,103,79,126,250,233,39,94,121,233,69,74,138,139,27,156,71,241,79,193,121,209,175,82,248,96,203,38,127,
30,148,196,193,130,124,190,255,238,59,18,147,146,81,20,133,188,3,251,181,243,85,218,108,120,60,222,144,122,186,92,46,244,122,3,137,73,201,124,176,101,19,123,190,252,226,148,174,111,172,53,142,94,241,9,
108,219,186,133,74,155,141,251,31,252,11,31,109,251,128,239,191,251,142,63,223,255,0,191,248,197,249,232,13,6,134,143,28,205,254,156,28,245,55,40,53,60,252,183,71,121,253,95,107,40,252,230,80,216,114,
37,36,238,188,251,30,214,190,182,134,195,255,249,143,127,27,252,105,226,29,72,146,68,98,82,50,177,214,255,71,121,89,25,163,198,140,229,139,93,59,253,65,235,230,215,187,79,98,50,95,236,218,201,145,170,
170,144,186,14,185,114,24,121,95,127,221,170,245,48,117,237,26,218,126,138,130,243,124,174,25,177,82,248,11,157,135,166,242,124,4,77,199,197,162,45,49,90,214,121,240,136,80,172,213,202,192,65,67,48,153,
205,236,219,179,91,139,137,201,178,204,150,143,118,112,253,152,17,141,38,169,202,178,204,197,169,105,28,204,15,77,190,237,155,145,73,122,70,38,14,187,157,125,123,119,83,105,179,105,83,178,180,36,7,171,
126,189,155,170,107,107,212,163,65,158,207,38,53,207,71,136,143,16,31,65,43,50,241,206,187,136,136,144,121,209,255,218,79,103,164,49,241,17,115,56,11,4,173,64,159,196,36,158,89,190,130,243,99,173,90,110,
147,32,76,192,89,32,16,156,89,74,138,139,152,122,239,100,113,33,78,71,124,106,106,106,56,33,214,242,110,23,200,114,4,17,254,220,30,227,105,166,190,11,4,103,71,124,194,228,63,135,93,203,91,172,172,123,
78,163,212,212,160,212,212,112,172,250,36,107,101,11,59,10,206,38,82,19,226,19,110,28,222,233,58,38,150,212,109,199,28,175,62,129,199,227,165,103,143,110,13,190,59,149,28,23,129,224,84,105,172,189,133,
13,56,139,181,188,59,6,110,69,105,114,173,108,129,160,237,187,93,97,26,109,115,248,98,95,142,184,130,109,200,21,151,157,124,29,52,183,91,161,75,100,164,184,88,130,246,33,62,45,9,44,119,49,26,195,199,21,
4,173,134,78,167,163,91,183,110,205,218,87,169,17,131,4,130,115,88,124,234,199,131,106,107,107,155,21,147,212,75,18,70,163,81,188,1,127,150,145,252,215,189,57,54,242,121,125,205,158,207,71,32,104,149,
246,218,216,67,84,92,26,129,64,208,38,30,188,184,4,2,129,64,136,143,64,32,16,226,35,16,8,4,173,73,216,36,67,145,130,214,62,104,174,157,132,125,5,109,218,78,91,146,100,40,16,8,4,162,219,37,16,8,132,248,
8,4,2,129,16,31,129,64,32,196,71,32,16,8,90,138,255,189,8,145,112,223,177,17,246,21,156,123,237,79,120,62,2,129,160,237,60,159,6,227,240,34,17,164,125,208,76,59,9,251,10,218,180,153,138,60,31,129,64,112,
46,33,196,71,32,16,180,93,183,75,18,19,190,180,79,154,105,39,97,95,65,155,54,83,73,120,62,2,129,64,116,187,4,2,129,16,31,129,64,32,16,226,35,56,27,232,197,132,255,130,179,68,83,225,197,70,242,124,68,34,
72,187,160,153,118,170,111,223,136,8,3,158,19,110,113,253,4,173,78,68,68,132,200,243,17,212,33,71,68,136,139,32,56,75,226,99,16,221,46,65,29,93,34,141,66,128,4,173,223,189,215,235,233,26,213,69,136,143,
32,20,179,169,43,17,98,189,53,65,43,10,79,247,110,38,36,169,241,168,143,104,125,157,20,131,65,79,116,207,238,28,59,94,141,219,173,80,83,91,43,46,138,224,140,180,43,89,150,49,69,117,105,82,120,132,248,
8,232,26,213,165,73,215,88,32,104,45,68,183,75,32,16,8,241,17,8,4,66,124,4,2,129,160,85,9,191,104,96,51,147,215,140,70,35,145,145,145,232,68,198,236,217,125,98,232,116,200,178,220,108,59,53,181,95,77,
77,45,110,69,161,166,166,22,165,166,70,92,92,193,41,35,71,68,104,1,103,163,124,242,84,142,211,10,56,167,165,165,17,29,29,45,196,167,13,196,39,34,34,130,31,190,47,61,229,50,188,94,47,199,171,79,112,236,
120,181,184,160,130,51,130,82,83,131,82,83,195,241,234,19,68,70,26,49,119,141,106,82,27,78,75,124,12,212,240,179,227,7,113,213,219,33,174,227,213,84,87,159,16,23,66,208,42,156,56,225,198,235,245,210,179,
123,183,198,31,162,226,50,117,206,39,148,16,30,65,171,183,51,165,134,234,38,222,33,20,226,211,9,113,187,149,118,255,27,250,36,37,113,203,173,19,91,173,252,101,207,175,108,149,114,163,45,22,162,45,150,
206,211,214,20,69,136,143,32,216,243,105,255,217,204,102,147,153,94,189,18,90,173,252,140,204,126,173,82,110,122,70,63,210,51,78,94,246,85,87,143,232,48,222,79,187,23,31,89,150,73,207,200,12,249,200,178,
28,198,184,225,183,7,120,116,254,147,103,244,201,115,217,229,3,24,53,246,154,118,213,32,106,59,208,171,20,189,226,19,72,207,200,36,214,106,13,217,102,50,153,232,21,159,64,159,164,164,144,182,17,252,119,
253,118,213,212,57,130,203,15,246,190,130,143,175,223,238,82,82,211,72,207,200,12,105,111,5,249,7,40,200,63,208,224,248,148,212,52,109,159,88,171,149,41,15,78,39,61,35,147,94,241,9,237,218,62,77,141,180,
250,39,144,15,125,7,227,100,239,100,180,5,150,152,24,22,63,179,156,131,249,121,117,66,242,200,195,56,28,118,76,38,19,115,231,63,137,217,100,70,81,20,162,45,22,114,115,178,248,199,83,79,54,40,39,37,245,
18,140,70,227,25,251,141,22,75,12,177,214,184,115,242,154,53,102,207,115,185,174,45,248,81,164,164,165,113,251,164,187,113,216,237,12,28,60,148,89,15,77,163,164,184,136,91,110,157,72,180,197,130,226,86,
248,98,215,103,252,187,184,152,135,255,246,40,0,46,167,147,148,191,166,49,249,206,219,0,152,124,239,20,92,46,23,178,44,51,125,230,108,110,185,241,119,33,215,233,178,254,3,152,120,231,93,76,127,224,190,
6,215,109,234,131,51,112,43,110,42,109,21,68,71,199,112,187,217,204,148,63,223,5,192,253,15,252,133,148,212,52,10,242,243,152,126,249,0,158,93,250,15,246,237,221,205,232,177,215,2,176,250,165,23,152,250,
224,12,156,78,39,14,199,81,100,217,200,148,7,167,51,249,206,219,200,200,252,13,150,152,24,210,51,127,67,73,113,33,223,150,151,181,115,83,73,141,139,207,169,230,249,156,109,5,117,216,237,220,127,207,164,
6,79,174,101,255,92,201,107,175,174,230,147,143,182,55,187,172,192,111,12,60,113,10,252,162,150,146,154,70,113,81,33,138,191,175,26,109,177,96,50,153,41,47,43,197,100,50,145,152,148,76,89,89,41,14,187,
93,45,11,159,250,223,115,120,2,182,246,96,223,83,248,81,218,3,8,160,184,184,144,145,99,174,225,217,165,139,193,7,142,163,118,158,120,124,174,102,195,88,107,28,83,252,109,231,209,249,79,210,55,61,131,220,
156,108,150,45,93,172,21,217,55,35,147,11,19,19,41,41,42,82,255,78,207,224,182,59,239,226,47,83,239,197,229,114,53,172,2,112,48,63,143,85,43,95,0,96,197,203,107,56,63,54,86,19,195,27,174,27,163,122,92,
59,51,185,118,252,4,246,238,249,146,192,138,141,62,159,218,114,74,138,11,181,227,215,190,181,129,158,209,209,108,221,188,145,89,115,230,177,106,229,138,14,225,157,54,214,222,218,253,139,165,41,169,105,
56,93,206,102,11,79,48,179,254,54,15,0,167,203,201,244,191,206,230,238,59,254,196,168,49,215,208,39,41,153,247,55,172,7,96,202,131,211,249,228,163,237,184,92,78,254,241,244,114,246,238,249,146,27,110,
250,35,7,11,242,120,109,205,106,17,64,106,67,236,254,7,0,64,110,118,22,191,191,241,143,117,127,231,102,133,116,141,227,227,19,66,130,200,177,113,113,144,147,205,221,127,190,31,107,92,28,209,150,24,226,
19,122,99,54,153,181,125,166,207,156,77,110,118,86,88,225,9,80,92,84,24,212,165,202,35,37,245,18,92,46,103,200,246,220,156,108,158,88,180,52,236,241,5,65,158,188,221,110,111,50,100,208,209,104,87,226,
19,109,177,132,52,160,41,247,76,34,37,237,146,16,67,143,26,123,13,86,107,28,46,167,147,183,214,189,222,100,12,169,87,66,2,147,239,184,85,109,104,127,157,197,192,193,67,121,239,221,245,76,255,235,108,222,
223,176,30,147,201,68,74,234,37,60,241,216,92,126,127,243,31,121,239,221,245,188,191,97,61,178,44,179,250,181,117,188,245,198,90,161,0,109,72,98,82,178,246,239,248,132,222,90,44,165,62,170,128,56,53,207,
39,88,148,18,147,146,153,254,192,125,154,231,17,204,253,247,76,226,137,191,47,97,212,216,107,248,96,243,166,70,235,240,249,206,207,180,127,191,191,97,61,138,226,14,169,91,159,164,36,10,242,194,215,77,
113,135,31,138,118,187,221,66,124,206,37,28,118,123,131,6,100,171,168,224,138,65,67,66,158,68,138,91,97,242,189,83,154,20,159,244,204,126,88,173,113,33,98,86,80,144,71,73,81,17,70,217,168,6,50,51,251,
177,107,231,14,20,69,225,210,254,3,48,155,204,218,40,132,221,110,199,100,54,11,5,104,99,30,157,255,36,7,243,243,24,53,102,28,203,150,46,10,187,79,165,205,70,89,105,41,179,254,54,143,207,119,237,192,106,
141,35,39,39,11,187,253,40,214,184,56,6,14,30,66,223,48,35,80,14,187,157,89,15,77,227,133,151,95,165,32,47,143,242,178,210,6,251,92,117,245,8,20,69,161,87,175,4,44,22,139,182,79,110,118,22,179,254,54,
143,130,130,60,174,187,126,2,171,94,106,89,23,42,55,59,139,219,39,221,205,174,157,59,180,110,160,16,159,115,140,220,156,44,102,205,153,71,172,213,74,165,205,70,73,81,17,46,167,243,164,199,21,23,21,134,
125,26,2,124,176,101,35,195,134,143,160,111,70,63,22,47,92,0,192,254,156,44,20,69,17,93,173,115,132,226,162,66,38,222,114,35,38,179,153,65,131,135,178,248,169,5,90,23,230,181,87,87,227,114,133,182,129,
71,31,153,201,192,193,67,73,76,74,166,184,168,144,202,138,10,92,46,23,115,31,153,201,160,193,67,249,244,163,237,188,189,110,173,214,118,238,159,124,167,38,64,247,223,115,39,178,108,12,91,143,101,75,23,
19,107,141,163,164,184,48,68,252,22,63,181,128,244,204,126,164,164,166,177,224,241,57,154,128,108,221,188,49,232,216,69,84,86,84,132,252,29,136,37,62,241,248,92,6,14,30,138,213,26,39,196,231,92,192,100,
54,115,251,164,187,181,191,223,122,99,45,14,187,157,197,79,45,224,217,231,95,226,131,45,27,41,200,207,227,210,254,3,154,229,69,21,23,21,242,196,223,151,240,193,150,141,88,173,113,236,221,179,155,242,178,
82,182,110,222,200,11,171,94,197,233,116,106,79,178,173,155,55,242,143,103,150,211,181,171,137,253,185,217,164,164,166,105,129,66,193,217,199,229,114,169,177,24,255,3,39,152,112,30,138,162,40,97,227,130,
37,69,69,97,111,238,220,156,236,16,207,169,49,20,183,91,139,15,214,63,223,190,61,187,217,183,103,119,3,47,44,248,220,245,235,18,220,62,195,149,219,145,208,3,25,191,74,188,96,92,240,198,115,241,101,67,
69,81,200,251,58,151,74,91,69,208,199,134,199,227,161,164,168,136,207,119,238,240,7,13,19,168,180,217,216,240,246,155,28,169,170,106,80,206,193,252,60,190,255,238,91,60,30,15,159,125,250,9,0,23,92,216,
7,155,205,70,137,127,148,75,81,20,14,124,157,203,135,219,182,106,193,70,151,203,197,238,47,118,113,254,249,231,243,63,191,252,37,251,246,236,230,72,85,21,14,187,157,255,252,187,164,201,160,100,155,139,
118,215,40,206,117,251,182,71,250,36,37,83,80,144,167,121,43,130,230,181,191,67,197,135,55,1,85,18,112,231,248,209,87,133,228,146,255,112,68,92,204,142,196,249,191,8,77,170,20,246,21,180,101,251,219,176,
245,147,73,64,190,152,195,89,208,110,169,245,120,240,212,122,58,102,151,196,160,199,160,215,119,104,251,181,155,36,67,193,169,211,81,237,235,58,118,28,215,177,106,186,117,51,171,191,41,36,145,86,7,120,
155,56,250,76,124,79,43,156,67,194,249,179,19,83,215,46,116,55,155,58,100,251,11,17,31,129,160,61,34,33,209,37,42,146,56,107,28,62,124,120,106,106,33,36,149,223,71,211,171,133,7,237,70,208,174,62,95,93,
57,129,34,66,182,249,15,168,191,79,232,29,87,175,46,52,171,30,122,191,96,81,138,0,0,28,104,73,68,65,84,131,158,50,119,13,82,39,176,159,16,31,65,59,127,172,170,255,59,118,188,154,138,242,111,209,27,12,
72,146,20,164,7,62,124,170,122,248,247,151,66,116,163,161,160,213,105,81,176,166,168,165,72,33,251,5,235,86,227,85,243,249,247,149,78,162,119,62,140,178,204,121,231,157,23,168,162,16,31,129,224,92,71,
39,73,56,28,255,229,191,63,254,136,193,104,36,210,40,135,127,153,81,146,192,235,5,157,206,47,50,62,255,102,9,9,73,21,24,73,2,159,132,215,235,5,201,167,149,227,243,169,231,209,202,241,249,37,68,242,59,
66,65,221,190,192,63,181,242,53,25,244,31,26,210,13,145,252,213,242,97,140,52,214,149,33,196,71,32,104,31,158,143,94,175,67,142,144,233,214,221,76,183,110,102,116,146,206,239,117,72,33,226,227,243,121,
209,73,58,36,73,194,235,243,250,183,249,208,235,116,32,73,232,144,240,225,197,227,241,106,229,34,73,120,60,94,36,80,203,149,84,193,82,95,14,245,225,245,120,144,116,122,36,212,185,177,125,254,46,151,20,
44,122,190,58,183,202,231,243,161,211,73,117,106,132,15,175,207,7,94,95,167,138,183,10,241,17,116,8,253,209,73,58,12,6,29,166,174,81,244,232,102,70,175,215,251,5,66,187,191,241,249,125,17,157,36,33,73,
18,58,157,14,143,199,163,121,55,122,189,94,245,120,252,222,144,215,171,138,132,78,167,3,159,143,90,143,7,73,39,161,247,139,151,199,191,175,42,56,18,58,29,90,121,234,121,37,240,121,85,9,148,212,137,255,
3,101,6,234,67,80,183,203,229,58,222,169,236,38,196,71,208,129,36,72,2,36,116,58,61,122,189,30,159,175,78,72,2,65,227,128,215,19,216,174,238,231,67,167,211,161,247,127,188,62,31,94,175,23,189,94,135,215,
235,197,235,245,162,147,36,109,232,91,242,119,219,130,69,43,32,66,129,242,180,238,149,4,58,41,208,189,146,208,235,85,143,73,210,73,117,221,49,159,207,223,245,235,92,163,204,98,26,85,65,7,65,82,93,28,73,
21,162,128,240,4,255,91,146,164,160,1,171,186,120,79,157,64,133,11,13,251,143,243,139,146,79,43,179,97,250,66,112,156,201,231,243,214,117,205,252,93,173,128,87,21,124,108,232,210,50,58,58,83,150,75,216,
60,31,65,7,243,9,58,129,125,125,106,48,37,100,60,42,48,105,156,36,233,252,2,36,249,5,72,210,60,21,157,78,135,78,167,83,247,163,97,160,87,239,23,135,64,92,200,227,245,226,241,248,130,186,102,222,16,145,
10,17,67,234,98,67,154,151,21,180,191,118,94,255,62,106,128,187,243,180,63,209,237,18,180,123,135,39,248,31,190,160,255,170,55,127,157,240,224,143,241,250,188,30,191,16,233,180,27,67,175,243,119,177,130,
60,20,0,143,63,128,28,232,90,249,124,62,53,248,44,161,197,149,60,30,143,38,36,117,49,159,192,140,133,117,2,164,117,225,252,251,226,171,19,48,85,184,164,78,101,58,33,62,130,246,31,234,169,243,51,144,36,
112,29,171,230,251,138,74,124,18,72,62,9,73,39,225,243,250,2,250,163,142,68,249,71,190,234,198,164,36,144,124,218,104,84,64,190,116,146,132,87,139,201,168,98,226,245,121,67,134,224,37,80,207,225,243,169,
251,227,67,242,213,13,182,7,60,42,41,48,44,239,223,106,208,235,249,229,255,196,97,54,117,213,196,167,211,117,187,36,73,180,225,14,237,28,116,22,251,74,62,124,62,9,167,235,24,199,142,87,243,11,75,52,17,
17,17,212,212,214,162,215,233,52,17,146,36,9,157,94,237,78,121,61,94,106,106,106,137,136,48,224,241,120,212,124,30,157,14,175,215,131,199,235,213,68,74,39,233,208,235,245,232,244,58,106,106,106,209,249,
149,204,171,6,116,212,64,118,96,100,204,31,243,169,245,120,208,75,122,144,64,175,211,83,235,243,32,249,212,99,60,181,30,116,122,29,158,218,218,208,174,91,39,106,127,194,243,17,116,168,62,152,26,115,150,
136,136,136,160,155,217,76,215,168,46,106,44,72,175,3,159,127,68,202,160,215,186,59,94,79,93,87,72,169,169,65,246,139,149,207,231,195,231,245,250,61,22,73,243,128,34,34,34,208,235,116,212,212,214,226,
118,187,49,248,51,170,117,58,29,114,68,132,150,251,227,169,245,168,195,250,58,29,110,183,27,163,209,72,77,77,13,58,255,16,125,96,132,45,34,34,66,187,65,131,131,222,162,219,37,16,180,171,216,143,218,103,
49,155,187,82,235,169,69,111,208,225,245,121,209,251,99,44,6,189,30,189,94,167,138,130,94,205,221,209,69,24,240,249,61,28,157,94,13,62,7,146,8,61,30,47,122,157,42,58,129,46,150,33,66,175,230,249,232,36,
53,1,209,47,120,90,80,217,160,211,226,60,30,143,58,92,31,25,105,68,146,116,68,26,101,124,72,232,60,181,126,193,81,69,50,48,34,38,225,235,84,189,16,33,62,130,142,228,247,160,147,116,116,51,155,233,222,
205,28,50,140,30,220,181,1,48,232,245,120,245,122,188,254,81,44,157,127,40,93,146,36,60,181,181,106,66,162,191,11,230,241,168,211,118,4,70,198,128,208,192,113,32,252,164,141,174,129,193,160,215,226,59,
6,189,94,75,72,172,59,198,71,96,244,45,80,22,157,44,232,44,242,124,4,29,3,127,100,55,240,38,67,132,193,128,20,52,76,30,60,2,21,8,0,123,181,104,177,164,5,148,213,46,154,62,200,123,169,27,201,10,94,239,
77,231,31,29,147,130,142,213,233,116,254,191,117,254,110,149,90,53,111,189,209,50,181,155,166,215,70,192,130,135,227,133,231,35,16,180,195,110,151,68,96,196,200,27,146,183,227,13,242,58,116,254,55,222,
9,74,10,212,105,121,54,146,63,232,236,11,201,7,2,117,88,93,231,207,108,38,100,120,28,237,88,175,127,8,62,224,253,120,189,117,229,4,11,85,176,8,5,188,168,186,115,117,50,207,199,31,176,215,62,130,14,230,
20,52,195,190,178,44,135,172,23,222,28,18,147,146,153,242,224,244,51,86,207,41,15,78,15,89,215,188,229,191,81,253,129,250,128,7,18,184,157,195,101,49,7,9,129,218,37,146,252,163,86,146,118,124,168,40,248,
223,225,10,18,139,96,111,200,231,245,170,239,108,17,20,60,14,58,79,176,247,21,120,249,52,248,245,12,205,131,210,53,79,124,212,53,222,251,145,30,102,201,159,166,136,182,88,206,184,205,130,215,40,107,73,
251,19,221,46,129,191,81,198,48,111,193,194,22,29,99,50,153,73,76,78,62,229,115,246,138,79,8,105,184,137,201,201,141,46,81,19,14,189,161,238,109,40,45,92,18,184,217,131,68,39,208,29,10,126,197,193,227,
79,248,171,243,72,234,142,13,188,231,165,211,233,66,186,75,129,248,79,64,112,130,179,163,67,186,80,65,30,140,65,111,104,32,98,170,7,166,171,19,28,255,249,165,22,244,185,162,45,49,252,99,217,114,110,191,
235,110,86,191,182,142,245,239,111,57,169,8,168,162,101,60,45,155,69,91,44,13,108,102,50,157,218,250,117,162,219,37,104,17,170,135,116,73,227,98,148,148,140,205,191,186,72,160,177,202,178,17,89,150,177,
88,98,180,239,100,89,230,150,91,39,226,176,219,49,153,204,228,230,100,133,60,209,235,47,57,28,182,241,234,245,72,120,26,4,127,130,3,204,129,0,113,96,202,140,218,218,90,205,211,8,116,185,164,160,119,186,
124,1,145,242,11,138,26,243,241,79,123,17,36,34,193,175,74,4,191,13,175,150,163,230,0,25,12,134,144,132,196,64,125,2,1,236,128,103,164,197,146,36,169,69,157,46,135,253,40,247,79,86,215,157,251,253,205,
127,100,202,180,233,218,223,245,9,120,72,54,91,69,88,123,6,219,204,100,50,107,11,98,90,173,113,216,237,71,41,47,43,85,109,246,167,137,40,138,130,201,100,14,89,33,54,61,163,31,138,226,14,89,254,89,136,
143,224,140,50,123,206,99,68,199,88,40,43,45,37,62,33,33,228,137,248,220,138,151,216,187,251,75,172,113,113,148,149,150,178,98,249,50,250,95,254,91,110,184,233,15,148,151,150,98,183,31,101,208,144,161,
60,60,99,154,38,84,149,182,10,20,69,209,196,103,234,180,25,216,108,21,152,77,102,100,163,204,195,51,166,53,222,120,13,186,32,215,222,167,102,49,251,189,32,189,78,79,77,109,109,136,247,131,20,24,30,175,
123,3,61,248,101,46,31,80,235,63,70,231,15,28,27,12,146,63,118,163,211,222,31,11,196,126,124,18,218,219,237,190,224,248,143,63,238,4,254,243,249,131,225,94,175,7,41,48,151,144,215,131,78,210,7,29,131,
230,125,157,10,46,167,19,199,209,240,171,146,204,155,191,80,179,89,98,82,50,138,226,214,132,231,217,21,47,81,144,119,0,139,37,6,187,253,40,203,150,46,86,187,211,211,166,55,176,89,192,211,113,57,157,40,
138,162,61,28,110,191,235,110,202,74,85,113,154,124,223,148,70,5,80,136,143,224,148,49,153,204,164,164,165,49,225,218,49,106,163,155,52,153,244,204,76,0,250,95,254,91,222,219,176,158,183,222,88,139,44,
203,188,254,246,187,172,90,89,183,68,240,220,71,102,2,80,89,105,99,224,224,161,172,90,185,130,226,162,66,246,231,100,135,172,226,185,111,239,110,222,122,99,45,0,235,223,223,130,201,100,110,176,250,104,
128,136,8,189,250,174,150,207,71,143,238,221,57,225,58,70,141,187,134,31,108,71,181,87,25,130,187,83,146,20,152,12,76,141,253,4,2,208,58,157,206,159,227,163,247,103,41,171,175,72,104,19,20,74,161,211,
176,214,122,124,120,188,30,127,128,59,48,75,80,64,116,8,204,137,232,31,45,83,189,40,117,207,192,203,101,234,247,30,143,71,11,126,251,240,225,243,130,100,104,190,248,168,139,104,78,38,214,106,197,26,23,
199,178,37,139,195,199,134,50,251,49,110,212,48,0,174,27,63,129,171,134,171,75,126,15,28,50,148,220,236,44,86,44,95,166,93,239,192,191,205,102,179,102,179,146,226,34,70,141,185,134,101,75,23,147,155,157,
77,165,173,34,196,102,185,217,217,154,173,95,127,251,93,162,45,150,102,173,101,38,196,71,208,108,210,51,251,145,147,157,21,212,232,178,52,241,185,172,255,0,122,37,36,48,112,240,16,205,189,15,184,238,229,
165,165,218,49,197,133,133,220,114,235,196,70,207,145,27,84,126,224,6,107,76,124,244,6,29,17,178,58,137,152,217,20,69,84,159,222,154,72,132,143,159,4,36,64,170,251,51,240,151,68,19,199,157,57,2,1,215,
186,23,79,125,161,65,88,157,58,41,89,176,87,215,24,138,91,33,55,59,139,203,46,87,87,232,13,215,77,77,76,74,14,233,30,229,100,103,105,226,115,89,255,1,164,166,93,66,74,90,154,102,179,248,132,222,0,148,
5,219,172,168,144,27,110,250,67,163,245,8,46,223,110,63,218,236,184,157,16,31,65,179,41,46,42,12,89,174,58,184,219,85,80,144,71,89,89,105,136,183,163,5,150,131,246,75,76,78,214,110,18,69,81,26,236,235,
116,58,155,93,31,79,173,7,217,32,225,58,230,64,242,129,164,15,140,24,129,116,14,14,89,251,124,62,188,158,192,219,237,170,167,83,27,232,46,250,188,120,124,32,121,37,60,181,53,72,242,201,215,236,82,20,55,
185,57,89,20,228,31,224,149,181,111,146,146,154,214,32,230,18,232,106,5,95,255,0,251,115,178,81,20,133,197,11,23,52,136,15,133,28,147,148,28,82,174,44,203,13,68,240,84,16,226,35,104,224,198,7,216,186,
121,163,22,132,4,168,180,85,224,114,57,153,62,115,54,229,101,165,140,26,115,141,230,149,124,252,225,54,94,92,245,42,178,44,179,111,207,110,82,210,210,120,109,205,106,0,44,150,24,102,205,153,71,121,89,
41,191,191,233,143,60,60,227,65,213,203,201,201,226,247,55,253,17,91,69,133,22,243,105,9,53,138,132,132,1,215,207,213,180,175,55,194,125,13,60,161,96,223,204,28,213,21,131,161,249,11,6,42,138,194,51,75,
22,49,125,230,108,238,190,253,79,33,162,30,8,220,207,154,51,143,131,249,121,154,215,19,176,217,45,183,78,100,202,131,211,249,124,231,103,12,28,60,132,101,75,213,174,155,108,148,153,53,103,30,37,197,69,
252,254,166,63,176,96,222,92,205,51,157,50,109,58,101,165,165,33,30,207,169,160,7,50,146,47,76,56,231,215,106,23,156,58,93,163,186,112,50,251,42,110,55,7,190,222,143,205,86,81,247,169,168,208,130,147,
193,130,228,241,120,208,235,245,60,255,220,50,10,242,243,176,219,143,82,125,252,56,31,127,184,13,107,92,28,241,9,9,252,223,190,189,84,85,253,64,98,82,50,30,143,135,143,63,220,142,53,46,142,23,159,95,206,
55,135,14,2,240,159,127,151,240,211,79,63,146,152,156,204,215,251,115,41,46,42,196,86,241,189,54,26,84,144,119,128,239,190,45,15,25,29,10,185,81,253,19,116,5,62,134,246,248,49,132,255,132,206,112,72,120,
123,237,207,165,170,234,7,0,190,251,246,91,138,139,10,113,58,157,13,108,246,241,135,219,81,20,5,89,150,121,249,197,21,154,205,60,30,15,59,62,249,136,30,61,123,114,193,133,23,146,147,157,205,119,223,150,
19,27,23,135,44,27,217,186,121,35,241,9,9,252,235,149,85,28,248,58,87,235,154,149,149,149,146,154,118,9,249,121,7,248,230,208,65,202,74,15,107,231,172,111,195,112,237,175,240,223,165,117,107,181,95,59,
98,72,200,90,237,85,71,29,226,142,237,64,156,23,19,77,91,217,119,244,216,113,164,103,246,99,193,188,57,194,16,237,40,182,55,122,236,184,51,102,179,250,237,239,253,237,159,213,173,213,46,133,9,203,9,58,
14,109,105,95,135,221,78,121,89,169,104,83,237,136,99,78,231,25,181,153,36,98,62,130,182,96,223,158,47,217,183,231,75,113,33,218,17,197,69,133,39,77,240,60,19,136,215,43,4,2,65,155,32,196,71,32,16,180,
9,103,188,219,117,203,109,183,115,221,248,9,76,24,55,186,213,42,29,107,141,227,158,251,166,208,203,159,16,149,155,157,197,178,37,139,90,237,124,189,18,18,184,231,190,169,152,76,102,100,163,145,226,194,
111,26,228,70,180,22,215,253,238,6,102,60,252,8,191,237,247,107,209,90,5,29,80,124,206,80,68,242,201,69,75,235,146,145,90,49,194,232,114,57,121,111,195,122,45,55,100,245,107,111,146,158,217,239,148,114,
69,154,131,195,110,231,249,231,158,161,188,172,84,251,157,3,135,12,229,243,157,59,90,213,56,189,226,19,24,53,118,28,182,138,239,79,239,122,138,17,5,65,91,34,157,133,110,215,51,75,22,241,218,154,85,173,
254,91,92,46,103,136,208,148,151,30,62,173,105,2,154,115,190,128,240,4,60,175,112,217,185,103,18,89,150,153,61,247,113,22,204,251,91,139,166,153,16,8,58,101,183,171,178,222,235,250,103,131,88,107,28,41,
105,151,240,204,210,69,173,122,158,196,164,100,174,29,63,129,140,204,223,240,218,154,85,173,62,130,115,251,164,201,124,242,209,54,202,203,74,177,196,196,136,150,42,16,226,115,46,17,107,141,99,201,178,
127,178,104,225,130,102,189,69,123,58,216,237,71,249,228,195,237,56,236,118,174,29,127,195,41,191,18,208,28,210,51,250,17,159,208,91,123,195,88,32,16,226,115,14,10,207,51,75,22,157,149,60,18,135,221,142,
195,110,39,55,39,11,69,81,184,110,252,132,86,19,159,41,211,102,0,240,236,138,151,180,109,207,174,120,137,185,179,255,218,234,34,43,16,8,241,57,135,132,167,87,124,130,22,243,81,103,218,203,12,153,114,224,
76,51,241,150,27,67,254,254,242,255,190,230,254,201,119,138,214,218,14,153,242,224,12,94,123,117,21,14,187,157,196,164,228,179,146,188,215,206,196,167,53,134,67,90,111,72,229,246,73,147,137,79,232,205,
146,101,255,212,182,229,100,255,95,179,103,80,107,41,131,134,92,201,45,183,78,196,86,81,65,82,242,69,108,217,244,62,171,86,190,192,217,29,54,18,195,93,167,251,0,81,20,229,172,199,37,213,121,169,35,1,137,
219,39,77,110,114,102,198,142,139,116,246,60,159,192,76,119,173,197,19,143,205,229,137,199,230,158,181,75,247,218,154,213,218,244,16,109,193,111,127,211,87,60,38,195,96,50,153,137,182,88,112,185,156,196,
199,247,166,32,255,0,138,162,144,146,154,134,44,27,41,46,42,196,229,114,34,203,50,119,76,154,76,65,65,30,197,133,133,90,119,185,87,124,2,22,75,140,118,92,99,231,8,76,61,26,60,167,77,224,28,129,249,141,
131,183,53,86,94,160,13,5,234,173,40,10,86,107,28,101,101,135,67,186,211,177,214,56,172,214,56,109,190,101,151,211,217,232,132,106,237,25,145,225,44,104,183,36,38,37,51,123,238,99,252,227,153,229,164,
103,246,67,81,20,70,143,29,199,85,87,143,32,61,179,31,175,172,93,167,142,134,166,94,66,175,132,4,250,36,38,145,158,169,78,164,62,122,236,56,102,60,60,155,244,204,126,188,176,234,213,176,203,6,37,38,37,
243,228,226,37,164,103,246,99,212,216,113,218,4,91,83,30,156,206,244,153,234,177,131,134,12,213,202,11,156,119,253,251,91,195,46,1,20,88,29,36,49,41,153,133,139,151,50,117,218,116,210,51,251,241,226,170,
87,181,217,8,19,147,146,121,110,197,74,6,14,30,194,236,185,243,120,114,209,146,102,173,74,33,98,62,130,118,77,77,77,13,165,223,217,56,98,255,239,89,61,111,76,207,30,252,79,220,249,68,117,137,108,241,177,
241,9,9,76,24,55,70,243,12,130,231,22,150,101,153,65,67,134,242,214,27,107,41,46,42,228,131,205,155,52,175,231,170,171,71,48,119,246,76,28,118,59,101,101,135,25,53,118,92,131,89,0,7,14,30,138,227,168,
61,100,118,70,89,150,25,54,124,164,54,39,114,128,224,243,154,76,38,210,51,251,241,201,135,219,27,173,119,172,53,142,219,254,120,35,138,162,80,86,118,152,129,131,135,178,111,207,110,6,14,30,202,91,235,
94,215,230,177,222,248,193,199,29,214,126,66,124,4,26,255,46,251,14,199,143,63,19,41,203,218,122,85,173,141,207,231,227,232,127,127,228,248,137,19,244,77,105,249,19,126,223,238,221,33,93,146,171,134,143,
96,208,160,161,68,199,88,176,90,227,216,186,121,83,131,99,2,147,170,7,175,83,86,92,88,24,166,155,180,138,91,110,189,157,215,223,126,151,189,187,191,100,197,242,101,164,103,246,11,59,131,223,85,195,71,
112,89,255,1,88,227,212,46,83,73,113,81,147,245,46,46,42,212,186,102,142,163,118,109,106,210,148,212,180,144,20,139,250,115,90,119,36,251,25,26,15,7,9,58,10,205,181,175,227,199,159,49,69,69,33,203,242,
73,103,210,59,83,120,189,94,34,35,35,27,204,11,220,92,220,65,179,246,153,76,102,166,62,56,131,9,215,142,70,81,20,166,207,156,29,42,58,70,245,28,138,162,80,86,122,152,5,243,230,54,25,128,86,20,133,85,43,
87,240,218,154,85,76,159,57,155,223,223,252,71,222,123,103,61,83,253,169,16,1,162,45,22,238,185,119,10,127,184,225,122,20,69,97,214,156,121,39,173,119,253,217,6,131,69,41,120,84,44,120,254,235,246,106,
63,49,159,143,224,164,232,80,23,186,59,91,13,23,234,22,245,235,210,165,203,105,151,165,40,110,20,197,205,176,225,35,49,153,213,64,241,190,61,187,1,117,178,244,91,110,157,168,174,248,144,147,197,251,27,
214,243,228,162,37,188,189,238,117,64,77,34,13,236,27,224,186,241,19,144,141,70,108,21,234,170,14,111,175,123,29,151,203,73,126,222,1,230,205,95,200,190,189,234,254,31,127,184,13,89,54,50,112,200,80,226,
227,123,19,159,208,155,253,57,217,167,244,27,62,223,185,131,217,115,31,195,173,184,73,207,232,135,53,46,174,195,218,79,15,100,252,170,79,111,49,135,115,7,198,212,140,57,156,1,190,183,85,97,52,26,207,106,
227,5,117,46,102,163,209,136,41,170,101,49,3,151,83,157,28,253,167,159,126,4,192,227,241,176,107,231,103,36,38,39,99,171,168,96,205,170,151,40,43,43,213,38,81,119,57,93,36,38,39,115,224,235,253,124,115,
232,32,37,69,133,92,244,171,20,109,36,171,254,136,210,241,227,199,233,18,21,197,121,231,159,207,134,183,223,212,188,145,207,119,126,198,127,254,93,194,69,23,95,204,129,175,247,115,164,170,138,47,191,216,
69,106,218,37,124,115,232,32,175,173,89,141,205,86,65,117,117,53,37,69,69,84,218,42,240,120,60,20,228,231,113,164,170,10,151,211,201,193,252,60,109,132,43,248,119,28,169,170,226,192,215,251,185,228,215,
233,124,246,233,199,244,232,222,147,3,95,231,114,164,170,234,164,215,227,92,181,95,253,246,247,77,73,208,28,206,215,143,28,26,50,135,243,15,98,14,231,14,197,249,245,230,208,109,204,190,251,178,15,96,54,
155,49,24,12,103,189,241,154,205,102,98,99,186,119,122,91,5,150,151,174,180,85,208,43,62,129,231,86,188,164,117,35,79,26,255,58,71,237,87,191,253,189,187,109,71,221,28,206,2,129,224,220,192,98,137,97,
202,180,233,106,12,231,168,157,69,11,23,180,250,12,10,109,133,161,201,136,144,160,99,32,236,219,110,40,46,42,108,181,76,253,115,173,253,137,36,67,129,64,208,38,8,241,17,8,4,66,124,4,2,129,16,31,129,64,
32,16,226,35,16,8,132,248,8,4,2,129,16,31,129,64,32,196,71,32,16,8,132,248,8,4,2,33,62,2,129,64,16,150,70,38,144,23,116,44,132,125,5,231,94,251,19,158,143,64,32,16,221,46,65,27,63,159,36,9,73,18,94,146,
176,159,16,31,193,89,38,50,50,178,205,26,239,217,158,0,75,216,175,237,237,39,230,243,17,104,196,89,99,169,62,225,198,227,241,156,213,243,70,68,68,240,139,152,24,240,186,133,17,58,145,253,194,78,32,47,
28,239,14,230,142,55,211,190,221,186,70,18,19,19,67,183,238,61,206,218,234,7,160,174,128,224,56,82,137,78,52,188,211,226,92,181,159,152,64,94,112,82,140,114,4,120,220,56,29,63,156,245,115,235,133,240,
116,58,251,137,142,182,64,32,104,19,132,248,8,0,117,33,189,112,75,6,11,206,125,187,165,103,244,35,61,163,159,16,31,65,251,36,218,18,195,163,65,43,120,10,218,143,221,22,47,91,206,196,187,238,102,245,107,
235,120,251,253,45,244,105,39,107,187,171,226,35,137,143,248,248,91,68,51,246,75,207,236,71,122,102,63,117,53,77,255,223,193,223,167,164,165,129,4,38,179,153,94,9,9,152,204,102,210,51,251,209,39,57,153,
232,24,139,182,159,201,108,166,79,114,50,72,234,106,162,193,101,138,79,243,63,14,251,81,166,220,51,137,137,255,123,19,111,175,123,93,93,253,162,145,125,83,210,210,66,174,115,192,86,193,182,13,216,163,
190,237,98,227,226,180,253,100,163,28,114,108,96,159,102,183,51,218,105,192,89,237,34,92,18,178,173,32,255,64,131,37,70,210,51,250,133,221,46,56,117,30,157,191,144,88,107,28,95,237,221,141,203,233,164,
188,172,148,103,87,188,196,21,151,246,13,217,231,134,235,198,144,152,148,204,196,73,119,3,234,138,161,118,251,81,210,51,250,241,232,35,51,1,184,238,119,19,232,218,213,76,73,81,33,79,252,125,169,182,116,
113,180,197,194,172,135,166,137,139,125,10,56,93,78,109,49,194,250,76,255,235,108,82,82,211,248,124,215,103,40,138,155,215,214,172,214,108,21,32,96,203,104,75,12,243,230,47,196,233,114,178,63,39,155,178,
210,195,140,26,51,142,233,15,220,11,192,168,177,227,232,21,159,64,65,126,30,139,159,94,174,217,174,87,66,2,83,238,105,222,234,27,237,82,124,162,45,49,44,126,102,57,5,249,7,234,26,252,35,51,113,216,237,
152,76,102,30,157,191,16,147,217,140,162,184,177,88,98,200,205,206,98,241,83,11,78,235,156,151,93,62,160,193,114,186,157,141,148,212,52,82,82,211,66,26,235,201,143,185,132,107,71,13,195,229,114,34,203,
50,183,252,105,34,38,147,25,151,203,201,168,49,227,248,203,212,123,73,73,77,195,97,63,170,217,104,197,203,175,18,107,141,107,114,29,117,65,29,38,179,153,219,39,77,38,54,214,74,108,92,28,203,150,46,110,
176,79,172,53,142,171,134,143,96,212,85,131,154,93,110,98,242,69,76,184,118,180,102,135,219,39,77,38,218,98,193,97,183,51,106,204,56,22,63,181,128,88,107,28,138,226,214,108,183,248,233,229,164,164,166,
81,144,159,215,49,197,7,234,92,205,250,30,209,179,207,175,228,181,87,87,243,201,71,219,155,117,51,21,228,231,145,146,154,134,221,110,167,210,86,65,180,197,66,124,124,111,236,246,163,148,151,149,2,48,112,
240,80,174,189,126,2,138,91,161,172,236,176,38,114,137,73,201,216,108,21,157,230,38,137,181,198,145,155,157,213,162,99,138,139,10,181,101,136,21,69,97,215,206,29,92,53,124,4,37,69,133,216,237,71,169,180,
85,48,122,236,56,210,51,251,177,236,249,149,254,253,220,196,39,36,8,241,105,38,138,91,33,55,59,139,75,251,15,0,160,196,191,172,115,48,214,83,176,93,126,222,129,16,27,124,242,209,118,6,14,30,74,110,118,
22,70,89,166,164,168,144,107,199,79,32,49,41,89,179,93,64,12,59,172,231,211,212,83,214,233,114,54,75,120,2,221,131,226,162,66,28,118,59,159,124,180,29,69,113,51,229,193,25,148,151,149,210,39,49,137,146,
226,34,222,122,99,45,3,7,13,193,98,177,144,158,217,15,167,255,70,122,246,249,151,216,187,231,75,172,214,56,202,74,75,121,225,159,203,58,124,35,175,180,85,144,24,38,152,105,63,122,84,243,102,234,7,59,21,
37,52,235,245,131,45,155,152,242,224,116,18,19,147,121,127,195,122,0,114,179,179,184,56,37,77,115,233,5,45,20,31,197,77,110,78,22,5,249,7,88,253,218,155,97,61,15,167,203,25,214,118,138,162,104,222,76,
172,53,174,73,219,189,183,225,109,30,157,191,16,139,37,134,247,222,93,239,15,119,228,97,179,85,52,187,171,213,33,196,39,218,18,19,162,182,83,238,153,68,74,90,26,197,65,170,63,106,236,56,172,214,56,92,
78,39,111,173,91,27,182,156,207,119,125,198,7,155,55,134,116,223,0,122,197,39,48,111,254,66,86,173,92,193,7,91,54,1,176,106,229,10,173,220,247,55,172,231,173,117,107,145,101,153,181,111,189,203,234,151,
86,180,251,216,82,192,125,15,176,117,243,198,144,39,95,65,126,30,78,151,147,89,127,155,71,65,65,30,138,162,240,193,230,141,124,190,115,7,83,30,156,78,65,65,30,3,7,13,109,242,58,4,158,202,233,153,253,88,
182,116,145,42,62,57,89,220,125,239,20,166,255,117,54,159,239,218,65,74,234,37,188,183,225,237,70,99,23,130,198,68,72,97,217,210,69,76,255,235,108,238,190,227,79,33,118,40,41,42,164,172,180,148,71,231,
47,100,223,94,53,124,240,193,230,141,236,250,108,7,83,30,156,65,110,78,22,151,245,31,128,211,249,115,19,189,13,59,138,91,97,212,152,107,152,120,203,77,90,185,178,108,100,214,223,230,241,201,199,219,73,
76,74,102,235,230,141,205,178,93,135,234,118,217,42,42,184,98,208,208,16,151,95,113,187,153,124,239,148,70,197,39,216,21,237,21,159,192,237,147,38,163,184,221,244,74,232,221,168,251,120,89,255,1,196,199,
39,112,197,224,33,234,121,109,21,152,204,230,118,125,179,56,236,71,153,53,35,52,200,235,114,58,27,236,55,229,158,73,164,164,170,35,38,123,253,49,176,197,79,45,96,224,224,161,88,173,113,60,241,248,28,162,
45,49,218,245,15,23,127,120,226,177,185,254,152,92,221,205,49,125,234,189,12,28,50,148,148,212,75,212,174,90,152,115,11,194,219,45,240,192,4,216,183,71,29,8,144,101,99,131,135,192,244,7,238,165,87,124,
2,131,134,12,213,108,247,194,63,151,105,182,91,182,116,177,214,230,29,246,163,97,109,183,248,169,5,152,204,102,173,43,173,182,137,59,185,106,248,200,22,219,174,67,117,187,114,115,178,152,53,231,49,45,
88,89,210,194,70,124,251,164,201,124,190,115,7,159,124,180,157,148,212,52,30,157,95,151,247,34,27,141,218,191,15,230,231,81,94,86,170,121,66,29,229,169,153,155,211,188,152,64,65,126,94,3,183,254,243,157,
59,66,158,144,0,46,151,51,108,252,161,210,86,1,182,208,109,46,151,51,196,3,21,52,223,110,245,109,209,84,176,183,188,172,148,215,214,172,110,212,118,1,187,40,138,210,108,219,5,60,224,51,210,237,210,235,
245,103,253,205,216,211,237,34,188,245,198,90,28,118,59,139,159,90,192,179,207,175,228,131,45,155,40,200,63,160,5,225,154,131,221,126,148,203,252,251,95,214,127,128,246,228,40,200,63,192,163,243,23,114,
213,213,35,200,205,201,226,227,143,182,241,194,203,175,18,17,33,243,213,222,221,164,164,165,53,48,232,185,66,184,41,22,218,131,125,5,29,131,166,166,248,208,3,233,191,74,236,61,46,120,99,77,109,45,181,
181,231,110,227,84,20,55,121,95,239,167,210,63,210,20,248,120,60,30,74,138,10,249,124,231,103,234,168,85,66,111,42,109,21,108,120,251,77,142,84,85,53,40,231,96,126,30,223,127,247,173,118,35,238,207,201,
66,111,48,96,177,196,240,214,155,107,217,183,119,55,14,187,29,143,199,195,238,47,118,145,154,118,9,182,138,10,42,109,54,62,249,104,59,113,113,255,143,248,132,222,100,125,181,47,108,249,231,2,114,68,4,
93,34,141,180,39,251,10,58,14,225,218,223,161,226,195,155,128,31,36,96,226,248,209,87,174,10,254,178,250,132,155,159,157,46,113,229,58,0,93,163,186,96,234,26,133,176,175,224,92,105,127,27,182,126,122,
7,144,111,0,106,165,122,51,110,68,69,70,226,62,161,160,212,212,136,171,215,142,209,235,245,152,162,162,16,246,21,156,75,237,15,240,0,94,29,160,124,247,125,197,143,245,191,53,155,186,18,17,33,166,251,105,
207,134,239,222,205,212,104,159,91,216,87,208,22,237,175,162,162,162,22,80,0,143,14,56,246,227,79,63,255,187,254,193,6,131,158,232,30,221,49,117,141,18,141,180,29,97,48,24,136,138,234,130,165,103,119,
34,12,134,38,246,19,246,21,156,253,246,231,241,250,190,5,170,1,197,0,56,13,178,49,191,162,162,34,51,46,46,46,108,159,173,107,84,23,113,85,59,112,159,92,216,87,112,182,168,241,120,15,1,78,224,132,30,232,
121,196,254,223,170,40,217,240,27,79,109,109,79,115,51,223,203,16,8,4,130,150,240,245,129,188,35,255,254,214,182,216,227,241,126,7,56,244,64,36,32,69,118,233,82,89,171,156,184,200,83,91,219,221,233,116,
34,68,72,32,16,156,9,42,42,42,248,207,225,210,35,39,106,121,230,191,63,57,15,0,71,0,167,4,88,128,158,192,249,128,245,162,11,126,121,139,167,70,201,168,81,220,191,20,151,77,32,16,156,14,134,8,217,102,144,
141,249,135,191,171,124,213,173,212,148,3,63,0,14,192,37,1,81,254,143,217,47,66,61,128,110,254,109,50,106,34,162,88,91,64,32,16,52,23,31,234,112,186,130,26,92,118,2,63,2,255,5,126,6,142,3,238,255,15,177,
173,18,134,62,227,250,56,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* OperatorEditor::operatorEditor_287x218_png = (const char*) resource_OperatorEditor_operatorEditor_287x218_png;
const int OperatorEditor::operatorEditor_287x218_pngSize = 15614;


//[EndFile] You can add extra defines here...
//[/EndFile]
