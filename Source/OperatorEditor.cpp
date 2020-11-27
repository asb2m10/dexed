/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "OperatorEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#ifndef M_LN10
#define M_LN10 2.30258509299404568402
#endif

class OperatorSwitch : public ToggleButton {
    Image image;
    SharedResourcePointer<DXLookNFeel> lookAndFeel;
public :
    OperatorSwitch() : ToggleButton("opSwitch") {
        image = lookAndFeel->imageSwitchOperator;
        setSize(32, 32);
    }

    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown) {
        g.drawImage(image, 0, 0, 32, 32, 0, getToggleState() ? 0 : 32, 32, 32);
    }
};

//[/MiscUserDefs]

//==============================================================================
OperatorEditor::OperatorEditor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    s_egl1.reset (new juce::Slider ("egl1"));
    addAndMakeVisible (s_egl1.get());
    s_egl1->setRange (0, 99, 1);
    s_egl1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s_egl1->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_egl1->addListener (this);

    s_egl1->setBounds (5, 128, 34, 34);

    s_egl2.reset (new juce::Slider ("egl2"));
    addAndMakeVisible (s_egl2.get());
    s_egl2->setRange (0, 99, 1);
    s_egl2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s_egl2->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_egl2->addListener (this);

    s_egl2->setBounds (33, 129, 34, 34);

    s_egl3.reset (new juce::Slider ("egl3"));
    addAndMakeVisible (s_egl3.get());
    s_egl3->setRange (0, 99, 1);
    s_egl3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s_egl3->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_egl3->addListener (this);

    s_egl3->setBounds (61, 128, 34, 34);

    s_egl4.reset (new juce::Slider ("egl4"));
    addAndMakeVisible (s_egl4.get());
    s_egl4->setRange (0, 99, 1);
    s_egl4->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s_egl4->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_egl4->addListener (this);

    s_egl4->setBounds (89, 128, 34, 34);

    s_egv1.reset (new juce::Slider ("egr1"));
    addAndMakeVisible (s_egv1.get());
    s_egv1->setRange (0, 99, 1);
    s_egv1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s_egv1->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_egv1->addListener (this);

    s_egv1->setBounds (5, 169, 34, 34);

    s_egv2.reset (new juce::Slider ("egr3"));
    addAndMakeVisible (s_egv2.get());
    s_egv2->setRange (0, 99, 1);
    s_egv2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s_egv2->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_egv2->addListener (this);

    s_egv2->setBounds (33, 169, 34, 34);

    s_egv3.reset (new juce::Slider ("egr3"));
    addAndMakeVisible (s_egv3.get());
    s_egv3->setRange (0, 99, 1);
    s_egv3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s_egv3->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_egv3->addListener (this);

    s_egv3->setBounds (61, 169, 34, 34);

    s_egv4.reset (new juce::Slider ("egr4"));
    addAndMakeVisible (s_egv4.get());
    s_egv4->setRange (0, 99, 1);
    s_egv4->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s_egv4->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    s_egv4->addListener (this);

    s_egv4->setBounds (89, 169, 34, 34);

    opLevel.reset (new juce::Slider ("opLevel"));
    addAndMakeVisible (opLevel.get());
    opLevel->setRange (0, 99, 1);
    opLevel->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    opLevel->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    opLevel->addListener (this);

    opLevel->setBounds (245, 76, 34, 34);

    opFine.reset (new juce::Slider ("opFine"));
    addAndMakeVisible (opFine.get());
    opFine->setRange (0, 99, 1);
    opFine->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    opFine->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    opFine->addListener (this);

    opFine->setBounds (78, 24, 34, 34);

    opCoarse.reset (new juce::Slider ("opCoarse"));
    addAndMakeVisible (opCoarse.get());
    opCoarse->setRange (0, 31, 1);
    opCoarse->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    opCoarse->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    opCoarse->addListener (this);

    opCoarse->setBounds (43, 24, 34, 34);

    khzDisplay.reset (new juce::Label ("khz",
                                       TRANS("1,000 kHz")));
    addAndMakeVisible (khzDisplay.get());
    khzDisplay->setFont (juce::Font (12.60f, juce::Font::plain).withTypefaceStyle ("Regular"));
    khzDisplay->setJustificationType (juce::Justification::centred);
    khzDisplay->setEditable (false, false, false);
    khzDisplay->setColour (juce::Label::backgroundColourId, juce::Colour (0x6a000000));
    khzDisplay->setColour (juce::Label::textColourId, juce::Colours::white);
    khzDisplay->setColour (juce::Label::outlineColourId, juce::Colour (0x00000000));
    khzDisplay->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    khzDisplay->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    khzDisplay->setBounds (15, 10, 95, 10);

    detune.reset (new juce::Slider ("detune"));
    addAndMakeVisible (detune.get());
    detune->setRange (-7, 7, 1);
    detune->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    detune->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    detune->addListener (this);

    detune->setBounds (6, 24, 34, 34);

    envDisplay.reset (new EnvDisplay());
    addAndMakeVisible (envDisplay.get());
    envDisplay->setName ("envDisplay");

    envDisplay->setBounds (16, 83, 94, 30);

    sclLeftLevel.reset (new juce::Slider ("sclLeftLevel"));
    addAndMakeVisible (sclLeftLevel.get());
    sclLeftLevel->setTooltip (TRANS("Keyboard Scale Level Left Depth "));
    sclLeftLevel->setRange (0, 99, 1);
    sclLeftLevel->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sclLeftLevel->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    sclLeftLevel->addListener (this);

    sclLeftLevel->setBounds (131, 115, 34, 34);

    sclRightLevel.reset (new juce::Slider ("sclRightLevel"));
    addAndMakeVisible (sclRightLevel.get());
    sclRightLevel->setTooltip (TRANS("Keyboard Scale Level Right Depth "));
    sclRightLevel->setRange (0, 99, 1);
    sclRightLevel->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sclRightLevel->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    sclRightLevel->addListener (this);

    sclRightLevel->setBounds (241, 115, 34, 34);

    sclLvlBrkPt.reset (new juce::Slider ("sclLvlBrkPt"));
    addAndMakeVisible (sclLvlBrkPt.get());
    sclLvlBrkPt->setTooltip (TRANS("Scale Level Breakpoint"));
    sclLvlBrkPt->setRange (0, 99, 1);
    sclLvlBrkPt->setSliderStyle (juce::Slider::LinearHorizontal);
    sclLvlBrkPt->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    sclLvlBrkPt->addListener (this);

    sclLvlBrkPt->setBounds (178, 130, 54, 24);

    sclRateScaling.reset (new juce::Slider ("sclRateScaling"));
    addAndMakeVisible (sclRateScaling.get());
    sclRateScaling->setTooltip (TRANS("Keyboard Rate Scaling"));
    sclRateScaling->setRange (0, 7, 1);
    sclRateScaling->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sclRateScaling->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    sclRateScaling->addListener (this);

    sclRateScaling->setBounds (186, 179, 34, 34);

    keyVelSens.reset (new juce::Slider ("keyVelSens"));
    addAndMakeVisible (keyVelSens.get());
    keyVelSens->setRange (0, 7, 1);
    keyVelSens->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    keyVelSens->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    keyVelSens->addListener (this);

    keyVelSens->setBounds (204, 76, 34, 34);

    ampModSens.reset (new juce::Slider ("ampModSens"));
    addAndMakeVisible (ampModSens.get());
    ampModSens->setRange (0, 3, 1);
    ampModSens->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    ampModSens->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    ampModSens->addListener (this);

    ampModSens->setBounds (140, 76, 34, 34);

    vu.reset (new VuMeter());
    addAndMakeVisible (vu.get());
    vu->setName ("vu");

    vu->setBounds (132, 52, 140, 8);

    opMode.reset (new juce::ToggleButton ("opMode"));
    addAndMakeVisible (opMode.get());
    opMode->setButtonText (juce::String());
    opMode->addListener (this);

    opMode->setBounds (146, 19, 48, 26);

    kbdLeftCurve.reset (new ComboBoxImage());
    addAndMakeVisible (kbdLeftCurve.get());
    kbdLeftCurve->setName ("kbdLeftCurve");

    kbdLeftCurve->setBounds (128, 170, 36, 26);

    kbdRightCurve.reset (new ComboBoxImage());
    addAndMakeVisible (kbdRightCurve.get());
    kbdRightCurve->setName ("kbdRightCurve");

    kbdRightCurve->setBounds (240, 170, 36, 26);


    //[UserPreSize]
    opSwitch.reset(new OperatorSwitch());
    opSwitch->addListener(this);
    addAndMakeVisible(opSwitch.get());
    //[/UserPreSize]

    setSize (287, 218);


    //[Constructor] You can add your own custom stuff here..
    SharedResourcePointer<DXLookNFeel> lookAndFeel;

    light = lookAndFeel->imageLight;
    Image tmp = lookAndFeel->imageScaling;

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

    background = lookAndFeel->imageOperator;

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
    opSwitch = nullptr;
    //[/Destructor]
}

