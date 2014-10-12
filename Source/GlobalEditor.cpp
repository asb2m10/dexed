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
#include "PluginEditor.h"
//[/Headers]

#include "GlobalEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GlobalEditor::GlobalEditor ()
{
    addAndMakeVisible (lfoSpeed = new Slider ("lfoSpeed"));
    lfoSpeed->setRange (0, 99, 1);
    lfoSpeed->setSliderStyle (Slider::Rotary);
    lfoSpeed->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    lfoSpeed->addListener (this);

    addAndMakeVisible (lfoAmDepth = new Slider ("lfoAmDepth"));
    lfoAmDepth->setRange (0, 99, 1);
    lfoAmDepth->setSliderStyle (Slider::RotaryVerticalDrag);
    lfoAmDepth->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    lfoAmDepth->addListener (this);

    addAndMakeVisible (lfoPitchDepth = new Slider ("lfoPitchDepth"));
    lfoPitchDepth->setRange (0, 99, 1);
    lfoPitchDepth->setSliderStyle (Slider::RotaryVerticalDrag);
    lfoPitchDepth->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    lfoPitchDepth->addListener (this);

    addAndMakeVisible (lfoDelay = new Slider ("lfoDelay"));
    lfoDelay->setRange (0, 99, 1);
    lfoDelay->setSliderStyle (Slider::RotaryVerticalDrag);
    lfoDelay->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    lfoDelay->addListener (this);

    addAndMakeVisible (cutoff = new Slider ("cutoff"));
    cutoff->setRange (0, 1, 0);
    cutoff->setSliderStyle (Slider::RotaryVerticalDrag);
    cutoff->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    cutoff->addListener (this);

    addAndMakeVisible (reso = new Slider ("reso"));
    reso->setRange (0, 1, 0);
    reso->setSliderStyle (Slider::RotaryVerticalDrag);
    reso->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    reso->addListener (this);

    addAndMakeVisible (pitchRate2 = new Slider ("pitchRate2"));
    pitchRate2->setRange (0, 99, 1);
    pitchRate2->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchRate2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchRate2->addListener (this);

    addAndMakeVisible (pitchRate3 = new Slider ("pitchRate3"));
    pitchRate3->setRange (0, 99, 1);
    pitchRate3->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchRate3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchRate3->addListener (this);

    addAndMakeVisible (pitchRate4 = new Slider ("pitchRate4"));
    pitchRate4->setRange (0, 99, 1);
    pitchRate4->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchRate4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchRate4->addListener (this);

    addAndMakeVisible (pitchRate1 = new Slider ("pitchRate1"));
    pitchRate1->setRange (0, 99, 1);
    pitchRate1->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchRate1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchRate1->addListener (this);

    addAndMakeVisible (pitchLevel2 = new Slider ("pitchLevel2"));
    pitchLevel2->setRange (0, 99, 1);
    pitchLevel2->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchLevel2->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchLevel2->addListener (this);

    addAndMakeVisible (pitchLevel3 = new Slider ("pitchLevel3"));
    pitchLevel3->setRange (0, 99, 1);
    pitchLevel3->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchLevel3->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchLevel3->addListener (this);

    addAndMakeVisible (pitchLevel4 = new Slider ("pitchLevel4"));
    pitchLevel4->setRange (0, 99, 1);
    pitchLevel4->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchLevel4->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchLevel4->addListener (this);

    addAndMakeVisible (pitchLevel1 = new Slider ("pitchLevel1"));
    pitchLevel1->setRange (0, 99, 1);
    pitchLevel1->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchLevel1->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchLevel1->addListener (this);

    addAndMakeVisible (transpose = new Slider ("transpose"));
    transpose->setRange (0, 48, 0);
    transpose->setSliderStyle (Slider::RotaryVerticalDrag);
    transpose->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    transpose->addListener (this);

    addAndMakeVisible (oscSync = new ToggleButton ("oscSync"));
    oscSync->setButtonText (String::empty);

    addAndMakeVisible (pitchModSens = new Slider ("pitchModSens"));
    pitchModSens->setRange (0, 7, 0);
    pitchModSens->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchModSens->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchModSens->addListener (this);

    addAndMakeVisible (lfoSync = new ToggleButton ("lfoSync"));
    lfoSync->setButtonText (String::empty);

    addAndMakeVisible (pitchEnvDisplay = new PitchEnvDisplay());
    pitchEnvDisplay->setName ("pitchEnvDisplay");

    addAndMakeVisible (algoDisplay = new AlgoDisplay());
    algoDisplay->setName ("algoDisplay");

    addAndMakeVisible (feedback = new Slider ("feedback"));
    feedback->setRange (0, 7, 1);
    feedback->setSliderStyle (Slider::RotaryVerticalDrag);
    feedback->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    feedback->addListener (this);

    addAndMakeVisible (algo = new Slider ("algo"));
    algo->setRange (1, 32, 1);
    algo->setSliderStyle (Slider::RotaryVerticalDrag);
    algo->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    algo->addListener (this);

    addAndMakeVisible (lcdDisplay = new LcdDisplay());
    lcdDisplay->setName ("lcdDisplay");

    addAndMakeVisible (output = new Slider ("cutoff"));
    output->setRange (0, 1, 0);
    output->setSliderStyle (Slider::RotaryVerticalDrag);
    output->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    output->addListener (this);

    addAndMakeVisible (vuOutput = new VuMeter());
    vuOutput->setName ("vuOutput");

    addAndMakeVisible (initButton = new TextButton ("initButton"));
    initButton->setButtonText (TRANS("INIT"));
    initButton->addListener (this);

    addAndMakeVisible (parmButton = new TextButton ("parmButton"));
    parmButton->setButtonText (TRANS("PARM"));
    parmButton->addListener (this);

    addAndMakeVisible (sendButton = new TextButton ("sendButton"));
    sendButton->setButtonText (TRANS("SEND"));
    sendButton->addListener (this);

    addAndMakeVisible (cartButton = new TextButton ("cartButton"));
    cartButton->setButtonText (TRANS("CART"));
    cartButton->addListener (this);

    addAndMakeVisible (loadButton = new TextButton ("loadButton"));
    loadButton->setButtonText (TRANS("LOAD"));
    loadButton->addListener (this);

    addAndMakeVisible (saveButton = new TextButton ("saveButton"));
    saveButton->setButtonText (TRANS("SAVE"));
    saveButton->addListener (this);

    addAndMakeVisible (storeButton = new TextButton ("storeButton"));
    storeButton->setButtonText (TRANS("STORE"));
    storeButton->addListener (this);

    addAndMakeVisible (monoMode = new ToggleButton ("monoMode"));
    monoMode->setButtonText (String::empty);
    monoMode->addListener (this);

    addAndMakeVisible (lfoType = new ComboBoxImage());
    lfoType->setName ("lfoType");

    addAndMakeVisible (programSelector = new ProgramSelector());
    programSelector->setName ("programSelector");

    cachedImage_globalEditor_864x144_png = ImageCache::getFromMemory (globalEditor_864x144_png, globalEditor_864x144_pngSize);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (864, 144);


    //[Constructor] You can add your own custom stuff here..
    lfoType->addItem("TRIANGLE", 1);
    lfoType->addItem("SAW DOWN", 2);
    lfoType->addItem("SAW UP", 3);
    lfoType->addItem("SQUARE", 4);
    lfoType->addItem("SINE", 5);
    lfoType->addItem("S&HOLD", 6);
    lfoType->setImage(ImageCache::getFromMemory(BinaryData::LFO_36_26_png, BinaryData::LFO_36_26_pngSize));

    programs = programSelector;
    //[/Constructor]
}

GlobalEditor::~GlobalEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lfoSpeed = nullptr;
    lfoAmDepth = nullptr;
    lfoPitchDepth = nullptr;
    lfoDelay = nullptr;
    cutoff = nullptr;
    reso = nullptr;
    pitchRate2 = nullptr;
    pitchRate3 = nullptr;
    pitchRate4 = nullptr;
    pitchRate1 = nullptr;
    pitchLevel2 = nullptr;
    pitchLevel3 = nullptr;
    pitchLevel4 = nullptr;
    pitchLevel1 = nullptr;
    transpose = nullptr;
    oscSync = nullptr;
    pitchModSens = nullptr;
    lfoSync = nullptr;
    pitchEnvDisplay = nullptr;
    algoDisplay = nullptr;
    feedback = nullptr;
    algo = nullptr;
    lcdDisplay = nullptr;
    output = nullptr;
    vuOutput = nullptr;
    initButton = nullptr;
    parmButton = nullptr;
    sendButton = nullptr;
    cartButton = nullptr;
    loadButton = nullptr;
    saveButton = nullptr;
    storeButton = nullptr;
    monoMode = nullptr;
    lfoType = nullptr;
    programSelector = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GlobalEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour (Colours::black);
    g.drawImage (cachedImage_globalEditor_864x144_png,
                 0, 0, 864, 144,
                 0, 0, cachedImage_globalEditor_864x144_png.getWidth(), cachedImage_globalEditor_864x144_png.getHeight());

    //[UserPaint] Add your own custom painting code here..
    Image myStrip = ImageCache::getFromMemory(BinaryData::Light_14x14_png, BinaryData::Light_14x14_pngSize);
    g.drawImage(myStrip, 300, 70, 14, 14, 0, monoMode->getToggleState() ? 14 : 0, 14, 14);
    //[/UserPaint]
}