//==============================================================================
void OperatorEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.drawImage(background, 0, 0, 287, 218, 0, 0, 287, 218);
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..

    if ( opSwitch->getToggleState() )
        g.setColour(Colours::white);
    else
        g.setColour(DXLookNFeel::roundBackground);

    g.setFont(Font (30.00f, Font::plain));
    g.drawText(opNum, 250, 14, 30, 30, Justification::centred, true);

    bool state = opMode->getToggleState();

    // 129 x 24
    g.drawImage(light, 127, 24, 14, 14, 0, state ? 0 : 14, 14, 14);
    // 199 x 24
    g.drawImage(light, 198, 24, 14, 14, 0, !state ? 0 : 14, 14, 14);
    //[/UserPaint]
}

void OperatorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    opSwitch->setBounds(226, 13, 64, 32);
    //[/UserResized]
}

void OperatorEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == s_egl1.get())
    {
        //[UserSliderCode_s_egl1] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egl1]
    }
    else if (sliderThatWasMoved == s_egl2.get())
    {
        //[UserSliderCode_s_egl2] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egl2]
    }
    else if (sliderThatWasMoved == s_egl3.get())
    {
        //[UserSliderCode_s_egl3] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egl3]
    }
    else if (sliderThatWasMoved == s_egl4.get())
    {
        //[UserSliderCode_s_egl4] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egl4]
    }
    else if (sliderThatWasMoved == s_egv1.get())
    {
        //[UserSliderCode_s_egv1] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egv1]
    }
    else if (sliderThatWasMoved == s_egv2.get())
    {
        //[UserSliderCode_s_egv2] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egv2]
    }
    else if (sliderThatWasMoved == s_egv3.get())
    {
        //[UserSliderCode_s_egv3] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egv3]
    }
    else if (sliderThatWasMoved == s_egv4.get())
    {
        //[UserSliderCode_s_egv4] -- add your slider handling code here..
        envDisplay->repaint();
        //[/UserSliderCode_s_egv4]
    }
    else if (sliderThatWasMoved == opLevel.get())
    {
        //[UserSliderCode_opLevel] -- add your slider handling code here..
        //[/UserSliderCode_opLevel]
    }
    else if (sliderThatWasMoved == opFine.get())
    {
        //[UserSliderCode_opFine] -- add your slider handling code here..
        updateDisplay();
        //[/UserSliderCode_opFine]
    }
    else if (sliderThatWasMoved == opCoarse.get())
    {
        //[UserSliderCode_opCoarse] -- add your slider handling code here..
        updateDisplay();
        //[/UserSliderCode_opCoarse]
    }
    else if (sliderThatWasMoved == detune.get())
    {
        //[UserSliderCode_detune] -- add your slider handling code here..
        updateDisplay();
        //[/UserSliderCode_detune]
    }
    else if (sliderThatWasMoved == sclLeftLevel.get())
    {
        //[UserSliderCode_sclLeftLevel] -- add your slider handling code here..
        //[/UserSliderCode_sclLeftLevel]
    }
    else if (sliderThatWasMoved == sclRightLevel.get())
    {
        //[UserSliderCode_sclRightLevel] -- add your slider handling code here..
        //[/UserSliderCode_sclRightLevel]
    }
    else if (sliderThatWasMoved == sclLvlBrkPt.get())
    {
        //[UserSliderCode_sclLvlBrkPt] -- add your slider handling code here..
        //[/UserSliderCode_sclLvlBrkPt]
    }
    else if (sliderThatWasMoved == sclRateScaling.get())
    {
        //[UserSliderCode_sclRateScaling] -- add your slider handling code here..
        //[/UserSliderCode_sclRateScaling]
    }
    else if (sliderThatWasMoved == keyVelSens.get())
    {
        //[UserSliderCode_keyVelSens] -- add your slider handling code here..
        //[/UserSliderCode_keyVelSens]
    }
    else if (sliderThatWasMoved == ampModSens.get())
    {
        //[UserSliderCode_ampModSens] -- add your slider handling code here..
        //[/UserSliderCode_ampModSens]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void OperatorEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == opMode.get())
    {
        //[UserButtonCode_opMode] -- add your button handler code here..
        repaint();
        //[/UserButtonCode_opMode]
    }

    //[UserbuttonClicked_Post]
    if (buttonThatWasClicked == opSwitch.get()) {
        this->processor->forceRefreshUI = true;
    }
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OperatorEditor::bind(DexedAudioProcessor *parent, int op) {
    parent->opCtrl[op].egLevel[0]->bind(s_egl1.get());
    parent->opCtrl[op].egLevel[1]->bind(s_egl2.get());
    parent->opCtrl[op].egLevel[2]->bind(s_egl3.get());
    parent->opCtrl[op].egLevel[3]->bind(s_egl4.get());
    parent->opCtrl[op].egRate[0]->bind(s_egv1.get());
    parent->opCtrl[op].egRate[1]->bind(s_egv2.get());
    parent->opCtrl[op].egRate[2]->bind(s_egv3.get());
    parent->opCtrl[op].egRate[3]->bind(s_egv4.get());
    parent->opCtrl[op].level->bind(opLevel.get());
    parent->opCtrl[op].opMode->bind(opMode.get());
    parent->opCtrl[op].fine->bind(opFine.get());
    parent->opCtrl[op].coarse->bind(opCoarse.get());
    parent->opCtrl[op].detune->bind(detune.get());
    parent->opCtrl[op].sclBrkPt->bind(sclLvlBrkPt.get());
    parent->opCtrl[op].sclLeftCurve->bind(kbdLeftCurve.get());
    parent->opCtrl[op].sclRightCurve->bind(kbdRightCurve.get());
    parent->opCtrl[op].sclLeftDepth->bind(sclLeftLevel.get());
    parent->opCtrl[op].sclRightDepth->bind(sclRightLevel.get());
    parent->opCtrl[op].sclRate->bind(sclRateScaling.get());
    parent->opCtrl[op].ampModSens->bind(ampModSens.get());
    parent->opCtrl[op].velModSens->bind(keyVelSens.get());
    parent->opCtrl[op].opSwitch->bind(opSwitch.get());

    int offset = parent->opCtrl[op].egRate[0]->getOffset();
    envDisplay->pvalues = &(parent->data[offset]);
    processor = parent;

    opNum << op + 1;
    internalOp = 5-op;
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
        txtFreq << "f = " << (freq + (freq * (fine/100)));
    } else {
        freq = pow(10,((int)freq)&3);
        freq = freq * exp(M_LN10*(fine/100));
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

void OperatorEditor::mouseDown(const MouseEvent &event) {
    if ( event.mods.isPopupMenu()) {
        PopupMenu popup;

        popup.addItem(1, "Copy Operator Values");
        popup.addItem(2, "Paste Envelope Values", processor->hasClipboardContent());
        popup.addItem(3, "Paste Operator Values", processor->hasClipboardContent());
        popup.addSeparator();
        popup.addItem(4, "Send current program to DX7");

        switch(popup.show()) {
            case 1:
                processor->copyToClipboard(internalOp);
            break;

            case 2:
                processor->pasteEnvFromClipboard(internalOp);
            break;

            case 3:
                processor->pasteOpFromClipboard(internalOp);
            break;

            case 4:
                processor->sendCurrentSysexProgram();
            break;
        }

    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OperatorEditor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="2" snapActive="0" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="287" initialHeight="218">
  <BACKGROUND backgroundColour="ffffff"/>
  <SLIDER name="egl1" id="dc070cc41347df47" memberName="s_egl1" virtualName=""
          explicitFocusOrder="0" pos="5 128 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egl2" id="66f5195e9c374029" memberName="s_egl2" virtualName=""
          explicitFocusOrder="0" pos="33 129 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egl3" id="9d57bd53203dcdb4" memberName="s_egl3" virtualName=""
          explicitFocusOrder="0" pos="61 128 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egl4" id="4f7c3ece3ea2cf9c" memberName="s_egl4" virtualName=""
          explicitFocusOrder="0" pos="89 128 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egr1" id="2ca8137d80da46fb" memberName="s_egv1" virtualName=""
          explicitFocusOrder="0" pos="5 169 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egr3" id="4ad6d0c532d15973" memberName="s_egv2" virtualName=""
          explicitFocusOrder="0" pos="33 169 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egr3" id="8a2027f9ede16b4f" memberName="s_egv3" virtualName=""
          explicitFocusOrder="0" pos="61 169 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egr4" id="8c04f1c943d837e8" memberName="s_egv4" virtualName=""
          explicitFocusOrder="0" pos="89 169 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="opLevel" id="f8521c8214fb8993" memberName="opLevel" virtualName=""
          explicitFocusOrder="0" pos="245 76 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="opFine" id="e445aa61bd6cddcb" memberName="opFine" virtualName=""
          explicitFocusOrder="0" pos="78 24 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="opCoarse" id="4eec63d30d7488d2" memberName="opCoarse" virtualName=""
          explicitFocusOrder="0" pos="43 24 34 34" min="0.0" max="31.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="khz" id="eb961eed8902a6fc" memberName="khzDisplay" virtualName=""
         explicitFocusOrder="0" pos="15 10 95 10" bkgCol="6a000000" textCol="ffffffff"
         outlineCol="0" edTextCol="ff000000" edBkgCol="0" labelText="1,000 kHz"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.6" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <SLIDER name="detune" id="f093ec8defca2fc2" memberName="detune" virtualName=""
          explicitFocusOrder="0" pos="6 24 34 34" min="-7.0" max="7.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="envDisplay" id="b18856de924c6340" memberName="envDisplay"
                    virtualName="" explicitFocusOrder="0" pos="16 83 94 30" class="EnvDisplay"
                    params=""/>
  <SLIDER name="sclLeftLevel" id="bd6f338ae68e454f" memberName="sclLeftLevel"
          virtualName="" explicitFocusOrder="0" pos="131 115 34 34" tooltip="Keyboard Scale Level Left Depth "
          min="0.0" max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="sclRightLevel" id="b9e23ed5187fc7e5" memberName="sclRightLevel"
          virtualName="" explicitFocusOrder="0" pos="241 115 34 34" tooltip="Keyboard Scale Level Right Depth "
          min="0.0" max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="sclLvlBrkPt" id="c563ac3116923bbc" memberName="sclLvlBrkPt"
          virtualName="" explicitFocusOrder="0" pos="178 130 54 24" tooltip="Scale Level Breakpoint"
          min="0.0" max="99.0" int="1.0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="sclRateScaling" id="f0d17c8e09aa4c49" memberName="sclRateScaling"
          virtualName="" explicitFocusOrder="0" pos="186 179 34 34" tooltip="Keyboard Rate Scaling"
          min="0.0" max="7.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="keyVelSens" id="21795d045d07602b" memberName="keyVelSens"
          virtualName="" explicitFocusOrder="0" pos="204 76 34 34" min="0.0"
          max="7.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="ampModSens" id="634ceaa7b0f81a6c" memberName="ampModSens"
          virtualName="" explicitFocusOrder="0" pos="140 76 34 34" min="0.0"
          max="3.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <GENERICCOMPONENT name="vu" id="6f952594ea99dc1e" memberName="vu" virtualName=""
                    explicitFocusOrder="0" pos="132 52 140 8" class="VuMeter" params=""/>
  <TOGGLEBUTTON name="opMode" id="15db8e5c9dd13966" memberName="opMode" virtualName=""
                explicitFocusOrder="0" pos="146 19 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="kbdLeftCurve" id="ee4e60dfcd6cb790" memberName="kbdLeftCurve"
                    virtualName="" explicitFocusOrder="0" pos="128 170 36 26" class="ComboBoxImage"
                    params=""/>
  <GENERICCOMPONENT name="kbdRightCurve" id="f5ca44d67593488a" memberName="kbdRightCurve"
                    virtualName="" explicitFocusOrder="0" pos="240 170 36 26" class="ComboBoxImage"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