void GlobalEditor::resized()
{
    lfoSpeed->setBounds (564, 50, 34, 34);
    lfoAmDepth->setBounds (686, 50, 34, 34);
    lfoPitchDepth->setBounds (646, 50, 34, 34);
    lfoDelay->setBounds (603, 50, 34, 34);
    cutoff->setBounds (234, 9, 34, 34);
    reso->setBounds (277, 9, 34, 34);
    pitchRate2->setBounds (767, 96, 34, 34);
    pitchRate3->setBounds (795, 96, 35, 34);
    pitchRate4->setBounds (823, 96, 34, 34);
    pitchRate1->setBounds (739, 96, 34, 34);
    pitchLevel2->setBounds (767, 57, 34, 34);
    pitchLevel3->setBounds (795, 56, 34, 34);
    pitchLevel4->setBounds (823, 56, 34, 34);
    pitchLevel1->setBounds (739, 57, 34, 34);
    transpose->setBounds (202, 60, 34, 34);
    oscSync->setBounds (652, 95, 48, 26);
    pitchModSens->setBounds (666, 5, 34, 34);
    lfoSync->setBounds (571, 96, 48, 26);
    pitchEnvDisplay->setBounds (751, 10, 93, 30);
    algoDisplay->setBounds (338, 30, 146, 91);
    feedback->setBounds (501, 80, 34, 34);
    algo->setBounds (501, 22, 34, 34);
    lcdDisplay->setBounds (6, 87, 140, 13);
    output->setBounds (157, 60, 34, 34);
    vuOutput->setBounds (6, 103, 140, 8);
    initButton->setBounds (3, 53, 50, 30);
    parmButton->setBounds (51, 53, 50, 30);
    sendButton->setBounds (99, 53, 50, 30);
    cartButton->setBounds (3, 111, 50, 30);
    loadButton->setBounds (50, 111, 50, 30);
    saveButton->setBounds (98, 111, 50, 30);
    storeButton->setBounds (270, 109, 50, 30);
    monoMode->setBounds (248, 65, 48, 26);
    lfoType->setBounds (584, 8, 36, 26);
    programSelector->setBounds (153, 115, 112, 18);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GlobalEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == lfoSpeed)
    {
        //[UserSliderCode_lfoSpeed] -- add your slider handling code here..
        //[/UserSliderCode_lfoSpeed]
    }
    else if (sliderThatWasMoved == lfoAmDepth)
    {
        //[UserSliderCode_lfoAmDepth] -- add your slider handling code here..
        //[/UserSliderCode_lfoAmDepth]
    }
    else if (sliderThatWasMoved == lfoPitchDepth)
    {
        //[UserSliderCode_lfoPitchDepth] -- add your slider handling code here..
        //[/UserSliderCode_lfoPitchDepth]
    }
    else if (sliderThatWasMoved == lfoDelay)
    {
        //[UserSliderCode_lfoDelay] -- add your slider handling code here..
        //[/UserSliderCode_lfoDelay]
    }
    else if (sliderThatWasMoved == cutoff)
    {
        //[UserSliderCode_cutoff] -- add your slider handling code here..
        //[/UserSliderCode_cutoff]
    }
    else if (sliderThatWasMoved == reso)
    {
        //[UserSliderCode_reso] -- add your slider handling code here..
        //[/UserSliderCode_reso]
    }
    else if (sliderThatWasMoved == pitchRate2)
    {
        //[UserSliderCode_pitchRate2] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchRate2]
    }
    else if (sliderThatWasMoved == pitchRate3)
    {
        //[UserSliderCode_pitchRate3] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchRate3]
    }
    else if (sliderThatWasMoved == pitchRate4)
    {
        //[UserSliderCode_pitchRate4] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchRate4]
    }
    else if (sliderThatWasMoved == pitchRate1)
    {
        //[UserSliderCode_pitchRate1] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchRate1]
    }
    else if (sliderThatWasMoved == pitchLevel2)
    {
        //[UserSliderCode_pitchLevel2] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchLevel2]
    }
    else if (sliderThatWasMoved == pitchLevel3)
    {
        //[UserSliderCode_pitchLevel3] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchLevel3]
    }
    else if (sliderThatWasMoved == pitchLevel4)
    {
        //[UserSliderCode_pitchLevel4] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchLevel4]
    }
    else if (sliderThatWasMoved == pitchLevel1)
    {
        //[UserSliderCode_pitchLevel1] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchLevel1]
    }
    else if (sliderThatWasMoved == transpose)
    {
        //[UserSliderCode_transpose] -- add your slider handling code here..
        //[/UserSliderCode_transpose]
    }
    else if (sliderThatWasMoved == pitchModSens)
    {
        //[UserSliderCode_pitchModSens] -- add your slider handling code here..
        //[/UserSliderCode_pitchModSens]
    }
    else if (sliderThatWasMoved == feedback)
    {
        //[UserSliderCode_feedback] -- add your slider handling code here..
        //[/UserSliderCode_feedback]
    }
    else if (sliderThatWasMoved == algo)
    {
        //[UserSliderCode_algo] -- add your slider handling code here..
        //[/UserSliderCode_algo]
    }
    else if (sliderThatWasMoved == output)
    {
        //[UserSliderCode_output] -- add your slider handling code here..
        //[/UserSliderCode_output]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GlobalEditor::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == initButton)
    {
        //[UserButtonCode_initButton] -- add your button handler code here..
        editor->initProgram();
        //[/UserButtonCode_initButton]
    }
    else if (buttonThatWasClicked == parmButton)
    {
        //[UserButtonCode_parmButton] -- add your button handler code here..
        editor->parmShow();
        //[/UserButtonCode_parmButton]
    }
    else if (buttonThatWasClicked == sendButton)
    {
        //[UserButtonCode_sendButton] -- add your button handler code here..
        editor->sendToDx7();
        //[/UserButtonCode_sendButton]
    }
    else if (buttonThatWasClicked == cartButton)
    {
        //[UserButtonCode_cartButton] -- add your button handler code here..
        editor->cartShow();
        //[/UserButtonCode_cartButton]
    }
    else if (buttonThatWasClicked == loadButton)
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
        editor->loadCart();
        //[/UserButtonCode_loadButton]
    }
    else if (buttonThatWasClicked == saveButton)
    {
        //[UserButtonCode_saveButton] -- add your button handler code here..
        editor->saveCart();
        //[/UserButtonCode_saveButton]
    }
    else if (buttonThatWasClicked == storeButton)
    {
        //[UserButtonCode_storeButton] -- add your button handler code here..
        editor->storeProgram();
        //[/UserButtonCode_storeButton]
    }
    else if (buttonThatWasClicked == monoMode)
    {
        //[UserButtonCode_monoMode] -- add your button handler code here..
        editor->processor->setMonoMode(monoMode->getToggleState());
        repaint();
        //[/UserButtonCode_monoMode]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GlobalEditor::bind(DexedAudioProcessor *parent) {
	parent->algo->bind(algo);
	parent->lfoRate->bind(lfoSpeed);
	parent->lfoDelay->bind(lfoDelay);
	parent->lfoWaveform->bind(lfoType);
	parent->lfoAmpDepth->bind(lfoAmDepth);
	parent->lfoPitchDepth->bind(lfoPitchDepth);
    parent->lfoSync->bind(lfoSync);
    parent->oscSync->bind(oscSync);
    parent->transpose->bind(transpose);
    parent->feedback->bind(feedback);
    parent->pitchModSens->bind(pitchModSens);
    parent->pitchEgLevel[0]->bind(pitchLevel1);
    parent->pitchEgLevel[1]->bind(pitchLevel2);
    parent->pitchEgLevel[2]->bind(pitchLevel3);
    parent->pitchEgLevel[3]->bind(pitchLevel4);
    parent->pitchEgRate[0]->bind(pitchRate1);
    parent->pitchEgRate[1]->bind(pitchRate2);
    parent->pitchEgRate[2]->bind(pitchRate3);
    parent->pitchEgRate[3]->bind(pitchRate4);
    parent->fxCutoff->bind(cutoff);
    parent->fxReso->bind(reso);
    parent->output->bind(output);
    algoDisplay->algo = &(parent->data[134]);
    pitchEnvDisplay->pvalues = &(parent->data[126]);
    processor = parent;
    repaint();
}

void GlobalEditor::setSystemMessage(String msg) {
    lcdDisplay->setSystemMsg(msg);
}

void GlobalEditor::setParamMessage(String msg) {
    lcdDisplay->paramMsg = msg;
}

void GlobalEditor::updateDisplay() {
    //algoDisplay->repaint();
    //pitchEnvDisplay->repaint();
    repaint();
}

void GlobalEditor::updatePitchPos(int pos) {
    pitchEnvDisplay->vPos = pos;
    pitchEnvDisplay->repaint();
}

void GlobalEditor::updateVu(float f) {
    vuOutput->v = f;
    vuOutput->repaint();
}

void GlobalEditor::setMonoState(bool state)  {
    monoMode->setState(state ? Button::ButtonState::buttonDown : Button::ButtonState::buttonNormal);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GlobalEditor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="864" initialHeight="144">
  <BACKGROUND backgroundColour="ffffff">
    <IMAGE pos="0 0 864 144" resource="globalEditor_864x144_png" opacity="1"
           mode="0"/>
  </BACKGROUND>
  <SLIDER name="lfoSpeed" id="b10eaf327ab3bff5" memberName="lfoSpeed" virtualName=""
          explicitFocusOrder="0" pos="564 50 34 34" min="0" max="99" int="1"
          style="Rotary" textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="lfoAmDepth" id="3e95a6206fa4a891" memberName="lfoAmDepth"
          virtualName="" explicitFocusOrder="0" pos="686 50 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="lfoPitchDepth" id="6ead769ca786c813" memberName="lfoPitchDepth"
          virtualName="" explicitFocusOrder="0" pos="646 50 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="lfoDelay" id="1fce68dc81619ef5" memberName="lfoDelay" virtualName=""
          explicitFocusOrder="0" pos="603 50 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="cutoff" id="40531f16bb0bd225" memberName="cutoff" virtualName=""
          explicitFocusOrder="0" pos="234 9 34 34" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="reso" id="c8c13464e81a8d83" memberName="reso" virtualName=""
          explicitFocusOrder="0" pos="277 9 34 34" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchRate2" id="73f386b3c91d3de4" memberName="pitchRate2"
          virtualName="" explicitFocusOrder="0" pos="767 96 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchRate3" id="187c25b24413fccf" memberName="pitchRate3"
          virtualName="" explicitFocusOrder="0" pos="795 96 35 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchRate4" id="1aeb2a8fbbcbeaab" memberName="pitchRate4"
          virtualName="" explicitFocusOrder="0" pos="823 96 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchRate1" id="419d613b3fb9604e" memberName="pitchRate1"
          virtualName="" explicitFocusOrder="0" pos="739 96 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchLevel2" id="3a355ad2221887d9" memberName="pitchLevel2"
          virtualName="" explicitFocusOrder="0" pos="767 57 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchLevel3" id="5c5d782e8a7f3ad7" memberName="pitchLevel3"
          virtualName="" explicitFocusOrder="0" pos="795 56 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchLevel4" id="eb900b141fbad8ff" memberName="pitchLevel4"
          virtualName="" explicitFocusOrder="0" pos="823 56 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="pitchLevel1" id="344cbe26ec9fa128" memberName="pitchLevel1"
          virtualName="" explicitFocusOrder="0" pos="739 57 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="transpose" id="7d1266b1c1534947" memberName="transpose"
          virtualName="" explicitFocusOrder="0" pos="202 60 34 34" min="0"
          max="48" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="oscSync" id="8f3fe641537cd00" memberName="oscSync" virtualName=""
                explicitFocusOrder="0" pos="652 95 48 26" buttonText="" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="0"/>
  <SLIDER name="pitchModSens" id="904f73df85a9f886" memberName="pitchModSens"
          virtualName="" explicitFocusOrder="0" pos="666 5 34 34" min="0"
          max="7" int="0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="lfoSync" id="ff92bb0a5a4f7187" memberName="lfoSync" virtualName=""
                explicitFocusOrder="0" pos="571 96 48 26" buttonText="" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="pitchEnvDisplay" id="9ddaae8ef924a038" memberName="pitchEnvDisplay"
                    virtualName="" explicitFocusOrder="0" pos="751 10 93 30" class="PitchEnvDisplay"
                    params=""/>
  <GENERICCOMPONENT name="algoDisplay" id="b26fb9e3b5f0bc37" memberName="algoDisplay"
                    virtualName="" explicitFocusOrder="0" pos="338 30 146 91" class="AlgoDisplay"
                    params=""/>
  <SLIDER name="feedback" id="4fac1940c29ab8c" memberName="feedback" virtualName=""
          explicitFocusOrder="0" pos="501 80 34 34" min="0" max="7" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="algo" id="8a226ddf9bbff752" memberName="algo" virtualName=""
          explicitFocusOrder="0" pos="501 22 34 34" min="1" max="32" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <GENERICCOMPONENT name="lcdDisplay" id="30c7bb8f114cbbe3" memberName="lcdDisplay"
                    virtualName="" explicitFocusOrder="0" pos="6 87 140 13" class="LcdDisplay"
                    params=""/>
  <SLIDER name="cutoff" id="7697fdd54fd1593e" memberName="output" virtualName=""
          explicitFocusOrder="0" pos="157 60 34 34" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <GENERICCOMPONENT name="vuOutput" id="dac75af912267f51" memberName="vuOutput" virtualName=""
                    explicitFocusOrder="0" pos="6 103 140 8" class="VuMeter" params=""/>
  <TEXTBUTTON name="initButton" id="92b278163c42e21d" memberName="initButton"
              virtualName="" explicitFocusOrder="0" pos="3 53 50 30" buttonText="INIT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="parmButton" id="d5cd6260b34be817" memberName="parmButton"
              virtualName="" explicitFocusOrder="0" pos="51 53 50 30" buttonText="PARM"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="sendButton" id="9b45b291c75c7b3f" memberName="sendButton"
              virtualName="" explicitFocusOrder="0" pos="99 53 50 30" buttonText="SEND"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="cartButton" id="465a101c3a0e744" memberName="cartButton"
              virtualName="" explicitFocusOrder="0" pos="3 111 50 30" buttonText="CART"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="loadButton" id="54e62fe23ff547d1" memberName="loadButton"
              virtualName="" explicitFocusOrder="0" pos="50 111 50 30" buttonText="LOAD"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="saveButton" id="44bf9479012da9b4" memberName="saveButton"
              virtualName="" explicitFocusOrder="0" pos="98 111 50 30" buttonText="SAVE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="storeButton" id="a94bec26832eb58b" memberName="storeButton"
              virtualName="" explicitFocusOrder="0" pos="270 109 50 30" buttonText="STORE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="monoMode" id="e683d8cbe286367b" memberName="monoMode" virtualName=""
                explicitFocusOrder="0" pos="248 65 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="lfoType" id="62f61dda9f76c8f" memberName="lfoType" virtualName=""
                    explicitFocusOrder="0" pos="584 8 36 26" class="ComboBoxImage"
                    params=""/>
  <GENERICCOMPONENT name="programSelector" id="990bbcccae72dbe6" memberName="programSelector"
                    virtualName="" explicitFocusOrder="0" pos="153 115 112 18" class="ProgramSelector"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: globalEditor_864x144_png, 16274, "../Resources/ui/GlobalEditor_864x144.png"
static const unsigned char resource_GlobalEditor_globalEditor_864x144_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,3,96,0,0,0,144,8,6,0,0,0,110,232,8,32,0,0,0,9,112,72,89,115,0,0,14,195,
0,0,14,195,1,199,111,168,100,0,0,0,7,116,73,77,69,7,222,9,16,3,34,22,232,201,175,57,0,0,0,29,105,84,88,116,67,111,109,109,101,110,116,0,0,0,0,0,67,114,101,97,116,101,100,32,119,105,116,104,32,71,73,77,
80,100,46,101,7,0,0,32,0,73,68,65,84,120,218,237,221,121,124,83,101,190,63,240,79,74,123,2,154,48,51,57,157,107,147,249,93,26,238,208,118,126,182,229,218,197,129,194,176,41,34,107,69,5,148,197,5,68,64,
101,145,165,3,165,104,5,101,113,40,91,1,45,200,166,128,40,8,178,239,12,219,143,237,74,91,134,182,252,108,203,189,36,253,141,73,71,123,162,99,142,163,57,44,253,253,145,228,144,52,45,20,104,75,151,207,251,
245,234,11,146,156,156,229,201,115,158,156,111,158,231,124,31,13,0,13,128,32,0,193,0,4,0,205,61,127,45,0,104,1,132,0,104,230,89,70,3,34,162,134,165,28,192,13,0,215,1,92,5,224,2,240,139,207,159,226,121,
254,134,103,121,182,135,68,60,31,137,88,159,169,214,234,67,176,231,131,14,241,84,2,29,128,95,121,254,90,2,120,208,243,188,183,82,176,66,16,81,67,108,16,175,3,184,230,105,0,127,2,224,4,240,79,207,159,12,
224,103,79,195,8,182,135,68,60,31,137,88,159,169,54,235,67,176,39,18,111,14,224,215,0,66,1,60,20,249,123,115,251,54,255,97,238,96,12,123,232,63,254,253,127,253,206,204,242,36,162,198,160,228,255,253,221,
90,86,38,149,88,74,254,158,125,241,82,225,81,0,165,0,202,42,44,166,101,123,72,196,243,145,136,245,153,106,169,62,148,107,0,232,61,149,225,33,189,238,193,63,116,238,240,232,75,143,38,196,119,183,219,237,
0,0,239,191,68,68,13,157,209,104,84,255,45,248,191,95,127,181,255,240,177,229,78,249,95,95,123,26,70,25,238,161,3,58,182,135,68,60,31,137,88,159,169,150,234,195,15,26,0,191,5,240,16,0,243,51,253,122,166,
254,86,252,77,7,86,2,34,106,10,141,163,75,185,90,184,238,211,47,166,1,176,0,112,192,61,124,224,55,0,90,177,61,36,226,249,72,196,250,76,53,92,31,174,0,40,109,6,119,55,232,191,69,253,222,220,35,250,15,17,
207,179,50,16,81,83,32,203,50,126,253,235,95,133,162,252,134,203,254,143,239,138,224,30,163,47,0,16,163,126,111,238,206,246,144,136,231,35,17,235,51,213,84,125,208,160,220,101,43,253,238,107,0,63,121,
239,255,106,25,21,241,251,36,86,6,162,250,227,219,50,7,11,161,134,252,91,168,161,210,231,237,118,59,254,16,25,241,104,246,223,46,109,135,123,136,72,57,128,95,177,61,36,182,13,13,227,124,188,122,245,26,
92,138,130,107,215,174,65,185,122,141,149,131,234,140,16,18,140,224,224,96,8,33,33,16,132,16,214,103,214,135,106,213,135,243,23,10,182,2,104,17,12,247,13,129,15,62,208,162,121,196,207,255,250,137,37,72,
84,79,46,174,152,2,169,230,124,231,41,211,202,46,252,132,224,224,255,128,59,3,85,11,207,23,36,219,67,98,219,80,207,207,199,27,55,110,224,95,63,255,130,127,253,252,139,250,28,219,76,170,75,87,175,94,195,
213,171,215,240,243,207,191,160,185,86,128,238,193,7,16,20,20,196,250,204,250,112,187,250,240,0,0,65,77,65,255,243,191,126,10,99,241,17,213,143,11,44,170,187,242,149,164,50,239,92,44,130,231,175,5,219,
67,98,219,80,191,207,199,159,254,245,179,223,197,42,209,253,244,139,75,193,143,242,79,172,207,84,157,250,160,5,16,18,12,247,124,3,33,26,13,99,109,162,251,141,231,225,125,43,227,102,158,63,176,61,36,182,
13,245,251,124,84,174,94,197,47,46,133,237,37,213,43,87,175,94,195,47,46,5,45,154,107,89,159,233,86,245,33,24,64,144,119,34,230,32,22,21,209,253,197,222,175,186,43,231,74,134,62,53,243,105,7,217,30,18,
219,134,122,124,62,186,92,87,89,41,168,94,114,41,85,6,96,53,86,159,79,159,255,27,11,250,62,234,144,248,159,247,90,31,130,0,104,130,225,30,102,202,176,155,136,154,58,77,133,127,137,168,30,158,143,87,175,
214,125,0,118,245,234,85,88,254,110,199,119,210,247,117,178,189,208,223,252,26,255,203,244,16,30,104,209,156,53,161,1,185,90,117,226,140,26,173,207,45,180,218,128,251,139,168,118,5,5,5,161,101,203,150,
53,86,31,130,3,235,6,17,81,83,185,182,187,219,215,137,232,126,157,143,215,174,95,175,243,61,252,111,235,223,225,248,225,71,52,23,4,52,107,214,172,86,183,85,94,94,142,178,239,127,192,191,126,249,5,143,
68,71,177,122,52,32,229,229,229,181,94,159,155,105,52,208,106,181,8,14,14,102,129,215,101,43,229,41,247,154,170,15,252,244,136,136,136,136,110,193,241,195,143,208,61,240,0,4,65,168,245,158,135,27,55,110,
160,121,243,230,16,4,129,5,79,212,72,49,0,35,34,34,34,186,133,32,0,193,193,193,117,50,236,203,187,141,22,45,90,176,224,137,26,123,0,198,236,43,68,247,27,207,193,58,43,233,219,180,119,108,15,137,109,67,
125,62,31,53,245,110,31,107,99,123,238,109,178,45,98,125,166,134,220,30,87,85,31,120,7,31,17,17,17,17,17,81,29,97,0,70,68,68,68,68,68,196,0,140,136,136,136,136,136,136,1,24,17,17,17,17,17,17,49,0,35,34,
34,34,34,34,98,0,70,68,68,68,68,68,212,228,49,13,61,81,61,193,115,176,254,148,117,197,215,183,237,61,204,66,163,58,243,76,239,238,77,170,109,184,211,243,177,41,181,149,252,94,96,125,166,134,125,78,86,
181,252,205,137,152,249,249,19,221,231,179,154,69,80,111,202,186,146,215,91,104,181,117,50,9,43,53,93,65,65,65,104,217,178,101,96,253,211,240,124,108,178,109,37,191,23,88,159,169,97,159,147,85,44,31,204,
146,36,34,186,181,102,26,13,180,90,45,130,131,217,100,82,45,126,175,123,234,25,17,17,53,110,252,57,151,136,136,136,136,136,136,1,24,17,17,17,17,17,17,3,48,34,34,34,34,34,34,98,0,70,68,68,68,68,68,84,127,
169,119,148,7,49,13,38,209,125,197,84,180,117,199,183,189,43,191,205,235,68,247,179,126,54,133,182,225,78,207,199,251,81,30,26,141,230,190,109,151,88,159,169,225,94,171,85,117,61,193,30,48,34,34,34,162,
91,104,222,188,121,157,95,40,115,218,11,162,198,139,57,149,137,136,136,136,110,193,100,50,226,231,159,127,193,245,235,215,235,100,123,33,33,33,248,237,111,67,1,92,99,225,19,49,0,35,34,34,34,106,90,90,
234,90,32,84,12,69,203,95,253,10,205,154,53,171,245,237,149,151,151,195,81,246,45,208,140,101,79,196,0,140,136,136,136,168,137,105,174,21,0,92,133,252,207,178,186,187,64,99,240,69,212,20,2,48,222,4,72,
68,77,133,230,30,95,39,186,159,245,147,231,35,17,235,51,53,228,250,192,30,48,34,162,123,212,177,227,35,247,244,254,83,167,46,112,27,220,6,17,17,53,17,106,0,198,52,152,68,247,151,134,191,138,213,93,89,
223,166,189,99,123,72,245,169,126,54,246,182,225,78,207,71,182,149,196,250,76,13,229,90,77,195,52,244,68,68,68,68,119,239,145,248,4,245,255,97,70,35,90,133,155,213,199,109,34,34,161,211,233,212,255,63,
18,159,160,62,110,21,110,70,152,209,168,46,43,8,2,30,142,142,241,91,111,155,136,200,122,119,188,130,32,224,145,248,4,191,125,173,43,99,223,156,84,47,203,132,168,38,48,0,35,34,34,34,170,134,201,127,78,
85,3,169,209,175,141,197,236,247,51,212,215,230,47,206,4,0,252,169,115,87,36,63,253,12,226,226,19,176,250,147,79,209,42,220,140,86,225,102,76,254,115,170,186,108,175,62,253,240,216,19,61,220,239,91,148,
137,199,186,63,129,228,167,159,193,146,229,89,245,234,120,13,162,136,249,139,50,49,250,245,177,88,253,201,70,204,95,148,233,23,72,214,166,136,136,72,232,244,122,86,58,106,148,212,33,136,229,229,229,44,
13,162,251,168,28,60,7,235,172,172,125,218,187,202,218,62,182,135,84,95,234,103,83,104,27,238,244,124,188,159,229,113,238,236,41,252,169,115,87,236,248,242,11,180,137,140,132,213,122,5,255,30,30,14,131,
40,162,196,98,129,83,118,226,228,137,163,56,121,226,40,0,192,233,116,226,79,93,186,96,203,103,159,98,220,155,147,240,27,131,1,14,135,132,158,125,250,32,227,253,185,120,200,24,6,151,226,194,130,191,204,
245,4,99,75,241,191,163,99,112,169,32,175,222,124,47,57,36,9,227,223,24,13,0,24,248,220,16,12,120,238,121,44,93,188,208,111,57,157,78,7,131,24,170,6,109,14,73,66,137,213,130,71,226,19,160,184,148,128,
227,241,246,36,150,88,44,112,56,36,245,249,86,225,102,24,68,17,165,118,155,186,125,239,231,221,198,19,144,93,200,201,174,58,96,52,136,104,101,54,67,118,58,113,185,184,200,111,123,21,159,107,19,17,9,135,
36,161,149,217,140,82,187,13,165,118,251,109,215,211,216,234,51,213,254,181,90,85,215,19,55,147,112,112,8,42,17,53,21,76,82,69,13,185,126,242,124,188,111,78,158,56,142,167,250,63,139,18,235,21,228,230,
100,227,114,113,17,30,247,244,100,157,59,119,6,0,240,248,19,61,240,167,78,93,161,211,235,17,30,110,198,190,189,187,160,40,10,142,28,58,128,78,93,186,34,55,39,27,130,160,197,229,226,34,60,245,244,179,136,
136,136,66,230,242,21,234,54,244,122,93,189,253,168,28,142,50,252,177,125,82,192,243,109,34,163,48,229,207,169,176,90,45,40,177,88,208,171,111,63,228,102,103,67,81,92,16,4,45,68,113,172,26,196,165,206,
72,135,65,12,69,137,245,10,218,253,57,21,239,188,53,29,151,139,139,208,38,34,18,25,139,151,226,200,161,3,136,136,136,130,65,20,213,245,15,124,110,8,58,117,238,130,130,252,60,140,127,115,50,166,79,157,
236,23,48,1,64,167,206,93,145,252,244,179,184,84,144,135,176,48,35,182,124,190,9,151,139,139,144,58,35,29,0,32,203,78,68,199,196,98,204,200,225,0,128,140,197,75,81,144,159,135,203,197,69,104,215,126,44,
206,157,61,131,181,171,86,162,83,231,174,232,217,167,47,46,23,23,33,44,204,136,21,31,44,243,11,18,239,119,125,214,106,181,104,222,188,57,130,130,56,136,173,46,5,5,5,65,16,132,26,107,223,152,5,145,136,
160,211,233,208,169,75,55,60,28,29,131,45,159,111,130,162,184,240,84,255,103,33,104,133,128,95,58,137,136,154,170,75,249,121,152,242,231,84,56,28,73,56,114,248,32,74,44,22,204,249,75,6,20,151,130,140,
191,204,133,78,167,195,184,55,39,99,208,211,253,160,40,138,223,176,195,29,219,183,98,250,140,119,96,16,69,236,248,114,43,0,160,32,63,15,165,118,155,26,156,212,203,239,7,189,30,195,71,142,2,0,244,234,221,
15,75,23,47,168,116,185,48,163,9,35,94,28,2,69,81,160,211,235,33,8,2,230,190,55,19,0,176,125,207,1,232,116,58,8,130,22,209,49,177,24,246,252,0,0,192,133,206,57,120,234,233,103,177,224,47,115,209,171,79,
95,172,248,96,25,246,237,217,5,0,216,123,232,24,0,247,125,104,195,71,142,66,255,62,61,160,40,10,74,75,237,232,213,167,31,214,174,90,233,183,253,63,182,79,66,137,245,10,182,124,246,41,100,89,6,224,238,
201,10,55,155,213,160,43,125,214,28,60,18,159,128,11,57,217,16,197,80,172,93,181,18,151,139,139,112,228,208,65,188,243,238,28,172,93,181,18,127,234,220,5,37,22,75,192,250,235,139,216,216,88,24,12,6,6,
96,247,33,0,11,9,9,193,183,246,146,26,89,31,3,48,162,70,172,83,231,174,56,121,226,216,109,151,123,252,137,39,241,112,116,12,246,237,221,141,82,187,13,233,239,206,193,133,156,28,20,228,231,177,16,137,136,
60,20,69,129,213,106,65,92,124,162,223,143,83,58,189,30,37,86,139,250,11,121,175,62,253,32,8,90,68,68,70,226,220,89,119,207,88,169,221,14,151,162,160,87,239,126,24,241,226,96,0,192,229,226,34,8,90,1,169,
51,210,241,215,195,7,209,38,50,10,251,118,239,186,179,30,151,58,146,155,147,125,203,160,228,114,113,33,20,69,1,0,56,28,18,28,210,205,99,80,92,238,160,44,58,38,214,239,123,165,184,184,16,163,95,31,11,0,
136,139,79,196,190,61,187,111,174,175,168,80,13,236,0,32,99,209,82,191,247,85,180,226,131,165,24,250,194,203,88,243,201,38,236,219,187,11,107,87,173,68,187,164,14,8,51,154,252,122,24,141,70,19,46,32,27,
78,159,225,133,138,226,130,78,231,190,223,108,233,226,5,24,62,114,20,54,124,246,5,142,28,62,88,239,2,177,144,160,235,112,254,240,29,79,198,6,142,1,24,81,3,34,8,2,30,142,137,245,124,153,219,224,144,36,
180,137,136,82,199,215,235,116,58,132,25,77,234,208,150,63,117,238,10,167,236,196,229,162,66,200,178,172,142,107,7,160,142,163,215,233,116,232,213,167,47,78,158,56,14,217,233,132,65,20,209,174,125,7,236,
252,114,43,28,142,178,26,219,119,239,61,2,178,211,137,86,102,51,46,229,231,65,81,20,117,204,191,247,177,151,119,188,191,239,61,2,222,253,175,236,158,2,34,162,186,176,116,241,2,245,98,29,0,230,188,59,83,
13,188,20,69,193,152,145,47,163,83,231,110,112,56,202,48,229,205,113,126,137,36,22,252,101,14,116,58,189,218,67,3,0,227,95,31,237,254,17,44,38,22,151,139,138,32,203,206,122,117,188,178,211,89,173,32,68,
113,41,1,193,106,69,86,139,5,67,95,120,89,125,28,30,110,86,3,178,226,162,66,24,61,223,95,190,129,87,137,213,2,217,233,196,148,137,227,42,93,167,186,159,178,140,21,31,46,195,218,213,43,49,251,253,5,120,
234,233,103,113,238,236,105,200,78,103,165,61,140,178,211,89,229,122,150,46,94,136,181,186,149,72,127,119,46,30,127,162,7,142,28,58,200,138,79,12,192,136,154,170,204,15,86,64,146,36,92,46,46,66,169,221,
132,220,156,243,120,231,221,57,24,244,76,63,119,208,18,25,133,17,175,140,194,244,169,147,241,199,246,73,16,69,17,113,241,9,40,181,219,32,8,90,100,126,176,2,185,57,217,208,233,244,24,241,202,40,140,127,
99,52,58,117,233,6,131,33,20,173,90,133,195,33,73,8,51,26,161,213,106,241,112,76,44,92,138,18,48,206,254,110,181,137,140,194,152,215,199,66,16,180,56,121,226,24,46,228,100,163,83,231,174,24,250,226,75,
56,119,246,12,198,191,57,25,25,239,207,197,165,130,60,204,126,63,3,37,22,11,148,171,10,122,245,238,139,185,239,205,132,193,32,34,107,213,58,156,59,123,26,58,157,222,239,158,2,34,162,186,226,110,19,111,
182,139,37,86,75,192,235,91,62,255,212,239,130,190,170,247,122,3,21,239,176,187,198,174,196,106,129,36,73,72,157,145,142,203,197,197,232,213,167,47,50,61,67,26,247,237,221,173,14,209,172,152,126,254,200,
161,3,200,88,180,20,251,246,238,86,131,167,138,163,59,220,189,142,2,28,146,4,163,209,232,25,222,105,135,213,106,65,234,140,116,252,159,19,199,17,102,116,127,111,222,42,177,198,208,23,95,134,236,116,66,
150,157,16,69,17,185,217,217,172,244,84,123,1,24,239,57,39,186,191,110,119,14,62,28,29,11,135,36,97,198,212,41,234,115,222,116,192,154,10,235,248,73,150,113,234,196,113,252,103,92,60,214,121,126,185,28,
248,220,16,252,213,103,56,197,146,229,43,16,29,29,139,253,123,118,161,87,239,190,216,191,119,183,218,43,54,98,228,104,245,125,154,26,60,190,86,225,173,241,220,211,125,33,203,50,52,0,158,122,250,89,164,
253,121,10,28,14,9,151,242,243,240,212,211,207,224,191,139,11,17,29,19,139,117,158,177,249,222,247,246,238,219,15,251,61,195,74,188,251,31,17,17,121,199,25,170,170,115,76,108,15,169,62,181,5,154,38,118,
188,77,189,60,234,147,239,37,9,51,223,154,126,219,50,255,239,162,66,44,93,188,64,93,110,255,158,93,80,92,138,250,120,230,91,211,241,189,36,225,170,162,96,198,212,201,248,99,251,14,136,136,140,196,204,
183,166,163,196,106,129,6,192,223,114,178,241,218,200,151,241,248,19,79,98,231,151,91,241,233,250,117,144,157,78,104,0,172,252,112,25,254,216,62,9,209,49,177,40,181,219,113,185,184,48,96,159,46,229,231,
161,77,100,36,68,81,68,218,212,41,234,122,103,189,53,29,127,234,210,21,17,145,145,176,90,45,248,135,221,6,141,103,159,52,149,28,231,127,157,57,141,136,200,40,232,117,122,247,126,59,164,59,170,115,172,
207,84,157,207,151,105,232,137,234,137,219,157,129,143,36,36,160,184,184,200,111,185,242,91,252,91,241,185,71,219,39,97,227,250,117,234,227,220,220,108,60,146,144,128,2,207,80,190,242,10,251,80,94,11,
199,119,238,204,105,56,61,191,6,123,39,248,76,127,119,142,186,140,36,73,112,41,10,166,255,121,10,94,30,57,10,70,163,9,153,139,23,224,66,78,54,254,51,46,33,96,255,255,152,212,1,197,119,17,128,49,13,61,
213,235,182,32,32,77,117,211,57,222,234,165,237,166,186,226,82,20,245,59,226,86,156,178,12,167,79,91,108,175,48,114,194,119,29,46,69,193,201,19,199,42,189,63,89,114,72,216,236,211,123,232,235,220,217,
51,234,253,116,149,177,90,45,176,86,232,141,244,110,175,178,33,132,21,247,201,251,184,184,184,232,174,190,87,26,67,125,246,222,230,112,187,97,254,190,183,67,120,121,111,117,240,187,110,137,79,168,244,
121,175,244,89,115,176,116,241,130,26,187,231,177,93,251,36,180,137,140,194,198,79,214,213,219,246,60,32,0,35,162,250,173,32,63,15,3,159,27,236,247,156,236,116,250,221,95,96,244,140,153,7,160,166,255,
245,42,177,94,65,68,68,148,218,203,213,170,149,25,59,182,111,173,211,99,80,20,151,207,255,21,148,88,45,152,243,222,59,1,195,28,47,21,228,33,109,234,20,180,10,55,35,107,213,58,12,122,186,47,28,82,153,223,
254,183,137,136,196,198,79,62,174,23,159,205,169,83,23,184,13,110,131,136,168,65,51,136,34,50,22,45,197,165,252,60,232,244,122,72,146,132,5,127,153,19,240,29,237,187,156,87,230,226,5,184,92,92,4,157,78,
135,212,183,222,129,40,138,80,92,10,12,162,136,130,252,60,53,35,166,175,232,152,88,8,90,161,6,247,223,125,59,69,67,192,0,140,168,129,184,148,159,7,241,245,177,24,247,230,36,148,88,173,176,90,45,184,144,
147,141,203,69,133,24,253,218,88,56,28,14,116,234,220,69,93,62,55,59,27,227,222,156,140,78,157,187,162,32,63,15,251,246,236,198,59,239,206,129,160,21,96,48,24,16,110,54,223,114,66,203,186,176,227,203,
173,152,243,254,2,236,248,114,43,100,217,9,217,233,68,113,81,17,134,190,248,18,46,228,228,32,204,104,66,169,221,6,89,150,177,229,243,77,234,254,135,133,25,97,52,154,152,136,131,136,136,168,6,85,156,120,
123,224,115,131,43,157,142,198,119,57,95,153,31,172,196,142,47,183,170,83,45,220,137,135,163,221,189,106,222,239,246,54,17,145,234,53,0,224,78,196,229,205,56,170,211,233,208,38,50,42,96,18,237,134,130,
1,24,81,3,225,206,174,53,28,237,218,39,169,129,9,0,76,153,56,14,157,186,116,133,32,104,49,125,234,100,53,115,148,195,33,97,202,155,238,215,138,139,11,113,185,184,8,83,38,142,67,187,246,29,2,178,90,101,
46,94,160,174,15,0,198,189,62,170,198,247,255,114,81,33,54,74,82,64,0,86,92,84,132,118,73,238,137,61,11,242,243,32,203,78,156,60,113,28,209,49,177,40,177,88,48,222,179,47,151,139,139,48,125,234,20,196,
197,39,224,178,211,137,21,31,44,101,165,32,34,34,170,173,96,172,138,137,183,171,210,38,34,18,178,211,121,87,193,215,184,55,39,169,215,54,83,166,166,98,204,200,151,241,120,247,30,144,127,146,213,33,133,
227,222,156,140,191,30,62,8,217,233,68,230,7,43,112,238,236,105,132,25,77,40,177,88,176,226,195,101,12,192,136,232,206,133,253,86,68,233,119,183,31,7,93,113,12,188,82,97,124,187,111,82,138,203,197,69,
126,143,75,237,246,74,27,198,138,137,44,106,163,103,76,150,229,74,199,129,95,42,200,11,232,201,186,144,147,93,233,62,148,88,45,1,25,199,238,166,156,137,136,136,40,80,117,39,222,54,136,162,223,252,106,
227,223,24,141,232,152,88,191,251,240,122,245,233,135,48,163,17,178,83,246,203,12,26,240,189,108,52,162,83,231,110,106,70,231,209,175,141,197,227,79,60,137,29,219,183,34,99,209,82,108,252,100,29,116,58,
29,162,99,98,49,247,189,119,220,175,125,185,13,91,62,255,20,130,32,96,195,103,91,177,118,245,202,6,85,206,12,192,136,136,136,136,136,72,117,187,137,183,43,27,130,232,144,36,196,61,151,160,62,46,46,42,
132,221,110,195,244,25,239,220,50,0,11,15,55,67,208,10,126,1,157,252,147,140,82,187,29,14,73,194,195,209,177,136,136,140,196,201,19,71,161,40,10,254,216,46,9,225,102,179,122,219,69,169,221,230,55,55,31,
3,48,34,186,35,213,237,5,163,187,47,95,34,34,34,170,92,117,39,222,174,76,65,126,30,194,140,38,180,10,55,163,196,106,169,246,52,49,146,36,65,113,41,149,222,83,182,99,251,86,60,254,196,19,136,136,136,66,
198,95,230,2,0,46,21,228,163,164,196,114,215,251,89,175,2,48,166,93,38,170,31,30,10,53,0,0,254,193,64,172,230,202,212,19,120,85,55,229,60,219,67,186,159,2,234,95,35,175,143,119,60,45,4,207,79,98,125,174,
173,157,175,222,62,149,151,187,135,42,190,114,243,126,241,125,123,118,162,212,110,199,220,119,223,113,79,154,189,103,23,46,229,231,221,76,87,95,213,58,203,203,113,185,168,16,197,69,133,152,61,47,3,127,
61,124,16,58,157,30,86,235,21,92,200,201,198,201,99,71,49,250,181,113,112,72,101,40,177,92,1,0,28,57,184,31,89,171,63,134,16,34,224,66,78,54,218,68,70,186,239,19,171,238,254,223,207,246,188,98,0,70,68,
245,51,104,32,34,186,157,54,17,145,232,213,167,234,251,53,238,70,171,112,51,74,237,54,40,138,226,247,188,193,32,98,220,196,201,152,249,214,116,22,60,81,35,226,144,36,188,243,86,106,181,150,155,254,231,
73,126,207,201,78,167,59,56,58,116,0,5,249,23,17,23,159,136,135,99,98,33,203,206,42,215,249,206,91,169,112,120,146,115,165,77,157,140,199,159,120,82,237,61,43,177,88,0,184,239,115,159,254,231,73,144,101,
231,205,237,59,36,140,121,229,37,60,222,227,73,60,28,19,139,115,103,79,3,112,207,53,90,144,223,48,178,35,51,0,35,34,34,106,224,116,122,61,34,34,163,106,116,157,189,250,244,195,190,61,187,80,98,181,160,
93,251,14,40,200,191,8,89,150,33,104,5,68,199,180,101,161,19,53,50,138,162,248,205,237,117,171,229,110,149,172,171,212,110,199,190,61,187,110,187,158,138,219,58,114,232,64,165,203,85,54,148,209,225,144,
176,229,179,79,3,158,171,169,73,157,235,48,0,211,176,230,17,17,221,69,123,216,177,227,35,247,180,181,234,76,234,203,109,52,165,109,104,110,249,216,59,255,141,236,116,250,92,152,104,252,254,53,24,68,180,
50,155,225,144,36,245,151,105,239,133,73,171,112,51,12,162,232,247,126,157,78,7,131,24,10,135,84,134,54,145,81,184,148,159,135,35,135,14,194,33,73,48,24,66,49,110,226,100,172,93,181,18,86,139,69,253,37,
218,96,8,69,43,179,217,111,30,30,131,65,132,160,21,160,211,233,221,243,245,120,150,127,56,38,182,194,254,222,201,249,166,185,167,243,147,168,126,125,127,176,62,179,62,176,7,140,136,136,168,65,153,60,117,
58,74,172,22,132,25,141,0,128,185,239,206,244,123,221,96,16,145,181,122,157,58,71,142,94,167,195,142,47,183,97,223,158,93,104,215,62,9,147,167,78,199,145,67,7,16,23,159,128,115,103,207,96,237,170,149,
104,19,25,133,17,158,212,211,185,158,9,222,199,79,156,140,53,171,86,194,104,52,193,32,134,226,225,232,24,8,130,22,185,57,231,161,211,235,49,121,106,42,46,23,23,161,93,251,177,56,114,248,16,182,124,246,
41,218,37,117,192,192,231,7,171,129,90,92,124,2,138,139,138,32,203,78,132,25,77,112,72,101,1,251,75,68,212,212,48,0,163,58,197,196,18,116,63,240,126,58,106,76,124,239,189,218,252,101,224,48,159,118,73,
29,112,242,248,81,44,93,188,16,0,144,181,106,173,250,218,128,231,135,96,206,187,239,224,66,78,182,123,254,156,207,183,170,147,156,62,28,19,139,254,189,123,4,204,215,183,111,207,46,12,31,57,10,91,62,223,
132,82,187,29,97,70,35,244,122,61,214,174,90,137,203,197,69,56,114,232,32,166,191,149,174,14,7,210,233,244,234,62,102,173,90,139,18,171,59,91,153,78,167,195,230,237,187,25,128,17,17,3,48,22,1,49,240,162,
166,82,255,24,136,81,67,103,48,136,24,253,198,88,0,128,209,104,130,209,100,10,88,230,225,152,24,252,151,207,132,237,185,62,247,106,196,197,39,32,229,205,113,0,220,247,113,148,218,109,104,227,185,119,236,
114,81,97,165,147,165,87,198,233,51,156,80,81,92,48,136,161,234,107,190,55,193,251,222,83,34,203,50,244,122,61,63,68,34,98,0,230,253,15,211,46,19,131,47,106,236,74,191,45,11,8,194,152,134,158,234,155,
138,245,207,247,241,128,231,6,163,196,106,193,134,143,215,193,32,138,216,185,239,144,251,245,242,114,148,123,150,45,46,42,66,155,136,72,156,56,118,20,128,251,158,47,171,213,130,242,242,114,56,36,9,191,
143,136,192,229,34,119,240,20,102,52,161,184,240,107,68,199,196,194,165,40,254,41,180,221,27,87,159,43,247,252,191,188,188,28,178,211,233,247,188,250,58,202,161,184,92,55,159,3,224,242,121,124,187,227,
171,206,249,200,243,147,26,202,249,203,250,76,76,67,79,68,68,212,192,125,239,144,240,216,19,61,96,181,88,208,169,115,87,72,101,101,1,203,156,59,115,10,75,63,252,8,118,187,13,173,90,153,17,29,19,139,147,
39,142,1,0,214,172,90,129,180,183,102,98,223,158,93,120,36,62,1,231,206,156,10,72,51,95,153,220,236,243,24,248,252,16,236,219,179,75,77,234,65,68,68,119,71,13,192,52,76,194,66,181,224,31,223,73,172,91,
84,175,124,91,38,221,118,40,34,235,44,221,79,21,235,159,239,227,45,159,127,138,210,82,27,194,205,102,172,93,189,2,91,62,255,20,26,13,112,185,184,16,75,23,101,64,163,1,254,81,106,199,248,215,95,69,175,
62,201,248,91,110,54,52,26,224,170,162,64,163,1,246,123,210,202,199,197,39,224,175,135,15,226,200,161,3,1,239,247,90,186,40,3,165,118,27,52,26,96,233,226,12,116,234,210,13,102,115,107,156,60,126,20,239,
188,149,170,46,251,189,67,82,31,255,215,153,211,184,148,159,167,190,230,187,14,0,24,247,218,171,183,60,190,59,45,15,162,250,126,254,178,62,179,62,220,50,0,35,34,34,162,250,239,228,241,99,62,143,220,233,
223,101,89,246,75,41,47,8,90,172,93,181,2,97,70,35,38,79,157,238,55,95,206,165,252,188,128,249,119,124,223,239,229,251,88,150,101,191,121,125,46,85,113,159,87,197,121,120,42,174,243,86,115,7,17,17,53,
21,12,192,136,136,136,26,17,157,94,143,113,19,39,67,43,104,225,116,58,177,118,213,138,6,51,57,41,17,17,3,48,34,34,34,106,80,74,237,118,53,211,33,17,17,213,227,0,140,89,88,168,54,176,94,81,125,175,151,
204,130,72,245,189,221,108,236,245,145,89,227,136,245,153,154,218,117,112,16,139,134,136,136,136,136,136,168,110,112,8,34,17,209,61,58,117,234,2,183,193,109,16,17,17,85,11,123,192,136,136,136,136,136,
136,24,128,17,17,17,17,17,17,49,0,35,34,34,34,34,34,34,6,96,68,68,68,68,68,68,245,23,211,208,83,173,98,189,162,250,94,47,153,134,158,234,123,187,201,52,244,76,219,77,172,207,212,184,174,131,217,3,70,68,
68,68,68,68,84,71,24,128,17,17,17,17,17,17,49,0,35,34,34,34,34,34,106,92,56,17,51,17,209,61,234,216,241,145,123,122,127,117,38,245,229,54,154,222,54,136,136,168,113,98,15,24,17,17,17,17,17,17,3,48,34,
34,34,34,34,162,198,133,105,232,137,168,201,97,26,122,106,40,245,147,231,35,207,71,98,125,166,198,215,158,179,7,140,136,136,136,232,46,180,75,234,128,97,47,13,103,65,16,209,29,97,0,70,68,68,68,116,23,
68,49,20,173,194,205,53,18,200,245,238,155,124,203,101,122,247,77,70,187,164,14,247,229,56,5,65,64,92,124,34,162,99,98,3,94,107,21,110,174,244,121,131,40,34,46,62,17,130,32,0,0,34,34,163,16,23,159,88,
35,229,117,171,253,140,136,140,242,123,78,167,211,35,46,62,17,58,157,62,96,249,184,248,68,132,25,77,126,251,27,23,159,200,138,77,181,78,29,130,168,209,104,88,26,68,212,36,220,174,189,99,123,72,245,185,
126,242,124,188,53,131,40,34,60,188,53,100,217,137,226,162,66,8,130,128,112,115,107,72,82,153,223,243,190,23,232,17,145,81,176,219,109,40,181,219,252,46,230,163,99,218,66,146,202,80,98,181,4,44,175,40,
174,26,13,228,188,129,64,85,110,247,122,109,50,136,161,88,144,185,28,5,249,23,161,211,233,225,144,36,204,159,55,27,165,118,27,134,189,52,28,221,123,244,196,144,129,79,251,149,223,176,23,135,163,255,179,
3,213,231,199,79,154,2,65,208,66,81,92,136,79,120,20,103,79,159,194,146,133,243,253,202,246,94,117,239,209,19,83,166,165,161,231,99,157,160,40,138,26,248,45,200,92,142,13,31,175,197,154,143,178,212,101,
43,62,223,62,169,35,70,188,58,26,118,187,13,225,225,243,0,0,75,22,205,199,145,131,7,248,253,66,53,222,190,49,13,61,17,17,17,53,10,113,241,137,24,49,106,52,114,179,179,17,102,52,98,203,103,159,194,233,
116,98,193,146,229,40,46,42,68,65,126,30,122,247,237,135,53,31,173,192,222,221,59,97,16,69,44,203,90,133,179,167,79,193,104,50,193,106,177,32,107,121,38,4,65,192,138,53,159,160,32,63,15,130,32,64,43,104,
145,62,99,26,0,96,65,230,114,56,164,50,56,157,78,24,77,38,216,109,182,187,218,215,91,5,114,222,32,178,184,168,16,178,236,172,244,253,17,145,81,208,233,244,234,50,222,0,205,27,4,121,123,131,10,242,243,
106,164,108,29,82,25,198,141,121,21,0,48,104,240,80,12,122,126,8,50,23,101,0,0,206,157,57,133,222,125,147,213,0,71,16,4,196,37,36,250,5,186,0,144,158,54,13,165,118,27,116,58,61,122,247,75,198,188,140,
69,120,121,232,115,106,176,116,175,6,62,63,4,123,119,239,68,239,190,201,216,190,237,11,245,249,220,236,243,232,222,227,73,191,0,172,87,159,126,56,119,230,148,223,251,247,238,222,165,46,19,23,159,136,180,
244,153,112,148,73,200,205,57,207,147,139,106,20,135,32,18,17,17,81,227,8,192,18,18,97,183,217,176,230,163,44,204,153,149,174,6,0,98,104,40,210,211,166,97,205,71,89,72,77,153,132,94,125,251,1,0,218,39,
117,196,246,109,95,32,115,81,6,82,83,38,161,123,143,39,213,158,175,220,236,243,200,152,55,27,115,102,165,35,34,42,10,6,81,84,3,166,212,148,73,152,51,43,253,174,131,47,65,16,176,110,227,103,232,213,183,
31,6,61,63,20,3,159,31,226,23,88,45,88,178,28,113,9,137,152,57,123,94,165,67,19,219,37,117,192,83,207,12,64,92,66,34,214,109,252,12,17,145,81,48,26,77,152,59,127,161,186,76,167,174,221,48,232,249,161,
181,82,206,146,84,134,112,115,107,245,241,225,67,7,208,219,83,166,238,253,235,136,115,103,78,87,249,126,89,118,98,243,166,141,112,58,157,136,75,168,153,33,127,17,145,81,112,72,18,54,111,218,136,167,158,
25,16,176,191,86,203,21,117,8,167,55,64,188,213,62,230,230,156,199,134,143,215,162,127,133,117,17,213,132,42,123,192,190,220,247,87,150,14,85,219,211,189,30,99,33,16,17,209,125,181,225,227,53,24,246,210,
8,124,186,229,75,156,61,125,10,89,203,51,221,23,224,101,101,106,79,82,113,81,33,162,99,218,186,3,133,246,29,208,202,108,70,167,46,93,1,192,61,252,204,220,26,237,146,58,160,125,135,142,136,136,138,82,47,
224,69,49,20,113,9,137,200,205,206,86,183,119,33,39,27,143,196,39,220,241,126,70,199,180,69,113,81,33,230,204,74,7,0,76,127,123,166,250,218,227,79,60,137,53,31,173,192,201,227,71,97,16,69,44,88,178,28,
123,119,239,244,123,255,185,51,167,213,224,65,150,157,104,151,212,1,27,62,94,171,6,34,197,69,133,232,221,39,89,125,174,38,232,244,122,140,120,117,12,0,160,119,223,126,88,178,48,67,125,205,81,38,161,32,
47,15,237,146,58,224,220,153,211,232,255,204,0,204,159,55,27,221,123,60,121,203,117,90,45,87,32,138,161,53,178,127,79,61,51,0,123,247,236,68,137,213,2,69,81,212,114,240,218,190,237,11,244,238,147,140,
115,103,78,163,83,215,110,56,121,252,24,12,183,217,182,213,98,193,227,183,57,6,162,123,10,192,42,75,147,216,66,171,69,80,16,59,201,168,106,65,65,65,104,217,178,37,211,170,82,131,194,52,193,212,80,234,
39,207,199,59,43,15,69,81,176,230,163,44,108,248,120,13,166,76,75,195,160,193,67,113,248,224,1,136,161,161,208,233,244,144,101,167,223,133,121,65,65,30,172,86,139,223,208,52,192,125,191,149,65,20,213,
0,73,13,66,116,122,12,26,124,179,183,234,110,19,74,68,199,198,162,184,168,72,125,124,41,63,79,13,6,186,247,120,18,113,9,137,234,118,42,27,130,216,46,169,3,30,127,226,73,136,98,40,140,38,19,14,123,238,
83,218,177,237,11,244,234,211,207,211,67,101,174,149,161,115,185,217,231,3,202,11,0,246,238,217,137,254,207,12,112,223,119,167,21,252,238,7,171,74,184,185,53,142,28,58,112,207,251,164,211,233,209,187,
111,50,34,34,163,208,255,153,1,16,69,17,79,61,51,0,25,243,102,251,5,173,41,211,210,96,16,69,244,238,147,140,249,243,102,223,54,241,73,184,217,12,71,153,196,239,23,170,241,246,188,202,30,176,102,26,13,
180,90,45,130,131,121,155,24,85,77,227,169,39,68,68,68,247,91,239,190,201,208,233,245,176,219,220,61,89,91,62,251,20,0,224,114,185,48,115,246,60,156,59,123,26,189,250,244,83,47,250,15,31,220,143,149,107,
62,129,78,167,195,201,227,199,16,29,27,139,13,31,175,197,185,51,167,240,218,216,241,152,50,45,13,71,14,30,64,187,164,14,200,90,158,137,220,156,243,72,73,77,195,152,55,198,67,81,20,116,238,218,13,249,121,
23,239,120,63,173,22,11,186,63,241,164,95,32,39,203,50,0,32,39,251,60,206,157,61,93,101,242,7,65,16,144,50,45,13,163,70,188,8,135,36,169,189,82,0,176,119,247,78,52,182,36,3,0,0,30,252,73,68,65,84,172,
219,248,57,74,172,22,191,123,160,106,130,236,116,86,26,120,85,12,112,70,188,58,166,90,219,142,142,137,133,209,104,66,65,222,189,223,163,214,187,95,50,182,111,221,162,222,147,38,8,2,118,238,63,140,204,
133,243,253,150,59,124,240,0,134,189,232,158,54,224,118,1,162,32,8,120,234,153,1,216,240,201,218,122,95,239,135,189,52,28,253,159,25,128,1,79,245,169,181,109,132,25,77,120,237,141,241,104,101,54,171,129,
184,183,188,107,67,171,112,51,94,27,59,30,58,189,30,130,160,69,113,81,161,95,64,93,155,250,63,51,0,41,169,51,208,241,209,71,106,109,27,140,174,136,136,238,209,169,83,23,184,13,110,131,234,129,252,188,
139,136,136,138,130,209,100,66,198,188,217,40,46,42,68,152,209,4,135,84,134,37,11,231,163,123,143,158,234,240,62,0,112,72,18,70,141,120,17,221,123,244,244,220,19,228,78,202,160,40,10,70,13,127,81,237,
141,242,237,73,26,50,240,105,117,88,218,168,225,47,194,32,138,119,188,159,222,0,207,27,60,181,239,208,209,175,23,43,45,125,22,140,70,119,82,16,163,201,132,205,155,54,170,239,85,20,197,29,252,117,233,6,
65,171,69,167,46,93,113,242,248,49,245,181,179,167,79,97,196,171,99,240,242,176,231,234,188,252,15,31,60,128,254,207,14,8,8,124,188,6,61,63,4,178,44,195,32,138,136,79,72,68,106,202,196,42,147,140,220,
137,94,125,250,249,245,86,42,138,130,195,7,15,160,119,223,100,88,45,22,191,0,117,211,23,219,241,118,218,212,74,215,19,151,144,128,17,112,127,38,221,123,60,137,195,7,15,220,113,22,196,186,54,119,254,194,
26,75,180,114,187,0,124,251,182,47,212,115,97,237,134,207,16,23,159,88,107,9,74,28,146,132,15,151,101,170,89,50,231,206,95,136,78,93,186,169,231,110,109,6,126,189,250,38,195,110,251,166,86,183,195,0,140,
136,136,136,26,133,18,171,165,202,180,230,37,149,12,53,244,94,232,249,6,56,234,5,167,236,172,178,39,199,247,162,252,110,2,8,69,81,240,242,208,231,208,189,71,79,184,20,23,70,13,127,17,58,189,123,158,170,
130,252,60,76,155,50,17,157,187,118,67,184,217,172,222,235,181,119,247,78,53,99,226,164,241,111,160,115,215,110,144,164,50,140,30,241,162,223,189,76,231,206,158,134,209,100,130,67,146,106,172,92,29,82,
25,210,211,166,85,250,218,134,143,215,170,219,218,240,241,90,28,57,116,64,205,106,152,158,54,13,14,169,12,0,144,185,48,195,111,46,174,204,133,243,107,36,251,161,32,8,106,176,237,107,205,71,89,106,202,
124,233,227,50,181,14,140,26,254,130,186,172,239,189,117,103,207,156,242,75,170,178,119,247,206,106,13,163,188,223,150,44,204,64,169,221,86,235,201,66,100,217,233,23,108,149,88,44,136,136,138,170,181,
0,76,150,157,126,231,86,152,209,84,163,83,63,84,85,151,210,210,103,97,246,204,183,177,44,107,21,3,48,34,34,34,162,187,161,40,174,26,25,230,86,243,251,165,248,5,0,190,23,155,37,86,75,64,2,13,223,96,160,
212,110,243,11,26,253,230,223,122,105,56,214,172,92,81,227,251,90,85,47,139,111,192,91,113,142,53,223,247,84,12,144,106,123,223,28,146,164,6,134,190,101,235,187,172,111,185,249,46,223,144,220,143,32,49,
204,104,66,116,108,44,150,44,154,95,171,219,137,136,140,194,83,207,12,64,124,66,162,103,104,240,233,90,221,222,136,87,199,224,200,161,3,40,177,90,32,134,134,214,234,182,152,97,131,234,133,136,200,40,12,
123,105,120,141,174,115,252,196,41,119,53,52,132,136,136,26,15,135,36,169,115,120,53,102,189,251,38,99,105,214,71,56,119,230,52,231,173,162,90,13,190,22,102,46,199,252,121,179,107,61,96,149,164,50,28,
57,120,0,135,15,30,112,79,187,16,159,88,107,219,138,139,79,68,184,217,92,105,111,56,3,48,106,180,116,58,253,93,103,147,170,50,168,139,138,130,32,212,94,130,16,239,36,151,85,125,17,86,149,93,105,230,123,
243,96,16,69,24,68,17,51,223,155,119,79,65,43,3,76,34,34,2,220,67,230,198,141,121,181,70,83,207,19,85,22,124,45,89,56,191,214,123,163,0,247,143,39,185,57,238,172,155,39,143,31,173,213,97,150,227,39,77,
65,152,209,132,165,89,31,97,105,214,71,0,128,165,89,31,213,218,117,214,45,211,208,19,85,87,85,245,167,186,245,170,28,229,126,203,235,116,122,68,68,69,193,106,185,2,135,36,65,167,211,195,104,50,249,13,
97,136,142,137,133,213,98,129,44,59,17,110,54,195,32,134,34,55,251,188,239,74,81,94,94,94,107,117,251,65,157,14,3,159,31,130,217,51,223,174,164,145,50,86,121,252,209,177,177,8,9,17,212,255,223,233,254,
245,238,155,140,97,47,143,128,163,204,61,166,93,81,92,152,52,254,13,86,194,187,172,175,76,19,76,245,189,61,109,236,245,241,78,207,71,158,159,196,250,124,103,215,98,53,21,124,45,200,92,142,197,11,230,171,
201,106,106,83,184,217,172,38,80,17,4,1,113,241,9,176,90,44,181,118,140,47,15,245,79,90,115,250,252,223,48,118,244,200,90,251,76,120,15,24,213,59,6,81,196,194,204,15,112,246,204,41,12,26,60,20,151,242,
243,240,249,167,27,176,48,243,3,12,30,208,31,178,236,132,65,20,49,47,99,49,158,77,238,165,246,54,229,230,156,199,132,73,41,72,79,155,234,151,245,232,78,233,116,122,232,244,122,104,181,2,12,98,40,236,54,
27,74,237,54,196,37,36,66,113,185,212,241,227,178,211,233,215,85,109,16,69,132,155,91,67,118,6,222,144,29,17,25,5,157,94,15,171,229,74,149,219,21,4,1,209,177,109,225,144,202,170,220,255,206,93,187,97,
208,224,161,24,245,242,11,53,146,57,234,78,143,217,43,46,193,61,12,160,184,176,80,221,143,112,179,25,82,153,132,112,179,25,130,86,235,31,12,251,148,129,119,221,68,68,68,212,48,188,50,106,12,204,230,214,
88,180,244,3,245,185,156,243,95,97,236,152,145,181,178,189,206,93,31,195,176,151,134,195,110,179,33,50,234,15,216,179,107,7,86,175,204,106,52,229,201,0,140,234,157,23,94,26,129,189,187,119,98,243,166,
141,16,4,1,235,62,221,140,207,63,221,128,195,7,247,163,251,147,61,177,125,235,22,116,239,209,19,135,15,238,135,162,40,24,52,120,40,198,142,30,9,89,118,66,146,36,116,239,209,243,158,78,210,136,168,40,164,
164,206,64,113,225,215,112,72,18,58,119,237,134,156,236,243,80,92,46,53,75,85,122,218,52,24,196,80,76,152,148,130,177,99,70,194,32,138,216,244,197,118,156,56,118,212,61,52,49,234,15,56,114,112,63,0,160,
93,82,71,204,72,159,133,61,187,119,224,181,177,19,170,220,238,188,140,69,106,6,38,67,104,40,82,167,76,12,88,198,155,130,181,38,131,175,59,57,102,0,152,57,123,30,244,158,64,42,37,117,6,210,167,79,69,113,
81,33,134,189,52,2,162,40,66,146,36,40,46,23,102,164,207,194,179,201,189,1,0,131,6,15,69,255,103,7,226,196,177,191,226,9,207,231,227,123,243,57,17,17,17,221,61,239,247,109,109,153,61,243,237,74,71,252,
212,150,245,235,214,96,253,186,53,247,173,60,59,36,254,39,3,48,106,90,194,76,38,196,37,36,162,115,151,110,0,0,71,89,25,116,122,61,246,238,222,137,148,212,25,106,0,54,123,230,91,8,51,154,16,110,110,141,
121,25,139,212,247,215,68,166,37,81,20,49,196,19,112,24,68,17,178,211,137,37,11,231,67,16,4,108,221,185,47,96,249,246,73,29,113,226,216,81,181,113,242,77,95,218,255,217,1,120,59,109,42,114,179,207,35,
204,104,194,182,93,129,239,143,142,137,133,211,233,196,124,207,36,131,43,215,174,71,152,209,20,208,83,20,29,219,182,214,110,16,173,206,49,27,68,17,70,211,239,48,106,248,11,0,110,222,235,182,196,51,231,
139,36,73,106,25,44,204,92,142,232,152,88,20,228,231,161,255,179,3,213,94,187,125,187,119,34,45,253,93,6,96,68,68,68,212,36,49,0,163,122,231,82,126,30,46,229,231,5,252,242,225,144,36,8,130,128,206,93,
187,65,113,185,212,97,122,14,169,12,111,167,77,173,209,108,60,57,62,195,231,20,69,81,131,58,247,4,152,174,74,3,40,223,33,119,249,249,23,213,255,199,39,36,98,246,59,238,160,164,212,110,171,116,114,191,
246,29,58,34,38,182,173,26,184,41,46,23,194,205,173,3,2,176,82,219,55,106,143,84,77,171,206,49,199,39,60,138,130,188,139,126,239,25,246,242,8,192,19,128,249,14,85,148,36,9,130,86,139,112,179,25,138,203,
165,246,218,89,45,22,132,155,205,208,233,244,53,222,147,119,191,116,236,248,200,61,189,191,58,147,250,114,27,77,111,27,68,68,196,0,140,168,78,28,58,176,31,11,151,126,128,7,117,58,228,102,103,35,50,42,
74,13,198,182,111,251,2,19,38,165,224,195,101,153,234,242,159,111,218,136,89,179,223,87,123,84,100,217,137,19,199,238,109,166,244,202,238,227,186,21,171,213,130,86,102,179,250,56,220,220,26,151,61,1,76,
113,97,161,154,64,68,16,4,191,9,51,125,3,153,232,152,216,219,38,211,40,42,42,68,231,46,221,2,238,175,170,9,213,57,102,73,42,243,203,252,24,25,21,133,220,243,95,221,12,220,92,129,193,169,221,102,131,209,
100,130,32,8,80,20,5,6,207,48,197,198,18,124,17,17,17,17,213,72,0,166,213,106,209,188,121,115,4,5,49,83,61,85,45,40,40,8,130,32,220,243,122,138,11,11,177,65,114,7,89,165,118,27,38,141,123,29,157,187,118,
67,76,108,44,206,158,190,153,109,103,239,174,29,176,90,174,248,245,194,108,222,180,17,197,69,133,136,79,72,132,36,73,40,242,4,39,75,22,206,135,67,42,171,147,114,56,113,236,40,22,46,253,0,63,201,50,12,
6,17,145,145,81,106,0,230,29,58,121,248,224,126,68,199,196,86,218,131,150,155,125,30,175,141,157,128,148,105,105,56,113,252,24,98,98,99,241,229,214,45,1,189,122,155,55,109,196,178,21,171,177,181,107,55,
28,58,184,31,87,61,61,85,247,26,112,86,87,65,222,69,56,101,39,82,166,165,193,106,181,160,255,179,3,49,251,157,183,110,249,30,69,81,176,121,211,70,204,156,243,62,206,157,57,141,238,79,60,137,237,91,183,
240,228,33,34,34,162,166,29,128,85,76,147,24,27,27,11,131,193,192,0,140,110,27,128,133,132,132,224,218,191,190,175,244,245,234,166,11,117,58,127,132,211,249,163,250,216,110,251,6,159,127,186,33,96,57,
151,203,133,28,159,30,23,175,156,243,95,5,60,95,84,248,245,93,29,83,209,215,95,67,42,251,78,221,247,245,235,86,195,233,116,170,143,223,154,62,21,229,229,229,144,202,190,195,226,5,127,65,121,121,57,236,
182,111,240,234,75,195,240,196,147,61,81,244,245,215,248,100,221,106,8,130,22,229,229,229,216,179,107,7,178,207,127,133,46,221,30,195,234,149,31,98,253,186,53,144,202,190,243,91,23,0,76,28,251,26,58,119,
123,12,209,49,177,40,42,252,26,206,31,127,12,40,63,169,172,12,47,13,30,136,248,132,71,17,29,219,22,138,226,130,229,202,255,220,115,90,214,234,30,179,203,229,194,123,233,111,33,62,49,17,225,230,214,152,
54,121,130,58,20,180,226,123,214,175,91,13,169,76,66,121,121,57,86,173,248,16,113,9,137,136,79,120,20,171,86,126,88,43,61,120,119,130,105,232,169,62,99,26,250,166,93,30,196,250,76,141,183,61,15,8,192,
42,122,232,55,15,0,229,191,0,215,89,120,116,11,215,129,107,87,171,126,249,223,66,13,248,182,204,209,160,14,73,150,157,126,195,227,42,166,132,247,246,190,249,222,39,229,125,223,151,85,244,236,148,218,109,
149,6,148,190,61,121,178,236,196,222,93,59,110,187,127,138,162,224,236,153,83,56,91,131,243,112,84,247,152,189,203,186,123,220,252,123,221,42,190,167,226,227,220,236,243,247,61,240,242,214,73,34,34,34,
162,251,133,221,91,68,68,68,68,68,68,12,192,168,177,96,143,3,177,46,18,17,17,17,185,169,67,16,53,26,22,6,213,158,135,126,235,190,240,109,104,195,17,169,105,6,94,108,15,233,126,170,88,255,26,123,125,188,
221,241,53,181,242,32,214,103,106,252,245,129,105,232,169,94,95,8,19,17,17,17,17,53,38,28,130,72,68,68,68,68,68,196,0,140,136,136,136,136,136,136,1,24,17,17,17,17,17,17,221,5,222,3,70,68,116,143,78,157,
186,192,109,112,27,68,68,68,119,26,128,49,13,11,17,17,219,67,170,127,245,79,211,196,207,55,13,207,79,98,125,166,70,85,31,216,3,70,181,106,199,129,99,44,132,70,228,169,39,187,178,16,136,136,136,136,238,
1,3,48,170,117,45,180,90,4,5,241,118,195,134,44,40,40,8,45,91,182,100,65,16,17,17,17,49,0,163,250,172,153,70,3,173,86,139,224,96,86,181,134,76,227,249,28,137,168,254,8,51,154,208,255,217,1,184,170,40,
88,189,50,235,158,214,213,187,111,50,0,96,239,238,157,152,48,41,5,123,119,239,68,113,81,225,61,173,115,194,164,20,172,255,120,13,28,146,196,15,139,136,136,1,24,17,17,81,195,246,231,212,52,156,56,126,12,
5,121,23,239,121,93,70,147,73,253,127,68,100,20,116,122,253,61,175,51,34,50,10,130,192,31,110,136,136,42,226,184,48,34,34,162,6,38,204,104,66,68,228,31,96,181,92,129,36,149,1,0,12,162,136,184,132,68,24,
68,209,111,89,157,78,143,184,132,68,132,25,77,126,207,123,151,143,136,140,170,116,27,113,9,137,136,75,72,132,32,8,149,62,95,89,192,85,213,250,12,162,88,229,118,136,136,154,26,246,128,17,17,17,53,48,125,
250,37,67,208,10,136,79,72,132,236,116,66,175,215,99,230,236,247,113,246,204,41,188,242,234,24,28,62,116,0,219,183,110,129,65,20,177,108,197,106,156,59,125,10,97,38,19,172,150,43,200,90,150,9,65,16,240,
209,218,245,200,201,62,15,65,16,16,110,110,141,147,199,143,170,235,127,225,165,225,176,219,108,208,233,245,120,109,236,4,140,26,254,2,0,247,176,66,89,118,34,68,16,144,146,58,3,67,6,244,7,0,188,50,106,
12,30,239,209,19,71,14,238,71,136,32,248,13,95,12,51,154,48,47,99,17,102,207,124,155,31,28,17,17,3,48,34,34,162,134,103,245,202,44,244,238,155,172,222,251,149,150,62,11,155,55,109,196,222,221,59,161,211,
233,177,114,221,122,108,223,186,5,237,147,58,98,251,214,45,216,188,105,35,4,65,192,166,47,182,99,205,202,44,180,239,208,17,57,217,231,213,160,104,89,214,42,191,245,231,230,100,99,253,186,53,234,107,17,
145,81,40,46,42,196,146,133,243,213,101,226,19,30,85,159,239,221,55,25,131,7,244,135,162,40,126,235,49,154,76,152,48,41,5,179,103,190,125,207,247,148,17,17,49,0,35,34,34,162,122,33,220,220,26,225,230,
214,106,50,13,71,89,25,12,162,136,118,73,29,16,110,110,141,206,93,186,1,128,218,171,21,23,159,136,220,236,243,62,1,215,121,191,245,73,101,101,234,255,139,139,10,17,151,144,136,226,162,66,140,25,59,30,
162,24,10,163,209,132,112,179,25,58,189,30,209,49,177,176,219,108,1,193,23,224,238,49,83,20,5,86,203,21,126,72,68,68,12,192,136,136,136,26,135,156,236,175,80,98,177,96,239,238,157,126,207,23,228,231,161,
196,106,9,200,146,88,92,84,232,119,79,86,171,112,51,74,172,22,245,177,111,82,142,112,179,25,135,15,238,71,187,164,142,136,140,140,194,164,241,111,0,0,62,253,98,187,59,168,179,219,16,17,85,249,253,93,179,
103,190,141,222,125,147,241,218,216,9,126,189,103,68,68,12,192,136,136,136,168,193,58,114,240,0,230,101,44,130,209,100,66,126,94,30,98,98,99,177,122,101,22,14,31,220,143,143,214,174,71,136,32,224,220,
153,211,136,137,109,139,245,235,214,224,196,177,163,120,101,212,24,56,28,18,12,6,17,49,177,109,253,2,176,78,93,186,193,233,116,194,104,52,193,32,134,162,32,63,15,17,145,81,8,51,253,14,157,187,118,67,92,
252,205,36,28,14,73,194,137,99,71,49,55,99,17,206,157,57,13,65,16,176,121,211,70,0,128,211,233,196,135,203,150,96,217,138,213,232,220,181,27,78,28,59,202,15,139,136,154,60,53,11,162,134,127,252,171,133,
63,106,92,154,74,61,101,61,166,134,112,158,165,79,159,170,254,255,114,81,33,198,142,30,9,69,81,16,25,21,133,115,167,79,65,3,224,123,73,194,168,225,47,224,123,73,66,124,66,34,114,207,127,5,13,128,159,100,
39,134,12,232,15,71,89,25,114,115,206,99,212,240,23,176,119,215,78,104,0,100,46,156,143,113,163,71,226,39,167,19,165,118,27,198,141,30,169,110,227,157,233,83,17,17,25,133,35,7,247,99,242,184,215,113,185,
176,16,26,0,115,102,190,141,172,101,75,32,138,34,46,229,93,84,215,243,189,84,134,171,138,130,212,41,111,162,212,102,171,145,243,177,58,127,90,65,64,76,76,108,192,243,70,163,9,241,9,137,234,95,197,101,
68,81,68,124,66,34,180,130,112,203,245,207,154,61,175,201,126,175,71,70,70,33,50,50,170,202,215,227,19,18,171,124,45,38,38,182,202,178,237,211,55,25,237,147,58,54,185,239,23,254,53,205,235,98,246,128,
17,17,17,53,64,5,249,121,126,143,75,237,54,108,240,36,206,240,229,144,36,181,71,202,151,162,40,1,67,22,1,168,201,50,170,122,173,170,100,26,37,22,11,214,248,12,117,244,93,206,33,73,117,58,33,179,65,12,
197,204,57,239,99,64,114,111,191,231,123,247,75,70,239,190,201,176,219,108,158,253,42,67,122,218,52,132,25,77,152,57,231,125,132,155,205,40,46,44,68,124,226,163,88,191,110,53,178,150,101,86,186,254,232,
216,182,77,174,190,197,37,36,98,230,236,247,33,203,78,79,25,139,152,61,243,109,156,244,244,106,246,238,155,140,94,158,123,16,197,212,80,172,94,153,133,35,7,247,3,0,198,79,74,65,156,39,99,167,78,175,199,
145,67,7,2,234,106,152,201,196,147,154,154,12,6,96,68,68,183,161,213,106,209,188,121,115,4,5,113,234,68,170,61,65,65,65,1,115,110,81,205,219,187,123,167,95,160,8,0,19,38,167,224,220,153,83,24,55,250,21,
40,138,2,131,40,98,230,236,247,209,255,217,129,216,190,117,203,45,215,215,202,108,134,226,82,80,106,183,65,16,4,68,199,182,133,36,149,161,196,98,81,3,23,223,132,39,173,204,102,200,78,103,64,64,170,211,
233,17,17,21,5,197,229,66,65,126,30,162,99,98,97,181,88,252,2,30,157,94,15,197,165,64,81,92,16,197,80,232,244,122,20,228,93,244,75,128,210,202,108,134,40,134,2,128,223,118,239,133,32,8,152,57,251,125,
164,167,77,85,215,25,29,19,139,5,75,63,192,144,188,254,112,72,18,198,140,157,128,151,135,14,10,56,174,241,147,82,96,52,153,48,124,232,115,119,180,189,232,216,182,234,182,188,199,228,61,86,239,156,118,
165,118,155,186,124,68,100,84,192,143,18,192,205,137,197,189,137,96,116,122,189,250,217,120,143,195,123,79,164,213,98,65,68,84,148,223,231,231,187,63,0,96,181,92,169,211,31,19,136,1,24,17,81,147,20,27,
27,11,131,193,192,0,140,106,61,0,11,9,9,193,141,159,127,96,97,212,33,157,78,143,206,93,31,195,146,5,243,213,64,198,33,73,56,121,252,40,122,245,77,190,101,0,214,46,169,35,70,140,26,131,201,227,94,7,0,204,
205,88,164,246,174,137,161,161,72,157,50,17,35,94,29,131,205,250,141,106,79,209,188,140,197,152,54,229,77,191,139,248,136,200,40,76,73,157,129,115,103,78,193,96,16,161,211,183,68,187,164,14,112,56,36,
181,167,104,194,164,20,156,56,126,12,225,102,51,218,37,117,68,169,237,27,72,146,132,153,179,223,199,144,1,253,33,203,78,180,75,234,136,180,244,89,216,187,123,7,116,186,192,224,236,110,181,235,208,17,178,
236,244,11,232,10,242,243,80,144,119,17,221,123,244,196,230,77,27,97,183,219,16,151,240,168,218,235,229,213,185,107,183,59,154,3,78,16,4,204,205,88,132,35,7,15,32,215,167,140,207,157,57,133,49,99,39,32,
107,217,18,0,192,176,151,134,99,178,39,33,76,167,174,143,161,115,151,174,72,79,155,230,183,174,185,25,139,96,181,92,129,162,40,232,213,55,25,89,203,150,96,97,230,7,24,226,153,50,161,149,217,140,180,119,
222,197,144,1,253,177,96,233,7,200,205,62,15,171,229,10,162,99,218,162,32,255,162,58,103,222,138,181,235,81,106,183,161,184,168,16,6,49,52,224,24,137,24,128,17,17,213,48,163,225,65,0,46,224,6,203,130,
106,209,13,224,198,53,22,67,109,235,221,55,89,77,34,114,238,236,105,228,158,255,10,118,219,55,106,111,138,26,96,228,93,196,160,193,67,171,21,124,201,178,19,209,49,177,144,157,78,100,204,155,13,0,88,177,
118,61,194,140,38,108,254,108,35,122,247,77,198,201,99,71,17,29,19,11,169,204,191,119,197,27,128,41,46,23,182,111,221,162,6,102,118,251,55,88,152,249,1,54,172,91,3,157,78,143,232,216,182,152,61,243,109,
132,191,60,2,138,203,165,6,27,130,32,160,93,135,142,56,114,112,63,6,13,30,130,201,227,95,175,241,57,215,194,205,173,145,115,254,171,128,231,115,115,178,209,42,220,12,0,72,157,242,38,82,82,103,224,181,
177,227,241,225,178,76,28,57,184,95,237,181,171,110,79,156,160,213,170,193,151,119,8,236,160,193,67,144,62,125,42,74,237,54,20,228,229,161,87,223,100,204,153,249,54,82,82,103,32,204,104,66,169,221,134,
222,125,251,97,195,199,107,3,214,23,29,211,22,155,55,109,244,15,28,243,46,162,93,135,142,56,121,236,40,122,251,4,216,122,125,75,117,89,239,4,230,89,203,50,209,189,71,79,148,218,109,72,157,50,177,73,156,
31,222,222,62,111,79,236,237,150,243,85,92,88,168,246,216,122,197,37,36,86,250,60,3,48,47,222,105,78,68,183,163,225,113,18,177,62,214,241,241,105,238,162,60,52,85,47,155,147,125,30,251,60,23,247,146,84,
6,89,118,194,104,250,29,116,122,189,223,69,98,120,235,214,112,58,157,149,174,67,167,215,35,37,53,13,39,142,29,133,252,147,123,153,118,29,58,34,58,182,45,150,174,112,79,106,173,40,46,132,183,110,141,147,
199,143,98,194,164,20,24,66,69,244,234,151,140,125,123,118,6,172,115,239,158,157,48,132,134,98,217,138,213,200,201,254,10,89,203,50,81,98,181,160,184,168,16,237,58,116,132,209,104,194,137,99,71,161,92,
117,247,102,21,228,95,84,215,225,112,72,208,106,5,64,3,196,37,60,138,226,226,194,26,175,51,118,187,13,237,146,58,4,172,87,157,190,64,227,222,143,212,148,137,104,21,110,70,74,234,12,232,245,122,108,223,
182,5,130,160,173,246,254,12,26,60,20,58,157,30,243,231,205,6,52,238,139,252,184,132,71,145,246,206,44,117,25,71,89,25,160,1,182,111,219,130,254,207,14,192,230,207,54,34,220,220,218,61,151,93,133,237,
164,166,188,137,65,207,15,197,248,73,41,200,90,158,137,115,103,78,97,195,39,107,49,226,213,49,56,121,252,40,186,247,232,137,81,35,94,80,223,231,45,87,135,67,114,15,7,214,184,239,77,171,108,221,117,90,
159,235,144,33,52,20,11,50,63,64,65,254,69,232,116,122,56,164,50,204,159,55,59,224,7,10,223,229,188,50,23,206,71,113,81,33,116,58,61,210,210,103,193,32,134,170,195,101,243,243,46,98,206,172,183,239,105,
223,30,239,209,179,225,245,62,106,110,23,128,17,17,17,17,53,114,165,118,91,192,196,211,69,133,95,35,46,33,17,39,143,223,76,147,255,72,133,201,170,125,41,46,5,47,15,125,14,75,179,86,161,83,151,110,56,121,
252,40,114,179,207,35,58,38,22,147,39,188,17,176,252,246,109,95,160,123,143,158,104,159,212,17,153,85,204,135,182,225,227,53,216,240,241,26,140,121,99,60,70,188,58,6,153,139,230,99,239,238,157,232,212,
165,43,194,205,173,253,222,87,213,176,194,130,252,139,136,136,140,170,241,30,176,220,236,175,144,246,246,44,232,116,55,131,84,65,16,208,185,107,55,140,27,51,210,111,217,18,171,5,251,118,239,196,35,241,
137,216,190,109,11,206,157,57,133,254,207,12,196,246,109,91,110,187,157,237,219,182,64,118,58,49,115,246,251,24,55,198,125,63,158,123,40,224,146,128,222,152,189,187,118,98,229,218,245,144,101,25,219,183,
125,81,69,121,228,33,125,134,59,201,202,202,53,235,49,100,96,127,117,24,225,227,61,122,162,184,168,208,111,40,104,101,229,42,203,78,180,137,136,106,82,231,136,67,42,83,63,215,65,131,135,98,208,243,67,
145,185,104,254,45,151,243,181,52,107,21,118,108,251,162,90,159,121,116,76,172,122,207,163,36,73,40,181,219,96,16,69,132,135,183,118,223,139,231,153,30,163,93,82,71,12,123,113,56,28,101,101,176,90,221,
247,225,233,116,122,68,68,70,193,110,183,5,4,136,245,29,3,48,34,34,34,106,116,116,122,61,70,188,58,230,230,5,123,37,89,29,189,50,230,189,135,153,179,223,87,47,6,59,117,233,134,112,115,107,140,27,243,74,
229,1,152,226,130,44,59,145,154,50,17,203,178,86,161,184,168,16,185,57,231,49,102,236,4,76,153,150,134,147,199,143,33,58,38,22,219,183,185,135,20,238,221,189,3,43,215,172,199,217,51,167,42,189,200,247,
110,207,155,12,194,27,80,156,60,126,20,195,94,26,1,0,213,10,170,246,237,222,137,41,211,102,96,199,182,45,8,51,154,176,230,163,172,26,186,32,151,144,185,112,62,86,174,93,143,237,219,182,64,113,41,234,253,
113,222,125,238,213,39,25,151,139,11,161,211,235,49,232,249,161,248,112,185,59,131,228,135,203,150,96,89,214,106,244,234,155,140,115,103,78,169,199,85,217,241,200,78,39,246,122,130,183,49,111,76,64,230,
162,249,216,188,105,163,231,152,190,128,83,118,66,43,8,216,187,123,39,100,217,137,156,236,175,48,232,249,161,120,121,216,160,74,247,123,252,196,20,20,20,228,65,175,211,195,110,183,169,193,227,142,109,
91,48,97,98,10,102,87,163,71,230,240,193,253,232,213,39,25,227,39,166,160,196,106,129,213,114,37,32,128,111,204,36,73,66,187,246,29,170,189,124,68,100,20,100,217,89,173,224,11,0,102,206,126,31,103,61,
245,226,200,193,3,112,72,101,152,48,49,5,86,171,197,29,92,217,108,200,92,52,31,143,63,241,36,244,122,189,59,155,166,231,115,156,59,127,49,10,242,46,66,20,69,72,146,84,105,144,200,0,140,136,136,136,168,
14,56,164,178,128,123,118,188,23,247,149,41,200,207,195,168,17,47,32,46,225,81,247,60,103,135,14,168,193,66,101,210,211,166,2,112,247,166,77,26,255,186,250,252,228,241,175,163,115,215,110,106,102,61,217,
233,84,3,24,73,146,176,163,138,158,26,111,48,18,110,54,35,107,121,166,95,112,98,181,92,193,37,159,222,159,138,199,176,119,247,78,40,138,75,253,255,217,51,167,208,189,71,207,91,222,191,115,55,182,111,219,
130,156,236,175,208,190,67,71,8,90,1,25,243,222,83,247,211,110,179,225,228,241,163,158,12,130,18,94,30,250,156,122,145,92,98,181,96,200,192,254,104,215,161,35,194,195,205,144,101,39,36,169,44,96,253,190,
199,145,49,239,61,68,199,180,85,131,53,135,84,134,118,73,29,1,0,57,217,55,239,69,203,205,201,134,40,134,86,153,149,208,187,79,118,155,13,147,125,62,167,28,79,207,166,239,103,60,118,244,43,149,126,198,
14,73,194,240,97,207,161,93,146,123,40,168,189,129,245,180,220,235,143,23,189,251,38,99,73,21,189,182,6,49,20,75,179,86,169,143,199,141,25,137,232,152,182,106,198,73,239,251,195,140,38,200,178,179,210,
233,48,0,224,114,81,145,95,192,150,62,195,125,127,163,78,167,199,202,181,235,145,185,104,62,246,237,222,9,163,233,230,143,10,35,94,117,39,102,241,62,254,98,199,94,100,45,95,82,35,73,103,24,128,17,17,17,
17,221,33,69,81,42,237,165,184,85,34,0,135,36,85,251,254,18,223,224,198,59,68,202,187,254,202,130,188,184,248,68,200,206,31,171,236,197,42,173,98,8,85,152,103,226,232,140,121,239,249,45,91,241,189,21,
143,163,170,11,221,123,85,98,181,248,29,175,239,113,231,230,156,175,178,103,72,150,157,183,45,91,223,227,168,248,249,21,228,231,5,4,148,130,32,184,135,198,45,252,75,149,235,172,106,159,70,188,58,38,160,
119,176,226,114,21,183,119,171,128,188,177,202,205,62,127,203,94,212,202,134,32,74,82,25,6,37,12,245,251,113,193,110,179,33,45,125,86,149,245,210,55,168,54,136,34,198,188,49,1,0,96,52,153,170,156,154,
35,60,220,140,136,168,63,32,46,193,157,80,199,110,183,169,189,200,12,192,136,136,136,136,106,80,80,80,16,110,220,104,56,41,73,151,102,173,130,163,172,172,90,195,221,124,141,159,152,130,112,179,25,233,
105,83,27,204,175,250,117,197,59,233,243,142,109,91,238,168,167,47,34,50,10,227,39,165,160,184,176,176,218,67,228,154,98,125,150,157,206,187,30,190,90,144,127,17,97,70,147,154,160,165,58,1,145,111,253,
30,246,226,8,92,46,46,196,230,77,27,17,17,25,133,185,243,23,121,150,113,249,7,196,103,79,195,41,223,204,58,218,208,4,3,40,119,255,49,237,23,17,221,78,163,110,39,202,111,254,203,246,144,120,222,221,231,
227,171,242,124,20,66,66,240,139,171,225,4,36,227,198,188,122,87,159,101,230,162,140,38,84,7,238,204,222,221,187,176,119,247,174,59,46,155,226,162,34,159,207,163,230,203,52,36,36,184,17,212,103,77,53,
203,71,227,25,170,248,154,207,231,178,19,165,118,27,102,207,122,27,11,51,63,192,222,221,187,80,144,127,81,29,78,90,245,58,53,234,107,178,44,35,46,62,81,189,247,204,233,116,2,208,160,184,168,8,225,225,
173,241,120,143,158,200,205,62,143,195,7,15,96,216,75,35,48,126,98,10,78,30,63,134,78,93,186,86,56,103,238,191,91,213,7,111,0,198,217,109,168,86,104,181,90,52,111,222,156,19,216,54,112,65,65,65,85,14,
3,104,36,174,123,218,65,13,219,67,170,111,254,45,212,128,111,203,28,141,246,216,238,244,124,12,105,96,1,24,53,29,66,72,72,131,175,207,14,169,44,96,66,235,170,150,75,157,50,201,239,57,239,61,143,71,14,
30,64,65,94,30,226,19,18,17,29,211,22,178,236,172,114,157,233,105,211,224,240,185,39,112,195,199,107,208,169,107,55,136,98,40,178,150,103,66,167,211,3,112,247,146,141,29,51,18,157,187,118,67,177,78,15,
135,36,97,236,152,145,232,222,163,167,39,131,233,177,134,82,31,110,120,3,176,235,0,56,245,35,213,138,216,216,88,24,12,6,6,96,141,32,0,11,9,9,193,141,95,254,217,88,15,241,134,183,81,100,123,72,84,191,207,
199,22,205,181,248,197,229,194,213,171,60,85,169,254,104,214,44,8,15,180,104,222,224,235,179,59,245,127,94,181,150,187,85,54,200,82,187,237,150,153,71,189,42,110,75,81,20,28,57,120,192,39,208,187,153,
96,165,196,106,241,155,112,187,54,239,119,172,197,250,112,29,192,141,96,79,101,80,190,249,198,246,195,239,126,103,250,53,79,33,170,73,70,81,7,64,185,217,249,78,13,211,117,224,198,245,198,121,104,54,155,
237,26,0,23,128,171,158,47,72,23,219,67,170,111,26,99,47,88,101,189,95,213,61,31,245,15,62,136,31,101,25,215,174,93,103,229,160,122,113,177,221,82,175,131,70,163,97,125,166,219,213,7,5,192,245,32,79,197,
248,233,135,31,127,252,111,22,25,17,53,185,216,242,70,249,255,3,240,51,128,95,60,255,178,61,164,122,27,176,84,49,100,175,209,28,71,117,207,199,224,224,102,48,252,250,87,120,240,129,22,8,14,110,198,202,
65,247,69,112,179,102,120,160,69,115,24,126,253,43,132,4,7,179,62,179,62,84,183,62,40,193,158,74,225,12,17,154,231,219,109,182,4,147,201,196,18,36,162,166,19,128,93,191,241,127,1,252,19,192,79,112,255,
66,249,35,219,67,170,207,30,106,4,65,88,77,157,143,186,7,90,64,247,64,11,86,10,98,125,166,134,82,31,156,0,126,105,6,64,11,32,228,59,199,15,223,182,8,105,246,232,245,107,215,126,163,215,235,89,74,68,212,
232,253,237,98,222,119,87,190,249,54,227,250,245,27,223,0,248,1,238,17,1,154,239,28,63,252,131,237,33,17,207,71,34,214,103,170,169,250,240,63,223,252,35,227,250,245,27,127,7,224,104,6,119,42,122,13,0,
77,243,22,15,148,94,83,126,249,195,245,107,215,126,229,116,58,193,138,65,68,141,145,205,102,195,255,92,177,124,231,186,161,89,242,253,63,157,23,1,124,11,64,246,124,65,222,96,123,72,196,243,145,136,245,
153,106,161,62,124,7,192,169,1,208,28,192,131,0,126,3,224,33,0,198,72,243,239,134,221,184,118,53,94,113,253,242,239,44,58,34,106,76,66,4,193,30,28,162,205,191,98,251,246,19,69,185,90,2,224,31,0,28,0,254,
5,247,16,145,22,108,15,137,120,62,18,177,62,83,45,213,7,89,3,119,15,152,0,224,1,0,122,79,197,248,53,128,150,158,231,4,0,205,192,89,0,137,168,225,42,135,59,245,171,2,247,13,176,78,184,135,132,124,15,224,
71,207,151,163,203,179,44,219,67,34,158,143,68,172,207,84,107,245,193,59,245,116,51,0,33,112,223,15,214,194,243,215,220,243,56,24,128,119,18,39,86,10,34,106,168,13,226,13,120,166,221,192,205,140,84,222,
236,84,87,113,115,158,22,182,135,68,60,31,137,88,159,169,214,234,195,255,7,0,115,200,35,166,99,238,206,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* GlobalEditor::globalEditor_864x144_png = (const char*) resource_GlobalEditor_globalEditor_864x144_png;
const int GlobalEditor::globalEditor_864x144_pngSize = 16274;


//[EndFile] You can add extra defines here...
//[/EndFile]
