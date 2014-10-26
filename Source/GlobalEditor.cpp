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
#include "DXLookNFeel.h"
//[/Headers]

#include "GlobalEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

/**
 * Ugly but usefull midi monitor to know if you are really sending/receiving something from the DX7
 * If the midi is not configured this component wont show up
 */
class MidiMonitor : public Component {
    SysexComm *midi;
public:
    MidiMonitor(SysexComm *sysexComm) {
        midi = sysexComm;
    }

    void paint(Graphics &g) {
        if ( ! (midi->isInputActive() || midi->isOutputActive() ) )
            return;
        g.setColour(Colours::white);

        Image myStrip = ImageCache::getFromMemory(BinaryData::Light_14x14_png, BinaryData::Light_14x14_pngSize);

        if ( midi->isInputActive() ) {
            g.drawSingleLineText("DX7 IN", 17,14);
            g.drawImage(myStrip, 0, 3, 14, 14, 0, midi->inActivity ? 14 : 0, 14, 14);
            midi->inActivity = false;
        }

        if ( midi->isOutputActive() ) {
            g.drawSingleLineText("DX7 OUT", 17, 28);
            g.drawImage(myStrip, 0, 17, 14, 14, 0, midi->outActivity ? 14 : 0, 14, 14);
            midi->outActivity = false;
        }
    }
};
//[/MiscUserDefs]

//==============================================================================
GlobalEditor::GlobalEditor ()
{
    addAndMakeVisible (lfoSpeed = new Slider ("lfoSpeed"));
    lfoSpeed->setRange (0, 99, 1);
    lfoSpeed->setSliderStyle (Slider::RotaryVerticalDrag);
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
    cutoff->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    cutoff->addListener (this);

    addAndMakeVisible (reso = new Slider ("reso"));
    reso->setRange (0, 1, 0);
    reso->setSliderStyle (Slider::RotaryVerticalDrag);
    reso->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
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
    transpose->setRange (0, 48, 1);
    transpose->setSliderStyle (Slider::RotaryVerticalDrag);
    transpose->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    transpose->addListener (this);

    addAndMakeVisible (oscSync = new ToggleButton ("oscSync"));
    oscSync->setButtonText (String::empty);
    oscSync->addListener (this);

    addAndMakeVisible (pitchModSens = new Slider ("pitchModSens"));
    pitchModSens->setRange (0, 7, 1);
    pitchModSens->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchModSens->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchModSens->addListener (this);

    addAndMakeVisible (lfoSync = new ToggleButton ("lfoSync"));
    lfoSync->setButtonText (String::empty);
    lfoSync->addListener (this);

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

    addAndMakeVisible (output = new Slider ("output"));
    output->setRange (0, 1, 0);
    output->setSliderStyle (Slider::RotaryVerticalDrag);
    output->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
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

    g.drawImage(myStrip, 619, 102, 14, 14, 0, lfoSync->getToggleState() ? 14 : 0, 14, 14);
    g.drawImage(myStrip, 705, 102, 14, 14, 0, oscSync->getToggleState() ? 14 : 0, 14, 14);
    //[/UserPaint]
}

void GlobalEditor::resized()
{
    lfoSpeed->setBounds (564, 50, 34, 34);
    lfoAmDepth->setBounds (686, 50, 34, 34);
    lfoPitchDepth->setBounds (646, 50, 34, 34);
    lfoDelay->setBounds (603, 50, 34, 34);
    cutoff->setBounds (234, 9, 34, 34);
    reso->setBounds (278, 9, 34, 34);
    pitchRate2->setBounds (767, 96, 34, 34);
    pitchRate3->setBounds (795, 96, 35, 34);
    pitchRate4->setBounds (823, 96, 34, 34);
    pitchRate1->setBounds (739, 96, 34, 34);
    pitchLevel2->setBounds (767, 57, 34, 34);
    pitchLevel3->setBounds (795, 56, 34, 34);
    pitchLevel4->setBounds (823, 56, 34, 34);
    pitchLevel1->setBounds (739, 57, 34, 34);
    transpose->setBounds (202, 60, 34, 34);
    oscSync->setBounds (650, 96, 48, 26);
    pitchModSens->setBounds (666, 5, 34, 34);
    lfoSync->setBounds (565, 96, 48, 26);
    pitchEnvDisplay->setBounds (751, 10, 93, 30);
    algoDisplay->setBounds (335, 30, 152, 91);
    feedback->setBounds (501, 81, 34, 34);
    algo->setBounds (501, 22, 34, 34);
    lcdDisplay->setBounds (6, 87, 140, 13);
    output->setBounds (157, 60, 34, 34);
    vuOutput->setBounds (6, 103, 140, 8);
    initButton->setBounds (3, 56, 50, 30);
    parmButton->setBounds (51, 56, 50, 30);
    sendButton->setBounds (99, 56, 50, 30);
    cartButton->setBounds (3, 111, 50, 30);
    loadButton->setBounds (50, 111, 50, 30);
    saveButton->setBounds (98, 111, 50, 30);
    storeButton->setBounds (270, 109, 50, 30);
    monoMode->setBounds (249, 65, 48, 26);
    lfoType->setBounds (583, 8, 36, 26);
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

    if (buttonThatWasClicked == oscSync)
    {
        //[UserButtonCode_oscSync] -- add your button handler code here..
        repaint();
        //[/UserButtonCode_oscSync]
    }
    else if (buttonThatWasClicked == lfoSync)
    {
        //[UserButtonCode_lfoSync] -- add your button handler code here..
        repaint();
        //[/UserButtonCode_lfoSync]
    }
    else if (buttonThatWasClicked == initButton)
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

void GlobalEditor::bind(DexedAudioProcessorEditor *edit) {
    processor = edit->processor;
	processor->algo->bind(algo);
	processor->lfoRate->bind(lfoSpeed);
	processor->lfoDelay->bind(lfoDelay);
	processor->lfoWaveform->bind(lfoType);
	processor->lfoAmpDepth->bind(lfoAmDepth);
	processor->lfoPitchDepth->bind(lfoPitchDepth);
    processor->lfoSync->bind(lfoSync);
    processor->oscSync->bind(oscSync);
    processor->transpose->bind(transpose);
    processor->feedback->bind(feedback);
    processor->pitchModSens->bind(pitchModSens);
    processor->pitchEgLevel[0]->bind(pitchLevel1);
    processor->pitchEgLevel[1]->bind(pitchLevel2);
    processor->pitchEgLevel[2]->bind(pitchLevel3);
    processor->pitchEgLevel[3]->bind(pitchLevel4);
    processor->pitchEgRate[0]->bind(pitchRate1);
    processor->pitchEgRate[1]->bind(pitchRate2);
    processor->pitchEgRate[2]->bind(pitchRate3);
    processor->pitchEgRate[3]->bind(pitchRate4);
    processor->fxCutoff->bind(cutoff);
    processor->fxReso->bind(reso);
    processor->output->bind(output);
    algoDisplay->algo = &(processor->data[134]);
    pitchEnvDisplay->pvalues = &(processor->data[126]);

    editor = edit;

    midiMonitor = new MidiMonitor(&(processor->sysexComm));
    addAndMakeVisible(midiMonitor);
    midiMonitor->setBounds(155, 21, 80, 45);

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
    monoMode->setToggleState(state ? Button::buttonDown : Button::buttonNormal, dontSendNotification);
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
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
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
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="reso" id="c8c13464e81a8d83" memberName="reso" virtualName=""
          explicitFocusOrder="0" pos="278 9 34 34" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
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
          max="48" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="oscSync" id="8f3fe641537cd00" memberName="oscSync" virtualName=""
                explicitFocusOrder="0" pos="650 96 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="pitchModSens" id="904f73df85a9f886" memberName="pitchModSens"
          virtualName="" explicitFocusOrder="0" pos="666 5 34 34" min="0"
          max="7" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <TOGGLEBUTTON name="lfoSync" id="ff92bb0a5a4f7187" memberName="lfoSync" virtualName=""
                explicitFocusOrder="0" pos="565 96 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="pitchEnvDisplay" id="9ddaae8ef924a038" memberName="pitchEnvDisplay"
                    virtualName="" explicitFocusOrder="0" pos="751 10 93 30" class="PitchEnvDisplay"
                    params=""/>
  <GENERICCOMPONENT name="algoDisplay" id="b26fb9e3b5f0bc37" memberName="algoDisplay"
                    virtualName="" explicitFocusOrder="0" pos="335 30 152 91" class="AlgoDisplay"
                    params=""/>
  <SLIDER name="feedback" id="4fac1940c29ab8c" memberName="feedback" virtualName=""
          explicitFocusOrder="0" pos="501 81 34 34" min="0" max="7" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="algo" id="8a226ddf9bbff752" memberName="algo" virtualName=""
          explicitFocusOrder="0" pos="501 22 34 34" min="1" max="32" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <GENERICCOMPONENT name="lcdDisplay" id="30c7bb8f114cbbe3" memberName="lcdDisplay"
                    virtualName="" explicitFocusOrder="0" pos="6 87 140 13" class="LcdDisplay"
                    params=""/>
  <SLIDER name="output" id="7697fdd54fd1593e" memberName="output" virtualName=""
          explicitFocusOrder="0" pos="157 60 34 34" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <GENERICCOMPONENT name="vuOutput" id="dac75af912267f51" memberName="vuOutput" virtualName=""
                    explicitFocusOrder="0" pos="6 103 140 8" class="VuMeter" params=""/>
  <TEXTBUTTON name="initButton" id="92b278163c42e21d" memberName="initButton"
              virtualName="" explicitFocusOrder="0" pos="3 56 50 30" buttonText="INIT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="parmButton" id="d5cd6260b34be817" memberName="parmButton"
              virtualName="" explicitFocusOrder="0" pos="51 56 50 30" buttonText="PARM"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="sendButton" id="9b45b291c75c7b3f" memberName="sendButton"
              virtualName="" explicitFocusOrder="0" pos="99 56 50 30" buttonText="SEND"
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
                explicitFocusOrder="0" pos="249 65 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="lfoType" id="62f61dda9f76c8f" memberName="lfoType" virtualName=""
                    explicitFocusOrder="0" pos="583 8 36 26" class="ComboBoxImage"
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

// JUCER_RESOURCE: globalEditor_864x144_png, 27571, "../Resources/ui/GlobalEditor_864x144.png"
static const unsigned char resource_GlobalEditor_globalEditor_864x144_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,3,96,0,0,0,144,8,6,0,0,0,110,232,8,32,0,0,0,6,98,75,71,68,0,255,0,255,
0,255,160,189,167,147,0,0,0,9,112,72,89,115,0,0,14,195,0,0,14,195,1,199,111,168,100,0,0,0,7,116,73,77,69,7,222,10,13,5,35,54,43,226,55,144,0,0,0,29,105,84,88,116,67,111,109,109,101,110,116,0,0,0,0,0,67,
114,101,97,116,101,100,32,119,105,116,104,32,71,73,77,80,100,46,101,7,0,0,32,0,73,68,65,84,120,218,236,157,121,124,19,213,218,199,127,169,101,226,146,112,165,9,74,203,125,109,203,181,203,149,182,8,5,20,
80,22,69,68,118,68,5,21,81,68,16,20,68,65,64,80,16,220,16,89,21,100,149,93,17,68,100,223,69,64,89,20,90,80,10,218,22,47,77,189,210,162,77,170,151,4,53,131,58,239,31,105,166,89,38,201,236,153,73,206,115,
63,124,174,157,156,217,206,60,231,156,231,123,158,231,60,199,0,192,0,32,1,64,34,0,10,192,149,53,255,174,2,96,4,80,7,192,21,53,101,12,32,66,132,8,17,125,9,3,224,111,0,127,1,184,12,192,13,224,15,159,127,
116,205,241,191,107,202,147,254,144,8,17,210,30,137,16,33,250,76,68,49,125,72,172,249,208,117,106,148,192,4,224,31,53,255,234,2,184,166,230,184,87,41,136,66,16,33,66,68,143,29,226,95,0,254,172,233,0,47,
1,112,2,248,95,205,63,23,128,223,107,58,70,144,254,144,8,17,210,30,137,16,33,250,76,68,73,125,72,172,33,241,43,1,92,11,192,10,224,250,140,70,55,220,122,227,191,210,90,39,55,184,190,81,234,13,255,151,246,
231,159,127,2,0,12,6,97,250,32,180,188,212,243,148,186,142,30,239,207,48,76,116,53,80,166,251,139,185,142,208,115,132,148,231,83,86,142,50,82,127,39,18,44,151,47,95,198,249,138,202,242,234,234,95,126,
248,239,249,202,194,239,74,203,246,3,184,0,192,30,80,212,24,170,63,76,75,189,129,237,15,165,180,123,49,125,131,82,253,137,82,215,85,74,71,149,236,19,164,180,59,49,191,9,57,30,107,109,94,106,123,76,73,
110,224,103,159,240,213,103,165,218,167,154,99,191,20,93,144,163,45,8,253,77,206,99,90,109,7,82,244,57,227,198,116,94,246,118,56,189,82,107,76,145,219,190,146,75,15,124,255,22,242,91,20,245,129,49,0,48,
215,40,195,245,102,211,213,217,237,110,187,229,209,22,249,77,59,254,247,191,255,5,0,92,184,112,65,241,15,42,71,103,165,6,236,68,27,232,162,209,153,235,5,34,133,12,8,98,7,22,45,130,24,17,97,210,160,65,
3,0,128,213,106,197,119,37,165,199,15,30,58,254,238,165,223,126,47,174,233,24,93,240,132,14,152,184,250,67,131,193,128,202,202,74,85,13,59,189,244,141,114,181,239,104,192,96,164,193,94,40,12,201,5,98,
241,208,246,197,182,199,31,127,252,17,0,130,218,163,84,3,85,75,19,35,98,116,60,240,89,124,255,150,99,60,229,106,23,66,116,87,44,96,233,165,45,136,209,231,150,205,155,177,246,118,101,101,101,72,125,18,
122,92,45,93,245,253,54,66,175,207,167,239,229,11,86,225,250,237,104,233,79,4,125,248,213,0,160,62,128,235,1,164,245,233,209,121,124,125,107,189,214,94,99,67,171,208,165,183,153,96,45,138,158,140,50,165,
65,39,92,39,32,23,32,17,143,151,246,196,98,177,192,77,255,89,178,97,203,238,23,0,216,0,84,195,19,62,80,15,192,13,222,254,208,107,236,169,101,216,105,177,111,212,99,223,35,181,109,242,5,51,57,188,91,241,
12,97,66,218,227,117,245,147,90,123,141,213,104,129,151,22,218,152,146,207,32,7,60,5,26,206,241,4,97,82,244,89,110,175,151,210,186,42,229,250,66,245,32,148,110,105,29,194,66,232,67,25,128,11,87,192,227,
6,189,46,243,95,169,157,154,54,105,220,239,135,31,126,224,221,97,121,255,9,249,88,66,207,9,60,79,174,25,97,174,127,177,2,86,190,255,18,18,18,66,190,111,52,254,137,253,54,225,206,231,83,54,92,25,33,191,
113,233,49,159,119,139,84,70,234,239,68,132,203,239,191,255,142,171,174,186,210,106,48,192,253,179,253,151,82,120,98,244,41,0,150,204,127,165,118,244,246,135,82,191,153,82,253,169,18,125,163,18,253,144,
26,125,144,148,126,62,212,239,82,250,131,112,198,137,144,227,241,212,238,249,180,199,242,242,114,217,12,84,53,218,167,156,109,73,137,118,36,164,79,17,211,30,248,182,139,112,158,59,61,77,46,73,209,103,
33,239,28,45,123,65,170,46,70,106,83,225,108,43,33,125,44,87,253,69,91,127,188,250,144,144,96,112,255,92,85,93,12,224,146,119,253,87,221,155,178,51,91,113,205,44,73,109,4,241,190,14,76,201,231,137,198,
172,157,212,89,132,112,207,193,231,218,92,231,135,58,143,79,217,112,101,132,204,156,6,134,127,132,154,29,55,24,12,188,66,62,25,134,193,207,246,95,66,85,20,161,39,129,82,223,90,143,243,120,85,85,21,254,
149,158,214,226,244,119,255,217,4,79,136,8,3,224,31,225,250,67,185,102,29,181,26,238,164,196,115,41,189,214,64,74,191,16,174,79,8,23,94,195,21,242,21,216,206,125,219,51,87,223,18,234,222,145,250,141,170,
80,125,67,28,181,71,111,93,93,190,252,39,254,112,211,248,243,207,63,65,95,190,236,249,45,82,254,2,94,125,187,12,109,73,108,30,5,25,1,68,208,152,29,41,18,3,12,191,211,2,189,18,96,130,126,242,60,87,164,
144,50,109,143,127,117,234,36,34,49,241,10,80,117,234,128,162,234,136,214,103,223,111,203,234,243,95,127,225,50,125,89,144,98,138,213,89,161,237,37,146,46,242,210,57,46,15,24,152,240,197,124,61,90,96,
66,234,147,191,231,75,61,221,225,171,15,69,223,126,191,1,192,85,137,240,44,8,188,198,116,205,85,25,191,254,250,75,212,160,75,143,107,29,162,101,12,69,19,42,133,220,91,40,172,133,186,118,164,235,200,105,
124,69,42,19,234,183,72,179,218,66,66,42,126,182,87,243,235,12,9,136,241,22,175,193,26,104,248,81,20,133,4,3,211,8,158,12,84,87,213,244,224,156,253,97,180,192,43,90,147,54,124,175,19,173,144,112,161,253,
2,159,62,129,47,140,133,106,219,161,38,100,184,126,247,150,9,213,7,113,221,239,231,170,106,210,30,107,234,226,239,191,25,92,250,237,119,252,246,251,239,162,13,73,85,141,88,158,55,149,187,61,9,25,31,253,
94,158,75,47,107,222,49,208,80,14,26,146,2,14,248,158,87,123,11,3,188,137,254,188,198,115,160,222,27,12,218,30,230,46,95,254,19,151,47,255,137,223,127,119,195,120,37,5,211,213,87,35,33,193,32,72,159,189,
223,199,87,159,57,117,73,38,240,226,173,167,60,189,76,162,251,94,14,59,134,75,191,252,116,192,231,28,246,61,12,140,159,62,121,250,85,95,219,44,224,124,5,21,74,128,62,92,13,128,74,68,77,74,204,63,254,248,
189,129,148,193,58,26,208,165,229,248,86,169,247,82,19,72,149,138,141,149,234,237,18,53,128,72,48,190,248,0,89,36,15,26,31,239,89,224,236,184,175,65,230,11,95,17,57,139,128,152,40,195,47,208,232,171,170,
170,242,238,197,66,213,12,144,87,5,246,135,106,103,86,211,82,223,40,5,182,228,234,3,164,180,113,62,237,88,10,140,113,29,15,60,198,245,55,151,151,44,240,186,190,127,255,108,255,69,251,22,169,10,237,209,
91,95,174,75,191,225,247,63,254,224,103,88,42,0,94,6,169,89,194,121,24,184,66,159,137,175,106,240,74,206,17,6,198,228,4,49,134,241,62,79,176,241,204,105,51,107,180,13,184,255,160,241,247,223,12,174,173,
107,226,173,207,190,223,193,171,207,66,224,139,175,126,8,210,213,136,225,141,194,117,46,172,190,241,0,49,78,189,242,209,169,64,176,247,234,20,195,252,173,58,132,241,212,7,35,128,58,137,240,236,55,80,71,
45,67,65,139,208,21,45,207,153,26,233,112,149,122,127,37,66,17,197,66,153,208,208,69,190,89,31,133,122,199,132,24,105,161,215,123,36,120,75,249,148,229,1,98,4,194,164,202,21,53,255,224,219,31,170,9,94,
90,74,190,33,118,29,147,220,207,44,101,162,70,74,59,247,45,19,174,31,16,234,21,11,252,155,107,226,205,55,92,145,211,67,22,31,237,61,108,123,164,233,203,178,193,151,34,222,3,129,198,173,92,205,151,235,
58,145,84,69,16,140,169,4,98,181,161,137,140,15,148,105,191,13,92,166,47,227,143,63,220,184,242,74,99,68,125,246,173,119,175,62,203,9,95,130,117,53,104,61,150,50,58,23,178,95,229,248,166,94,184,226,44,
18,224,13,243,135,53,239,67,37,176,58,164,49,125,72,4,144,224,221,136,57,65,105,67,33,218,107,193,180,10,91,122,93,96,45,54,92,80,78,168,138,54,140,241,53,210,34,193,152,247,216,79,85,213,62,6,151,175,
222,248,186,210,25,238,206,141,120,195,120,11,215,172,123,205,224,152,224,211,107,171,150,89,77,43,107,193,66,45,148,214,82,63,35,166,95,224,235,25,139,38,136,113,189,171,175,135,236,194,207,142,152,133,
48,49,237,209,77,211,170,129,151,65,142,253,112,69,26,183,114,67,25,95,24,11,11,98,2,67,19,131,192,137,55,136,249,254,55,83,99,72,107,191,13,184,47,95,14,5,96,126,250,236,119,14,77,11,130,175,175,10,139,
200,64,30,69,105,150,155,37,85,31,18,0,24,18,107,44,61,131,150,246,176,209,115,2,142,112,153,182,162,5,146,106,210,191,84,207,150,84,195,75,105,24,19,19,186,196,199,80,11,127,45,255,129,187,118,160,98,
130,7,85,226,13,147,164,190,181,159,64,249,212,191,209,92,11,22,170,191,82,162,207,150,179,255,17,211,47,200,221,198,213,2,49,95,15,153,119,29,129,223,4,76,236,183,245,144,237,145,190,252,167,104,248,
18,11,94,151,47,95,70,249,143,23,80,229,80,39,1,138,53,233,90,220,208,176,1,174,190,250,170,208,109,11,66,215,89,243,131,177,176,109,33,204,100,95,160,199,34,164,154,6,28,12,244,96,120,127,242,157,144,
244,142,125,90,111,3,151,47,255,41,88,159,57,207,137,224,41,189,202,104,68,66,66,2,25,181,85,148,132,132,4,212,173,91,87,54,125,72,212,138,161,160,183,36,28,225,140,22,45,24,86,90,130,53,177,158,45,169,
215,16,11,99,209,240,138,133,26,215,130,175,21,60,139,234,27,51,239,25,171,8,132,69,163,29,41,1,94,74,175,229,82,35,12,90,150,140,89,18,218,172,216,54,46,182,125,11,73,170,193,23,196,106,61,226,1,237,
29,241,217,214,255,250,243,47,81,116,197,71,149,67,129,221,127,202,207,227,151,95,47,226,74,138,194,21,87,92,161,232,251,49,12,3,123,245,175,248,253,15,55,154,230,102,243,126,86,33,64,198,39,104,34,108,
214,222,48,222,176,136,33,137,60,32,204,91,222,215,27,22,216,6,180,168,250,204,223,194,147,143,253,25,168,207,17,224,235,10,131,1,70,163,17,137,137,137,32,162,174,13,96,52,26,101,211,135,196,104,26,10,
122,77,194,33,6,184,244,18,106,168,68,104,97,184,235,75,241,142,41,17,142,36,164,172,20,67,45,220,239,225,64,140,27,198,24,31,131,186,230,119,223,65,139,192,152,38,224,43,218,73,56,12,188,22,253,27,52,
81,183,98,51,168,10,1,49,62,237,59,18,168,73,245,134,113,93,135,11,196,130,67,147,107,77,110,24,12,113,213,206,67,234,168,68,175,87,164,80,195,95,126,189,8,211,213,87,123,50,153,41,236,121,248,251,239,
191,113,229,149,87,130,162,40,97,117,227,243,14,124,97,44,210,124,157,24,111,152,160,144,196,16,16,22,248,179,127,27,96,216,208,68,61,79,66,112,234,178,130,107,4,137,104,75,18,245,6,94,209,72,194,33,36,
68,39,86,55,205,148,3,156,228,130,169,88,5,49,158,99,91,88,175,152,47,140,5,25,104,32,48,22,203,224,37,6,186,180,218,95,137,237,23,228,110,223,124,218,174,208,223,66,121,195,248,128,153,127,155,247,109,
235,241,1,98,74,192,23,223,53,94,9,0,18,19,19,85,9,251,242,222,227,170,171,174,18,95,87,33,32,72,200,88,195,123,66,66,160,55,44,18,132,249,62,119,104,8,243,245,134,197,200,100,35,129,175,248,5,48,173,
102,63,140,198,250,41,62,208,101,136,227,150,33,55,144,73,9,83,212,18,136,73,50,230,56,6,43,97,32,230,11,99,129,11,251,107,227,55,8,140,169,3,95,209,156,200,10,5,94,122,235,179,164,38,230,225,219,190,
165,180,109,165,66,18,249,65,24,226,6,196,162,9,95,145,82,115,43,245,190,178,44,205,16,8,98,90,128,176,192,243,184,33,44,112,34,162,70,247,255,102,244,169,207,4,190,226,23,192,180,182,112,60,26,155,143,
42,61,91,28,13,227,71,173,4,28,114,172,243,146,2,84,82,64,76,238,176,165,72,207,16,214,152,11,51,88,202,1,98,126,105,237,33,32,61,22,129,47,209,19,56,106,131,151,214,214,164,70,211,91,206,183,125,43,29,
146,40,229,184,231,155,250,27,160,254,245,96,240,107,231,108,27,143,33,16,19,10,95,114,130,151,6,94,158,171,81,137,2,177,104,65,24,175,162,17,206,229,3,97,12,195,192,144,160,125,189,151,2,95,178,100,229,
36,162,45,0,139,37,240,18,51,243,44,87,230,66,173,205,48,43,189,166,75,78,128,146,2,71,82,238,175,85,111,152,90,32,230,7,99,4,196,36,181,239,104,76,100,73,241,118,69,99,159,65,181,39,104,228,10,37,86,
10,194,184,238,201,237,33,11,94,23,26,206,27,198,238,159,20,171,109,91,73,248,210,186,235,65,228,182,35,124,189,97,146,32,76,2,252,137,91,19,198,1,97,6,232,124,109,24,129,47,2,96,34,13,133,87,222,152,
137,220,38,77,121,149,63,247,159,179,40,45,254,14,103,78,127,131,147,39,142,227,183,75,151,120,157,55,229,141,25,200,205,107,42,234,165,135,61,209,31,85,63,255,132,122,245,146,240,214,236,249,168,151,
100,241,251,253,151,106,7,158,27,49,56,226,179,24,12,6,140,30,55,17,45,111,109,227,119,220,237,118,99,226,11,207,161,220,118,14,0,48,241,149,105,200,201,109,34,234,89,135,15,121,20,85,85,63,9,190,206,
143,255,253,1,223,158,57,133,239,75,75,112,252,171,35,248,237,183,75,138,26,69,74,129,152,212,53,98,122,246,134,69,26,184,66,13,146,220,215,12,13,98,126,227,57,1,49,69,225,75,105,240,210,195,26,85,185,
188,229,74,181,111,177,144,37,22,194,132,92,39,60,132,213,26,162,190,191,249,181,237,88,105,215,34,224,43,38,192,75,70,16,147,10,97,66,79,148,35,69,189,40,8,211,248,120,22,60,113,70,224,43,94,36,65,174,
193,90,108,188,114,163,127,101,160,115,215,30,24,61,110,34,22,190,247,1,30,30,48,8,215,94,91,79,113,133,55,24,12,248,245,215,95,240,225,251,203,131,126,175,151,100,193,125,125,251,115,158,227,251,175,
69,203,86,65,240,5,0,91,54,174,103,225,43,90,242,207,255,187,1,157,58,119,195,83,207,140,198,220,69,43,48,96,224,16,252,35,76,189,250,190,151,82,245,29,141,107,136,49,120,229,246,98,240,49,138,195,26,
218,16,190,198,33,244,53,13,97,203,177,251,140,121,255,131,4,161,71,254,62,60,116,70,168,254,134,43,31,216,15,9,41,175,183,186,85,19,104,229,132,109,161,97,236,66,142,7,71,47,25,66,182,241,160,205,127,
99,160,77,27,84,134,47,131,193,192,238,195,166,105,16,19,218,22,96,16,91,37,145,219,64,168,186,4,207,80,187,64,40,65,104,72,241,127,14,67,192,228,148,182,193,154,119,127,68,224,43,126,0,76,168,17,42,215,
192,110,50,155,209,231,129,135,48,227,157,69,104,28,194,211,83,123,63,105,131,182,247,223,129,207,246,226,100,225,241,160,50,221,122,220,139,180,244,127,133,124,191,58,117,234,160,95,255,129,65,199,127,
252,239,15,216,252,201,58,77,125,100,147,201,140,174,61,238,197,180,153,243,112,83,227,60,85,191,169,18,32,166,198,125,213,134,176,72,131,132,161,230,127,66,199,222,208,16,22,62,92,205,127,76,35,201,102,
164,124,87,57,39,178,248,130,151,150,161,75,238,190,33,90,107,220,228,126,94,213,32,76,199,109,90,17,248,10,211,129,122,215,223,233,168,130,52,9,16,178,220,11,124,215,229,234,11,194,34,125,62,2,95,113,
4,96,209,0,175,64,73,74,178,224,213,169,179,208,225,206,187,85,185,223,123,139,230,194,237,118,7,29,127,226,201,225,33,207,233,210,189,55,254,249,127,55,4,29,95,186,104,30,46,95,190,172,201,143,93,47,
201,130,151,95,123,11,237,58,220,37,24,84,181,212,105,107,205,80,83,11,194,34,15,66,66,239,203,211,27,22,137,242,8,124,201,162,27,145,192,43,33,33,33,44,120,233,13,186,180,222,182,165,150,17,243,155,188,
222,64,30,16,166,35,93,81,12,190,66,182,55,15,124,133,155,252,210,59,132,73,245,130,137,170,91,145,94,48,97,63,107,31,194,248,132,30,18,248,138,109,73,20,3,56,98,6,137,115,255,57,139,229,239,45,0,0,92,
123,109,61,252,243,255,110,64,106,218,191,80,175,94,61,100,253,187,49,231,57,67,158,26,137,170,159,127,194,153,211,223,240,190,199,202,101,139,34,62,235,175,191,84,251,253,110,175,250,25,31,190,191,28,
143,13,26,234,87,46,51,251,38,180,235,112,23,14,238,223,235,119,252,31,215,214,67,159,7,30,14,186,238,254,125,123,240,237,153,83,65,247,230,170,173,178,115,223,99,245,242,197,17,223,233,215,95,171,195,
254,94,118,238,123,172,90,230,185,206,63,234,213,67,195,134,255,135,212,180,116,92,123,109,61,100,102,223,196,121,206,160,39,135,163,234,231,159,252,158,149,207,55,151,115,157,152,212,235,73,73,208,17,
173,44,106,188,203,69,202,6,37,114,93,24,128,16,107,195,34,237,63,132,154,115,33,122,205,129,222,65,96,243,206,3,100,180,32,162,154,244,232,220,46,98,155,231,106,235,124,214,132,5,157,107,208,254,218,
48,181,225,203,251,147,110,13,96,33,217,8,249,38,200,80,216,54,224,243,200,252,215,131,5,63,31,91,86,3,250,206,107,114,7,194,183,93,32,162,83,0,83,10,188,188,114,233,146,11,103,138,184,65,234,230,102,
45,48,106,204,139,48,153,205,126,199,141,70,35,158,29,51,1,79,61,209,159,151,87,233,183,75,151,240,237,233,83,162,214,99,236,220,182,9,173,219,180,13,130,150,7,251,63,22,148,196,226,209,129,67,96,52,26,
253,202,253,82,237,192,234,229,139,121,215,209,111,151,46,225,219,51,69,130,234,221,192,89,175,151,66,130,84,147,166,249,120,102,212,11,48,153,130,235,245,153,81,227,48,98,232,99,130,188,117,114,101,55,
140,54,132,41,217,169,106,25,194,66,223,155,27,194,2,191,117,144,209,22,7,32,230,43,87,25,141,170,108,194,74,36,126,37,33,33,1,117,235,214,229,221,230,227,9,194,212,128,47,239,90,175,160,243,245,104,244,
202,248,77,213,72,200,33,199,141,185,146,114,104,21,194,68,169,23,129,175,248,4,48,65,240,37,80,71,190,62,113,28,207,12,27,136,87,166,206,196,63,255,47,213,239,183,164,36,11,58,221,211,29,219,183,124,
18,249,22,6,105,161,29,239,45,154,135,87,223,156,237,7,87,158,132,28,15,99,245,138,37,0,128,27,51,178,208,166,109,135,160,115,87,45,95,28,148,105,80,149,134,27,230,183,111,78,22,98,212,136,33,152,244,
202,180,160,112,201,122,73,22,116,188,187,43,118,110,219,20,85,240,137,6,132,69,59,59,162,118,33,12,224,227,13,19,178,119,75,172,200,21,6,3,140,70,35,18,19,19,65,132,136,146,147,56,70,163,81,96,159,35,
172,9,198,18,132,9,134,47,49,94,175,56,48,122,181,226,5,19,243,172,194,245,95,91,16,198,185,198,147,132,30,198,133,36,240,81,14,49,251,106,9,85,192,255,253,239,87,124,176,106,41,231,239,237,239,184,203,
175,44,223,89,104,161,107,34,202,109,231,176,101,227,250,160,227,93,123,220,139,212,180,70,0,60,225,123,129,114,242,196,113,28,61,252,121,84,27,112,168,247,252,223,175,191,96,45,71,166,71,0,184,167,91,
47,89,59,141,104,93,75,173,236,136,209,168,27,105,245,34,230,25,35,239,135,23,180,116,132,204,202,17,33,18,125,184,8,219,190,67,39,221,209,99,91,22,149,170,91,40,124,105,100,141,156,164,71,144,117,156,
150,247,68,185,214,130,133,215,125,254,91,116,104,190,141,147,113,54,190,0,76,141,52,244,190,231,28,251,242,8,78,20,28,11,42,211,232,95,25,168,87,47,201,111,17,58,12,202,60,203,150,141,31,225,199,255,
254,16,244,219,160,39,135,227,158,174,61,145,222,232,70,191,227,110,183,27,203,22,205,211,208,192,20,252,238,199,143,29,229,204,244,120,253,245,13,80,191,254,245,49,1,97,122,185,167,224,251,75,72,202,
33,190,207,230,183,41,121,144,225,70,6,8,34,68,20,239,31,248,26,174,177,10,97,66,234,133,223,120,25,2,190,116,0,156,209,128,123,45,66,10,159,132,28,156,229,245,96,3,16,248,138,63,0,83,195,235,197,117,
222,145,67,7,56,203,55,205,111,201,3,174,12,162,247,125,242,158,119,249,242,101,44,156,55,43,168,92,102,214,191,57,211,206,127,242,209,26,84,85,253,172,201,65,202,183,46,66,121,232,110,202,201,83,167,
3,137,1,152,82,162,77,168,1,97,114,213,25,159,116,216,177,56,80,24,200,224,71,36,46,128,47,134,219,113,200,173,26,180,15,95,146,31,75,165,247,145,111,123,7,5,33,93,227,91,50,112,142,229,100,252,137,89,
73,84,27,190,34,149,255,249,167,159,56,143,95,119,253,245,17,207,205,201,109,130,117,27,119,133,45,51,255,157,153,248,252,192,167,97,203,124,127,182,4,123,118,109,67,167,206,221,252,142,7,38,222,40,59,
247,61,54,111,252,72,84,197,55,206,109,130,181,159,236,12,91,102,193,220,153,56,184,255,83,201,29,17,195,48,168,250,153,187,94,235,95,119,189,244,78,67,182,44,72,218,74,170,17,75,182,135,176,245,96,158,
161,0,96,120,149,143,229,117,97,4,190,136,68,91,255,164,172,5,11,151,144,67,143,125,174,40,239,87,24,248,210,219,122,175,104,119,173,81,187,191,228,27,115,39,162,209,227,216,162,249,77,193,137,240,255,
150,98,149,66,169,205,109,127,253,245,23,110,82,76,172,163,234,236,202,234,229,139,241,75,181,35,108,153,133,243,102,235,102,208,10,149,206,254,31,215,94,27,247,6,142,82,229,181,228,5,147,107,61,88,168,
242,177,232,9,35,240,69,68,179,186,169,80,184,152,150,189,96,209,130,47,111,116,181,222,247,216,139,58,57,202,172,199,194,194,16,35,60,150,22,214,250,241,240,126,17,253,139,97,0,83,114,31,48,128,223,166,
150,246,42,110,79,205,181,245,234,5,220,95,89,69,188,124,249,114,88,239,211,129,125,123,80,110,59,167,155,15,109,15,17,38,89,167,14,165,41,99,85,222,141,72,137,200,49,44,40,62,219,65,132,8,17,37,237,220,
176,109,58,236,134,176,122,110,195,17,194,14,249,194,151,87,174,188,242,74,213,199,24,174,132,99,226,204,47,158,182,93,12,101,223,19,252,173,116,176,246,145,115,226,128,136,110,37,81,29,248,226,163,88,
158,235,94,115,141,137,179,204,175,191,252,162,106,231,87,191,254,117,97,179,4,54,105,154,143,127,92,91,15,255,11,225,177,211,154,92,29,162,94,137,196,18,42,133,79,37,44,46,20,145,127,121,206,235,235,
52,28,145,192,60,145,88,209,197,88,8,237,150,35,241,134,208,53,95,129,135,27,166,36,227,183,223,255,192,95,127,253,165,202,59,215,169,83,7,245,235,91,1,252,73,26,129,76,35,100,196,253,240,162,164,207,
252,188,95,4,190,98,22,192,162,97,148,176,27,12,251,92,183,97,192,126,85,94,249,253,247,223,34,94,239,167,159,46,224,139,8,235,187,108,101,252,188,86,67,135,143,10,90,243,229,43,245,146,44,24,48,112,8,
230,206,158,38,234,221,127,254,233,66,196,181,104,124,159,149,143,52,108,248,79,206,227,118,13,38,16,137,198,224,174,133,125,193,120,20,138,86,0,62,68,175,7,211,49,132,17,33,162,109,115,82,202,222,77,
225,215,130,233,126,127,48,142,208,45,41,240,5,0,117,235,94,5,139,213,138,186,117,255,129,43,174,184,66,249,119,96,24,84,87,199,231,248,28,181,113,68,163,186,78,38,4,227,28,192,148,128,47,255,140,52,158,
255,190,46,68,82,136,159,67,36,145,240,131,137,159,127,194,199,235,62,144,252,108,29,59,117,65,227,220,38,17,203,181,185,189,61,10,142,29,21,181,7,88,149,76,207,202,87,66,37,219,184,116,201,69,90,1,49,
202,68,129,168,206,198,48,34,68,226,183,95,136,193,4,71,124,247,75,146,3,190,0,224,74,35,5,224,50,46,57,237,234,25,104,87,16,221,149,50,246,232,86,239,3,108,99,63,239,23,129,177,152,145,4,126,186,32,244,
131,27,34,130,23,23,124,1,192,141,25,89,156,231,125,95,82,172,74,133,252,227,218,122,232,215,255,177,160,227,123,118,109,195,201,19,193,123,105,13,24,56,4,255,184,182,158,230,63,244,191,66,212,235,183,
167,79,145,86,16,103,3,148,88,115,71,82,223,64,6,13,34,68,52,221,198,99,117,45,152,28,240,69,68,103,16,46,80,223,163,249,221,35,141,161,68,39,99,202,144,242,147,68,53,149,41,28,120,1,30,47,77,187,59,58,
5,157,247,211,79,23,80,85,245,147,42,245,58,104,200,211,48,153,204,126,199,126,169,118,96,237,251,43,112,205,53,215,96,198,59,139,253,66,19,165,134,34,170,33,245,235,95,135,118,29,238,10,58,238,114,57,
117,149,72,132,136,190,250,167,120,10,69,108,211,230,102,73,231,31,62,252,53,185,7,185,135,234,230,43,160,207,246,40,196,251,197,215,80,39,134,110,124,32,91,68,157,215,208,56,21,164,191,68,73,99,74,18,
248,42,128,100,248,10,154,113,8,46,59,224,241,39,57,215,93,21,124,117,132,243,122,114,75,171,54,109,209,242,214,54,65,199,87,45,95,140,223,126,187,132,170,170,159,177,117,211,250,224,129,248,246,246,104,
213,166,173,102,63,242,35,3,135,112,214,235,193,207,246,146,22,160,71,178,81,161,195,87,250,28,34,68,136,144,190,71,233,190,76,72,198,67,34,68,180,214,206,72,226,141,56,6,48,165,54,100,14,44,123,245,53,
215,96,216,136,209,184,133,3,126,220,110,55,54,111,92,175,184,193,119,245,213,215,96,192,192,33,65,199,79,158,56,238,183,198,107,243,39,31,225,199,255,254,16,12,143,26,12,69,188,250,234,107,48,244,233,
231,56,161,146,171,94,227,85,200,230,207,188,91,174,116,123,141,24,63,68,136,104,26,90,180,222,92,249,140,255,114,173,251,226,146,155,155,230,179,255,221,160,65,50,110,72,77,99,255,190,49,35,19,38,147,
137,253,239,155,155,230,179,127,223,144,154,134,6,13,146,217,178,20,69,225,166,198,185,126,215,189,49,35,51,186,99,33,15,143,40,69,81,104,218,44,31,141,115,114,85,127,190,103,71,141,69,70,102,86,204,204,
47,68,138,24,35,99,103,28,3,152,18,202,230,85,170,58,117,234,224,166,156,60,180,191,227,46,204,158,187,4,29,238,236,196,121,206,150,141,235,253,82,189,43,53,219,62,96,224,16,212,75,178,4,65,202,234,229,
139,253,142,93,190,124,57,232,24,80,27,138,24,109,169,83,167,14,110,106,156,139,118,29,58,98,230,59,139,208,158,103,189,106,5,98,8,12,169,209,14,73,29,200,109,240,17,33,18,157,41,17,3,209,227,48,29,155,
18,222,131,209,99,198,179,32,245,228,176,17,120,125,234,12,246,183,233,179,230,2,0,110,107,219,30,61,122,245,65,211,102,249,88,186,242,67,220,144,154,134,27,82,211,48,122,236,4,182,236,61,93,186,227,142,
142,119,177,231,221,209,177,19,122,244,234,131,183,231,45,82,128,172,228,27,87,147,44,22,204,156,243,46,134,62,245,12,150,175,94,139,153,115,230,161,65,114,178,42,159,56,35,43,27,102,179,89,249,190,93,
11,27,51,19,239,87,204,27,82,137,114,116,214,124,202,230,228,221,140,143,54,237,22,244,112,135,63,223,143,143,215,189,207,251,62,141,115,155,96,237,39,59,35,94,119,196,147,143,162,202,39,253,122,147,155,
243,57,65,101,235,166,245,168,56,255,99,208,241,111,190,46,196,225,47,14,160,205,237,237,253,142,183,185,189,61,78,125,93,24,118,3,103,169,207,42,246,58,190,114,236,203,195,126,245,26,43,162,22,188,17,
72,228,238,3,120,215,11,73,147,72,132,136,102,16,78,208,58,48,29,180,93,41,155,45,243,145,175,190,58,130,219,219,118,192,230,77,31,227,198,140,76,252,80,110,195,13,169,105,72,74,178,224,135,114,27,92,
46,23,14,125,126,0,135,62,63,0,0,112,57,93,184,189,109,123,172,95,183,6,35,70,142,70,82,146,5,213,213,14,116,238,210,13,51,167,79,69,131,6,201,160,221,110,204,154,62,149,133,177,155,26,231,226,219,51,
69,154,173,227,106,135,3,35,158,26,12,0,120,160,223,67,120,160,239,195,120,103,206,12,191,50,38,147,25,22,139,165,6,218,172,168,182,87,161,188,220,134,166,205,154,131,166,221,56,115,218,255,253,154,54,
107,14,0,40,47,47,67,181,195,193,30,79,77,75,131,197,98,69,101,69,69,216,103,74,78,78,65,114,74,67,216,237,118,148,219,202,216,227,205,242,91,192,225,168,61,70,81,20,146,83,26,194,229,116,34,53,45,29,
21,21,63,250,93,59,57,37,5,41,41,13,225,168,170,130,205,231,58,106,26,245,196,251,21,31,146,40,21,190,56,59,63,25,30,108,255,190,61,88,186,104,174,44,207,20,78,234,212,169,131,65,79,14,15,58,254,227,127,
127,192,230,79,62,10,121,222,170,229,139,209,164,105,126,80,194,142,71,6,14,193,215,39,11,53,187,65,243,225,207,247,99,193,188,89,68,243,99,12,10,181,60,73,68,88,139,8,17,34,114,193,21,95,248,82,202,123,
112,232,243,131,232,217,171,15,202,203,203,112,242,100,33,254,115,182,20,119,118,244,76,224,30,251,234,40,0,224,142,142,157,112,251,237,237,97,50,155,113,67,106,26,118,237,216,10,154,166,177,111,239,110,
220,222,182,61,78,158,44,4,69,25,241,253,217,82,244,236,213,7,55,102,102,249,121,190,188,97,139,220,99,74,52,199,179,224,99,14,135,3,183,220,218,58,232,120,70,102,22,198,188,240,34,202,109,101,40,183,
217,208,181,91,15,156,40,60,14,154,166,65,81,20,44,86,43,134,15,125,2,0,240,210,164,87,144,100,177,162,220,86,134,91,91,79,196,164,23,199,162,180,164,4,25,153,89,152,243,206,2,236,221,179,19,25,153,217,
44,208,5,74,191,135,30,65,227,156,92,148,219,108,72,77,77,195,236,153,211,224,112,56,48,103,238,66,84,86,156,7,0,88,172,86,140,25,245,12,44,22,43,222,121,119,49,108,101,231,112,186,232,20,198,223,253,
50,86,44,95,130,109,91,54,225,161,135,7,32,39,55,15,54,91,25,146,147,83,48,245,181,201,160,105,58,42,227,102,56,49,26,141,184,242,202,43,145,144,144,64,58,5,21,37,33,33,1,20,69,41,11,96,98,59,68,57,32,
201,229,114,98,213,178,197,56,184,127,175,226,240,5,0,253,250,15,196,117,215,55,8,58,190,108,241,60,92,190,124,57,228,121,255,251,245,23,108,88,247,1,30,29,52,52,104,214,103,232,240,231,48,237,181,73,
154,82,28,151,203,137,15,87,47,199,167,123,118,104,22,50,162,1,43,186,3,36,133,158,215,100,50,163,125,135,59,208,56,39,15,31,174,89,13,218,237,198,189,247,61,0,202,104,196,236,25,190,25,62,245,155,57,
77,9,131,143,8,17,34,26,153,233,17,241,187,152,230,253,237,153,34,140,30,51,30,45,111,109,133,207,62,221,131,31,202,109,120,253,205,25,160,105,26,51,167,79,133,201,100,194,136,145,163,209,183,79,119,208,
52,141,81,99,198,179,231,110,217,180,1,227,95,154,140,36,139,5,91,54,109,0,0,156,57,83,132,11,149,21,24,57,252,73,221,84,183,201,108,198,227,79,120,158,183,75,215,30,120,123,206,116,206,114,201,201,41,
120,172,127,95,208,52,13,179,217,4,138,50,226,245,87,60,182,209,214,157,251,96,50,153,81,199,88,7,141,115,155,224,193,251,123,2,0,78,158,40,64,175,123,239,199,91,83,95,67,215,110,61,48,127,222,219,216,
190,109,51,0,96,239,254,67,156,247,105,218,172,57,206,156,62,133,85,43,150,178,67,100,78,110,30,92,206,139,152,54,245,85,48,12,131,183,231,45,68,102,102,54,156,206,255,33,37,165,33,30,238,123,47,92,46,
39,138,78,125,141,62,247,245,197,182,45,155,208,170,245,109,56,112,96,31,54,172,95,167,56,233,26,12,225,183,118,9,55,129,144,155,155,139,164,164,36,2,96,81,0,176,58,117,234,192,118,78,158,109,177,18,229,
86,38,177,6,203,143,255,253,1,59,183,109,194,193,253,123,195,130,143,156,114,99,70,22,186,118,239,29,116,252,192,190,61,188,220,255,59,183,111,70,219,14,29,145,222,232,198,128,206,160,5,218,117,232,200,
43,20,81,105,241,214,235,145,67,7,241,219,111,151,98,22,96,72,88,32,183,180,107,127,7,14,30,248,44,98,185,78,157,239,65,227,156,60,108,223,186,25,149,21,231,241,250,155,51,112,162,224,56,138,138,190,81,
206,96,34,223,140,8,17,105,253,158,10,147,33,122,105,170,106,120,191,0,128,166,105,252,80,110,67,211,166,205,49,239,237,218,104,18,147,201,140,31,202,109,236,12,249,61,93,186,131,162,140,200,200,200,100,
61,99,23,46,84,130,166,105,116,238,210,29,131,30,125,16,0,240,253,217,82,80,20,133,23,94,124,25,159,125,186,7,25,153,89,216,185,125,43,170,171,29,28,227,156,168,193,81,49,93,58,121,162,0,203,222,91,20,
114,12,62,91,90,226,241,32,49,12,170,29,14,56,124,66,11,105,218,13,147,217,140,198,185,185,56,227,51,206,148,150,150,96,216,240,103,1,120,194,7,183,111,219,82,251,91,73,9,231,115,76,155,250,10,6,15,121,
10,235,62,222,130,79,54,172,199,218,53,171,209,170,245,109,200,201,107,130,5,139,151,177,229,44,86,43,156,206,255,161,164,228,59,184,92,78,159,231,168,11,0,152,60,105,2,134,12,125,26,91,119,124,138,21,
203,150,96,195,250,181,209,159,75,224,176,169,169,196,191,224,186,88,21,69,123,75,122,161,80,250,198,117,26,195,125,48,226,53,131,138,4,28,240,45,239,251,147,255,253,24,197,236,204,68,190,84,206,7,186,
188,255,127,112,255,167,248,246,76,81,152,217,37,131,79,3,253,14,103,75,138,67,194,65,168,231,249,252,192,167,248,238,91,113,49,210,151,46,121,238,85,255,186,235,57,215,66,237,221,205,223,75,52,119,246,
52,180,190,173,93,208,241,171,175,190,70,214,103,21,122,157,179,37,197,56,91,90,140,75,151,92,250,48,36,116,98,140,43,245,156,124,175,75,81,20,114,114,243,0,0,149,21,21,112,56,236,200,200,202,198,153,
162,83,172,17,208,32,37,25,103,75,75,208,187,207,3,104,215,254,14,56,157,78,148,150,20,195,229,114,34,201,98,65,90,90,35,48,12,112,162,240,56,123,78,215,110,61,241,249,129,207,224,114,94,132,197,98,69,
171,214,183,225,147,143,63,130,195,110,151,237,14,19,10,143,0,0,32,0,73,68,65,84,29,77,102,51,172,22,43,156,78,39,210,210,211,81,244,205,215,160,105,26,105,105,233,176,88,235,163,232,212,215,126,225,30,
153,89,217,48,155,235,194,86,118,14,14,135,231,57,44,22,43,210,210,27,129,166,221,40,58,245,13,136,16,33,18,5,11,81,79,147,39,10,108,186,60,247,237,153,48,249,36,131,152,250,250,20,22,188,104,154,198,
176,193,143,225,246,182,29,224,168,182,99,204,168,17,126,75,21,102,190,245,6,76,102,51,92,174,218,177,121,228,240,39,113,103,199,187,209,56,39,23,103,207,150,178,112,160,85,113,57,157,44,120,133,135,85,
119,216,191,25,48,40,183,149,225,145,1,143,179,199,210,210,210,113,186,6,200,74,75,139,145,156,146,130,179,165,30,240,74,78,73,225,188,79,181,195,129,105,83,95,133,201,100,198,252,197,203,81,89,113,30,
133,5,199,209,56,39,15,207,142,24,234,55,190,38,39,39,195,229,228,174,95,135,195,142,169,175,79,129,201,108,198,226,247,86,162,162,226,71,28,61,124,72,213,73,4,145,234,76,38,142,116,40,178,133,32,250,
42,206,129,207,246,68,116,175,74,85,70,57,188,75,190,233,229,197,74,197,249,31,241,241,186,15,194,150,145,203,19,38,244,58,74,67,141,86,66,15,197,158,175,151,250,241,149,249,139,150,193,225,176,227,108,
105,9,42,83,42,112,162,224,56,94,125,227,45,220,219,189,115,13,180,100,97,208,144,97,24,59,122,36,110,109,213,6,22,139,21,205,242,91,160,178,226,60,40,35,133,5,139,150,163,176,224,56,204,102,51,6,63,57,
12,195,134,60,142,246,29,238,128,197,106,197,13,105,233,112,56,236,104,144,156,2,163,209,136,156,220,60,184,221,110,84,86,86,200,242,236,89,89,217,24,62,114,20,140,148,17,7,246,239,67,225,241,99,104,223,
225,78,60,250,248,96,28,61,114,8,163,199,142,199,212,215,38,163,232,212,55,152,49,103,30,108,101,231,64,211,52,186,245,232,133,41,147,38,192,98,177,98,197,251,107,113,228,240,23,48,155,235,98,248,72,43,
158,28,244,104,84,7,67,34,68,136,104,129,177,212,241,126,121,229,194,133,74,224,66,37,251,247,15,229,182,160,223,215,127,180,166,22,88,124,96,43,240,92,47,180,237,220,177,53,194,120,34,106,16,146,113,
60,83,166,46,203,109,101,112,56,236,120,105,210,43,56,123,182,20,93,186,246,192,156,89,111,1,0,118,108,219,130,151,94,126,21,22,139,21,25,25,161,211,207,247,123,232,17,56,157,23,65,187,105,152,205,102,
156,46,250,6,14,135,3,79,143,120,22,19,39,191,134,189,187,119,34,39,55,15,159,124,252,81,216,103,25,49,242,57,216,202,202,224,166,105,152,205,117,81,90,92,172,166,18,215,254,167,134,179,31,42,105,54,169,
57,175,163,5,8,76,148,166,47,220,97,135,114,192,23,17,237,25,255,106,94,63,30,235,52,146,52,206,201,131,195,97,199,184,231,159,101,143,37,39,115,207,8,186,92,78,124,126,240,51,52,107,214,2,239,45,158,
207,14,82,123,247,236,194,146,69,158,191,23,44,94,134,156,220,60,108,219,186,25,93,187,247,196,246,173,155,113,162,240,56,24,134,193,144,161,79,179,229,228,24,87,188,85,151,158,214,8,221,186,116,100,103,
32,251,220,223,15,207,63,59,28,14,135,29,69,167,190,198,189,247,245,69,73,241,119,200,205,107,130,197,11,230,161,180,164,118,0,236,222,179,55,182,110,217,132,197,11,230,1,0,22,45,93,137,204,172,108,191,
50,68,136,16,137,59,250,34,117,160,162,49,90,237,112,224,229,137,47,68,44,119,182,180,4,111,207,174,205,140,184,125,219,22,63,15,216,164,23,199,161,218,97,7,77,211,120,97,204,179,184,181,85,27,100,100,
102,99,210,139,99,97,179,121,128,246,68,97,1,158,24,216,31,119,117,186,7,27,55,124,132,213,43,150,194,233,227,189,242,62,119,97,193,49,100,102,101,131,50,82,24,54,120,32,27,234,56,114,248,80,180,107,127,
7,114,114,243,80,90,82,12,167,243,34,0,6,179,166,191,201,94,163,164,164,24,179,102,120,50,80,30,216,255,25,50,179,178,1,0,67,158,24,192,70,94,68,77,149,245,170,219,209,88,207,175,67,175,90,162,88,64,34,
144,69,224,75,11,215,210,170,247,75,137,103,201,207,111,206,134,98,132,238,132,66,203,173,173,218,96,213,138,165,236,223,39,10,11,144,159,223,2,167,107,194,23,249,118,115,82,228,200,225,47,88,248,162,
40,10,249,45,90,226,141,183,102,178,191,59,236,158,1,249,249,103,135,99,200,176,225,72,73,105,136,153,111,77,69,97,193,49,228,55,111,137,21,203,150,176,101,11,11,142,163,117,155,219,9,128,17,33,253,185,
206,250,60,121,141,86,67,104,59,53,70,236,145,104,123,191,124,133,166,233,160,20,242,92,186,229,114,57,61,227,85,205,111,23,2,34,41,206,156,46,98,141,102,154,166,241,249,193,253,248,252,224,126,78,224,
91,247,225,251,97,223,231,108,105,9,59,54,250,22,115,185,156,216,182,117,83,208,24,86,90,90,59,102,184,106,66,244,1,224,116,209,55,81,11,109,215,138,237,236,221,32,156,166,233,160,92,8,190,117,27,184,
145,56,0,124,127,182,196,207,219,11,0,55,55,203,199,247,165,193,199,189,50,249,149,169,120,103,206,12,206,53,143,98,196,3,242,89,88,189,114,153,190,0,76,88,135,39,44,233,6,1,51,2,95,122,134,47,161,231,
40,241,29,78,23,125,131,190,15,246,247,59,230,116,58,253,54,166,244,141,145,167,221,52,40,99,109,202,84,91,217,57,100,102,101,163,176,192,179,246,43,53,53,13,159,108,88,175,170,126,186,125,214,120,209,
52,13,91,217,57,76,158,56,129,77,21,236,149,162,83,223,224,249,103,135,35,45,45,29,43,222,95,135,110,247,220,9,187,189,10,89,217,217,40,44,56,6,192,179,70,108,165,15,144,69,83,14,31,254,154,220,131,220,
131,136,218,6,106,96,244,77,12,110,92,171,52,55,243,241,28,40,241,12,66,146,48,196,149,158,71,89,135,147,146,44,152,62,107,46,190,61,83,4,147,201,140,234,106,7,102,190,245,134,39,116,54,160,220,140,217,
115,253,32,237,157,57,51,241,253,217,82,152,76,38,140,159,56,5,22,139,5,180,155,70,146,197,130,51,167,139,48,245,181,201,65,223,189,113,78,158,159,157,194,203,150,10,163,36,73,22,43,82,83,211,34,38,224,
208,37,128,133,131,47,2,89,177,11,94,177,2,95,90,251,22,188,203,50,12,78,23,157,194,83,195,173,120,118,212,88,148,151,219,80,110,59,135,19,133,5,40,45,41,193,176,225,35,81,93,237,64,219,118,29,216,83,
10,11,143,225,185,209,227,208,174,253,29,40,42,250,6,219,183,109,198,107,111,76,7,85,135,66,146,197,130,212,244,70,108,34,142,104,201,134,245,235,48,115,206,60,108,88,191,14,78,231,69,56,157,23,81,90,
92,140,71,31,31,140,19,5,199,144,210,176,33,42,42,206,195,229,116,226,195,15,86,225,141,183,102,129,162,140,236,102,153,36,17,7,145,184,237,243,101,9,185,209,183,181,27,235,54,135,232,97,142,100,152,213,
155,34,107,234,113,170,171,29,236,182,8,247,63,240,16,238,235,251,32,230,206,153,197,89,238,153,167,159,12,210,189,119,230,47,193,230,141,27,176,121,227,199,130,85,182,113,142,199,171,230,245,178,102,
100,102,161,178,162,130,77,76,147,100,177,192,124,141,9,229,229,54,152,76,102,207,239,149,21,168,172,60,175,187,207,158,32,76,71,12,4,178,8,124,233,226,121,229,188,183,86,96,143,166,105,60,49,176,63,190,
60,122,24,128,39,11,34,0,60,55,98,40,190,47,45,129,211,121,17,99,71,143,196,236,153,158,125,187,170,29,14,140,28,241,36,50,50,179,97,164,140,56,91,90,130,145,195,135,194,233,114,162,188,220,134,97,131,
7,178,215,158,61,99,26,74,75,138,217,119,125,242,137,199,100,55,34,74,74,138,131,60,86,27,214,175,197,212,215,38,195,98,181,214,100,60,44,131,211,121,17,7,247,239,67,90,122,35,84,156,63,143,33,131,6,0,
0,74,75,138,241,252,200,167,107,178,58,150,176,199,137,16,33,18,135,182,55,59,17,76,190,163,216,19,229,240,126,133,29,31,229,82,70,190,169,244,25,125,182,157,176,97,180,81,22,71,181,29,169,169,233,188,
203,223,152,145,9,151,211,201,9,95,145,244,237,153,103,159,71,255,71,6,226,206,142,119,99,249,234,181,160,40,10,119,118,188,27,189,251,220,199,150,25,249,220,24,164,166,165,35,201,98,193,226,229,171,209,
182,93,7,140,28,53,6,195,158,30,169,232,119,87,194,14,76,148,162,48,225,254,38,18,27,224,165,69,248,138,149,208,195,192,178,13,174,179,224,194,207,145,227,160,189,0,230,11,102,123,247,236,98,59,55,223,
117,98,190,177,241,0,80,89,89,193,153,9,170,52,96,109,153,18,158,49,151,211,201,153,254,183,232,84,112,220,125,97,193,49,54,212,208,87,108,182,50,216,108,101,146,158,163,190,181,30,233,60,136,144,62,62,
214,120,44,134,194,15,201,103,151,7,30,137,136,19,147,201,140,129,131,134,0,0,58,119,233,142,185,115,102,114,150,75,74,178,224,157,119,107,183,35,120,230,169,33,104,156,147,135,242,242,218,49,250,158,
174,221,209,32,57,5,46,167,19,31,133,201,24,222,32,57,25,109,219,117,192,125,189,187,2,0,134,62,53,2,119,222,117,55,54,109,252,24,179,222,126,23,171,87,46,135,201,100,70,78,78,46,94,159,50,17,29,59,117,
198,166,79,214,227,163,15,63,64,29,170,14,214,174,223,140,165,75,22,232,170,158,121,3,152,152,117,95,4,206,244,63,40,199,51,124,197,218,104,44,253,50,100,192,35,66,132,72,244,198,61,95,91,66,170,93,193,
48,218,244,158,73,234,167,117,226,253,34,235,191,244,97,31,159,60,81,136,101,239,45,14,249,59,87,8,98,181,195,142,166,125,31,100,255,62,91,90,130,202,202,10,76,120,105,74,72,0,99,24,32,53,53,29,148,145,
194,220,249,181,81,50,46,151,19,23,42,43,80,237,176,163,113,78,46,50,50,179,240,249,129,253,160,105,26,183,220,218,26,169,105,233,236,178,139,202,202,10,191,189,249,228,177,49,149,85,200,68,190,10,18,
13,37,9,236,112,201,0,196,232,246,94,209,132,47,181,238,39,199,218,175,32,47,24,223,208,11,68,183,126,245,2,184,196,251,69,36,22,250,123,161,6,172,180,182,173,73,171,213,247,255,194,150,209,215,55,215,
14,124,17,155,41,126,197,229,114,98,249,210,197,194,244,177,166,240,153,211,69,104,144,156,130,27,82,211,240,67,185,13,223,159,45,229,117,122,181,195,14,218,77,99,196,83,131,131,190,223,166,79,62,70,199,
187,238,70,70,102,22,166,79,125,205,115,159,51,69,248,161,220,134,165,75,22,6,233,51,19,161,109,104,69,255,19,229,0,30,37,33,137,64,152,246,193,67,235,240,165,245,208,67,46,8,3,192,43,28,145,8,1,47,34,
68,68,246,140,177,62,112,234,10,194,212,130,47,185,46,41,58,26,69,168,222,201,250,110,12,255,119,80,201,238,50,8,220,128,89,75,106,237,27,170,8,0,59,183,111,193,133,202,74,76,125,109,50,102,204,158,135,
157,59,182,226,219,211,69,184,41,39,55,226,181,206,158,45,197,217,210,18,76,157,54,11,159,126,186,27,38,147,9,229,54,27,78,158,40,192,231,7,247,99,216,211,207,192,225,112,160,188,102,211,243,79,247,236,
194,146,101,171,81,135,162,112,242,68,129,110,82,207,135,4,176,80,202,33,167,219,159,64,88,252,128,23,129,47,105,114,125,253,36,217,238,31,169,140,82,191,135,157,197,39,179,147,68,136,200,102,192,102,
100,102,161,107,183,158,152,61,243,45,217,250,203,180,180,116,156,63,127,30,180,207,246,17,96,24,88,44,86,140,30,59,30,19,198,141,142,74,29,200,101,15,104,193,152,149,220,13,10,29,175,212,72,59,47,106,
220,149,246,14,122,30,78,180,100,222,86,87,59,48,101,210,132,136,245,89,93,237,240,105,255,158,194,222,53,222,251,246,238,198,153,211,167,208,180,89,115,220,148,147,11,151,243,34,38,135,216,188,251,229,
137,47,160,186,102,227,236,241,227,70,225,206,187,238,70,106,106,26,202,109,101,236,90,50,154,166,241,194,216,81,112,94,188,88,123,127,135,3,131,31,127,4,29,59,117,70,227,156,60,118,109,252,151,71,15,
225,116,145,62,178,35,39,170,211,193,48,50,196,107,199,7,132,69,203,40,213,162,215,75,15,240,165,135,122,214,219,59,105,169,45,146,16,104,34,122,233,143,77,166,186,200,204,202,150,181,125,118,235,209,
11,91,55,111,130,205,86,134,86,109,110,67,209,169,111,224,186,120,17,148,209,136,220,188,38,186,134,47,45,64,152,218,93,167,92,161,87,250,244,126,197,82,255,160,156,206,114,109,180,29,170,220,215,39,11,
67,126,163,11,149,149,216,185,125,107,196,239,30,120,175,125,123,119,115,234,152,239,166,222,190,16,182,238,195,247,253,142,57,236,14,56,236,14,93,168,88,98,56,163,67,107,198,71,172,26,68,122,75,203,174,
198,53,163,177,79,152,210,231,40,85,86,233,186,214,170,142,104,73,218,180,185,89,210,249,124,54,245,37,247,136,191,123,132,134,45,51,50,179,178,225,116,94,244,203,114,234,43,22,139,5,169,105,141,96,183,
87,177,51,211,14,135,29,0,144,154,150,14,139,197,2,151,211,137,210,210,98,246,154,22,139,5,118,135,29,89,89,217,40,58,245,13,246,236,222,9,187,195,238,241,120,141,25,143,197,11,223,133,237,220,127,224,
172,185,158,197,98,245,108,23,81,113,158,221,76,221,98,177,130,50,26,97,54,155,107,182,150,56,7,167,243,34,114,243,110,246,236,243,87,82,28,87,6,173,98,70,159,2,158,47,62,151,101,34,23,208,4,136,234,97,
140,209,158,141,173,46,244,43,177,102,53,210,77,194,63,143,186,155,146,39,234,79,65,24,77,62,151,222,58,7,45,27,213,4,190,24,217,202,69,211,251,21,43,144,69,188,96,68,52,51,110,212,24,15,227,38,76,68,
185,205,134,228,228,20,0,192,171,83,94,10,130,175,165,43,215,224,232,225,67,72,78,73,129,217,92,23,159,124,252,17,182,109,221,132,86,173,111,195,11,47,78,194,158,93,59,144,223,188,37,142,30,57,132,37,
139,222,69,102,86,22,6,63,57,28,0,112,162,240,24,74,74,138,49,234,249,241,88,188,112,30,82,82,26,194,106,177,34,39,55,15,70,138,66,193,241,99,48,155,235,98,252,196,201,40,45,41,198,240,214,183,97,239,
238,29,88,243,254,42,180,190,237,118,60,248,240,0,22,188,242,91,220,130,210,226,239,224,116,94,68,74,202,63,97,183,87,97,202,164,9,113,9,97,178,117,137,81,130,47,41,23,144,205,251,38,58,252,144,81,231,
219,232,84,95,69,189,127,164,201,91,48,138,62,160,178,201,52,24,197,109,153,68,33,70,135,20,67,68,110,239,149,111,133,232,193,64,210,138,49,170,212,115,240,189,110,229,79,85,138,117,130,162,223,77,12,
124,129,81,230,22,50,103,61,148,190,144,90,188,119,44,236,165,85,110,15,98,147,112,144,48,68,34,90,148,137,19,198,178,255,189,113,235,238,160,230,212,170,245,109,56,184,127,31,102,205,240,108,202,190,
108,229,26,246,183,126,15,245,199,148,73,19,112,162,240,56,40,138,194,250,141,219,177,114,185,39,245,115,110,94,19,220,221,241,246,160,253,250,182,110,217,132,33,67,135,227,195,15,86,161,242,252,121,36,
167,52,132,185,110,93,44,94,48,15,165,37,197,216,179,115,59,94,126,117,42,214,188,191,10,0,96,54,155,217,245,33,203,87,175,133,205,86,134,197,11,230,193,100,54,99,219,206,125,234,1,152,64,43,213,91,143,
114,55,121,121,243,72,196,90,196,76,116,222,83,11,147,224,90,217,74,65,9,248,18,123,122,60,173,13,79,212,155,194,233,5,198,180,166,68,177,12,94,241,6,95,58,24,167,53,247,96,85,246,95,68,129,24,129,47,
34,154,105,54,53,253,78,146,197,130,167,135,63,7,0,72,78,73,65,74,74,195,160,178,190,139,210,1,248,109,106,158,223,188,37,70,14,247,236,221,67,211,52,42,43,206,179,107,199,74,75,190,227,220,44,157,107,
62,198,121,177,54,156,208,77,211,176,90,173,236,111,190,27,171,211,180,27,69,167,60,225,150,46,167,19,230,186,117,69,215,129,90,237,81,14,16,83,164,155,83,112,188,82,50,244,80,109,239,23,35,221,160,208,
206,88,25,65,9,165,64,152,232,215,228,19,125,3,70,190,251,242,244,126,5,21,99,180,237,245,76,136,53,216,96,24,198,239,159,154,247,82,235,190,98,158,145,192,151,198,225,75,230,103,224,115,223,168,121,191,
162,40,94,16,35,66,68,143,240,5,0,253,30,124,4,229,229,101,120,117,202,75,120,105,194,24,206,242,103,207,150,32,35,51,139,253,59,45,189,17,251,223,14,135,29,153,153,181,201,58,146,83,26,162,164,248,59,
15,72,185,105,222,109,216,233,188,24,242,55,183,219,237,247,55,237,115,93,169,34,24,194,36,238,129,22,248,143,79,153,88,131,47,165,65,87,249,7,210,222,128,196,165,199,98,65,85,136,206,73,214,81,5,149,
69,113,155,93,225,144,72,161,146,24,78,49,148,242,86,169,57,171,28,143,169,174,245,146,245,78,142,203,196,12,124,201,24,122,40,15,124,105,167,222,181,54,72,18,33,18,77,169,174,118,160,227,93,157,81,110,
43,67,219,118,119,192,110,183,7,149,57,122,248,16,22,44,89,142,202,138,10,164,166,165,33,39,55,15,7,247,127,6,0,88,178,240,93,76,122,229,117,108,223,178,9,205,154,183,192,209,195,95,248,167,153,15,33,
5,5,199,240,224,195,3,176,109,203,70,54,9,71,92,194,48,163,143,27,202,9,95,90,245,126,201,53,126,69,107,88,82,34,163,167,22,116,81,105,239,151,18,239,30,13,86,72,20,162,16,114,193,19,89,87,17,63,224,21,
202,251,21,213,198,18,195,240,37,227,215,214,143,129,34,80,170,236,191,144,141,153,137,232,182,95,95,187,102,53,42,43,206,35,53,45,29,75,22,205,199,218,53,171,1,0,165,37,197,152,93,179,230,171,162,226,
60,134,62,241,24,186,245,232,133,19,133,5,0,60,161,128,0,131,109,91,55,193,102,59,135,252,230,45,177,119,247,78,236,217,189,19,0,80,82,82,140,89,51,166,250,221,107,230,244,169,168,168,201,110,56,243,173,
55,208,225,142,142,72,75,111,132,3,159,125,234,183,7,152,195,94,197,254,125,228,208,23,40,250,166,54,195,227,204,183,106,175,1,0,67,30,31,160,136,177,202,128,9,189,145,173,206,54,101,142,7,248,146,195,
251,165,72,248,161,22,117,64,11,186,171,176,157,34,40,81,25,24,201,207,27,254,103,238,4,28,114,171,86,162,124,223,70,24,84,197,74,54,195,88,135,46,185,239,17,235,94,175,104,195,151,166,67,15,201,158,46,
68,136,200,34,7,15,124,198,54,167,202,74,207,255,187,92,78,148,214,164,164,55,153,204,160,140,70,44,89,52,31,201,201,41,120,225,197,73,88,187,166,118,191,156,211,69,167,112,186,232,148,95,123,118,57,157,
236,154,46,239,181,125,83,198,187,156,78,108,221,188,145,253,219,127,157,23,205,254,237,112,216,217,116,247,129,215,0,252,215,163,197,165,33,171,1,240,82,3,190,228,60,77,218,228,163,54,247,30,211,149,
237,43,147,157,162,148,247,75,121,29,82,70,18,229,253,70,194,61,91,196,27,166,77,232,82,226,62,196,235,165,7,248,210,134,238,105,65,72,191,68,68,143,125,61,195,48,48,155,205,24,245,252,56,24,141,70,56,
157,78,44,89,56,31,14,71,21,169,80,2,95,170,49,134,40,131,87,5,239,151,86,194,15,117,51,121,160,2,124,73,245,126,233,49,252,48,34,128,169,5,71,196,27,166,45,5,97,52,152,141,143,192,151,90,240,165,80,232,
33,241,126,17,33,34,91,159,23,169,57,85,86,86,96,228,240,161,162,238,193,121,109,141,37,150,10,215,95,134,12,67,244,125,15,45,218,26,82,146,133,40,0,95,90,9,61,84,213,251,21,5,93,14,180,123,35,234,176,
78,116,82,114,123,150,209,230,11,212,33,190,225,135,74,215,139,31,128,241,1,160,72,80,38,5,218,8,136,69,143,200,149,218,152,89,111,94,47,2,95,218,209,73,34,68,136,200,213,95,50,49,250,190,34,89,74,75,
225,136,82,163,18,52,6,95,209,170,50,49,222,47,173,188,23,47,117,84,83,103,101,222,232,91,174,106,85,194,251,197,87,95,148,184,79,162,82,15,45,117,211,102,33,80,72,128,75,123,224,21,245,107,232,120,189,
151,30,225,139,120,191,136,16,81,167,15,21,190,129,41,195,187,188,86,189,95,66,67,148,120,121,16,116,16,214,37,231,120,165,22,124,105,199,251,165,202,103,208,201,140,131,54,225,75,13,239,151,86,109,111,
63,0,19,212,193,241,0,44,57,51,38,198,34,140,49,49,182,155,189,156,215,213,131,215,139,192,87,108,195,151,80,29,60,124,248,107,197,159,137,220,35,254,238,65,230,50,196,195,9,111,8,171,53,48,116,243,241,
98,10,190,4,94,139,191,247,75,187,141,197,187,63,107,40,155,54,98,70,79,37,244,85,129,73,107,185,214,125,133,186,23,47,27,72,238,240,67,37,0,76,40,72,241,133,48,57,193,73,175,48,166,149,208,45,2,94,81,
0,47,157,195,151,164,231,32,214,34,17,34,170,216,71,177,236,253,226,83,47,92,230,128,224,181,52,74,24,182,114,175,167,86,112,188,138,26,124,49,234,36,15,137,152,124,67,111,227,149,92,222,48,133,108,39,
57,55,122,230,173,247,50,122,191,66,134,31,202,168,39,137,234,232,9,163,192,134,115,220,149,16,45,48,211,234,250,24,173,67,151,44,215,34,240,165,24,124,73,73,4,16,27,70,47,201,210,74,68,251,58,26,11,99,
149,102,218,37,87,253,68,186,151,210,123,112,42,60,86,41,9,95,98,174,43,108,93,155,190,188,95,124,121,138,215,4,130,216,73,3,133,19,190,68,86,39,233,161,135,242,123,191,212,151,68,62,157,155,20,47,88,
96,133,43,109,204,144,68,1,202,214,1,1,175,232,129,151,150,224,75,201,14,158,8,17,34,114,52,39,137,253,138,142,218,176,108,94,48,13,246,103,81,135,47,165,170,142,145,30,70,201,23,190,244,234,253,18,21,
74,171,49,93,148,170,111,188,67,15,37,215,129,250,58,144,40,22,168,196,158,67,50,29,234,19,58,99,1,188,8,124,201,7,95,36,244,144,8,145,232,247,239,114,132,30,234,173,46,4,219,40,53,117,161,153,212,222,
10,27,187,178,133,28,242,188,152,156,235,190,228,50,242,137,253,22,61,125,20,189,238,75,225,112,221,192,203,71,59,252,48,8,192,196,118,124,98,193,141,128,88,252,64,87,92,128,151,66,240,37,235,128,74,224,
139,8,17,2,95,145,154,172,46,214,126,5,70,234,132,143,196,210,212,254,74,81,4,47,77,192,151,192,208,195,120,241,126,113,233,112,180,245,86,174,73,98,49,240,21,203,222,175,176,0,198,185,65,92,4,8,19,3,
84,4,196,180,53,75,162,201,180,244,42,131,151,168,91,202,156,186,149,192,151,118,12,60,18,214,76,68,63,240,37,67,183,165,225,245,204,145,19,127,69,134,48,64,123,222,48,70,201,100,72,49,8,95,90,179,155,
212,208,143,104,232,172,220,118,138,28,250,32,230,185,197,122,191,162,6,96,98,59,65,177,139,99,227,113,239,47,45,117,26,74,121,187,162,185,142,72,207,94,47,2,95,68,136,144,49,64,120,59,141,205,208,195,
80,239,194,61,81,204,35,103,134,6,188,97,242,172,167,81,96,156,143,2,124,9,45,46,54,241,134,86,189,95,190,122,172,248,122,198,40,216,41,114,237,247,165,246,132,145,146,225,135,156,0,230,175,8,226,146,
111,72,245,106,5,126,172,88,6,178,88,219,15,76,150,107,70,1,188,68,221,86,33,175,23,129,47,34,68,136,40,13,95,122,243,126,9,237,154,249,64,24,107,99,104,208,176,85,29,188,162,8,95,74,100,61,100,116,24,
70,27,73,127,149,246,224,42,177,191,28,35,211,254,98,124,251,43,225,222,47,141,108,196,44,36,236,48,90,155,49,235,21,202,180,208,25,104,62,37,189,94,192,75,224,73,209,3,47,2,95,106,233,116,155,54,55,75,
186,31,159,77,125,201,61,226,239,30,66,122,191,88,133,47,185,141,88,53,97,76,238,132,18,162,146,90,232,20,190,248,175,251,18,249,122,81,158,252,150,98,31,203,9,98,74,218,77,114,173,251,82,171,189,169,
149,124,35,8,192,194,117,104,90,218,140,153,207,135,141,6,156,197,219,62,96,4,188,180,14,95,242,164,153,143,119,207,23,217,7,140,136,246,251,114,25,224,75,111,237,49,128,174,228,128,48,190,125,182,215,
232,85,51,91,159,162,224,197,227,6,146,54,221,149,9,190,132,140,117,17,19,111,232,96,172,81,210,139,43,9,114,228,176,83,228,88,7,40,194,126,209,146,247,43,36,128,41,97,160,168,185,198,43,158,23,205,43,
191,167,7,35,231,197,8,120,65,137,61,91,8,124,17,248,34,18,207,240,165,118,95,172,229,246,40,6,194,148,50,94,213,250,44,106,121,189,36,61,163,64,248,146,53,244,80,83,153,15,25,89,116,87,13,189,148,37,
228,80,33,248,146,211,251,21,21,0,243,245,86,241,245,130,5,158,39,228,69,137,113,163,15,216,212,18,116,169,14,94,26,129,47,205,132,28,198,9,124,69,187,99,38,66,68,10,124,197,67,232,33,95,47,152,239,43,
234,197,228,80,28,188,212,130,47,153,50,30,202,26,122,168,179,73,6,185,39,16,148,172,67,177,240,165,196,183,147,226,253,82,35,252,16,0,18,132,84,166,55,110,85,46,67,197,123,61,98,224,136,171,51,165,195,
11,101,189,7,195,200,226,241,18,155,217,80,180,215,75,224,90,47,33,105,91,9,124,17,240,34,66,68,110,29,140,117,248,242,123,63,14,27,69,225,97,40,72,110,185,181,53,250,15,24,40,219,16,41,54,220,80,144,
215,75,131,240,37,199,4,132,158,188,95,126,223,44,32,179,167,150,30,89,13,248,18,164,19,34,18,111,104,117,172,79,136,244,48,220,202,192,200,211,17,68,1,44,244,12,91,106,132,23,202,121,31,70,202,136,18,
0,53,177,2,94,66,59,52,254,33,135,140,228,107,17,248,34,16,70,68,47,186,23,63,235,190,132,142,77,124,195,171,229,170,147,36,139,5,55,220,144,38,25,186,110,185,181,53,186,116,237,30,182,124,151,174,221,
113,203,173,173,197,219,91,60,203,114,141,103,20,69,161,105,179,230,104,156,147,23,116,153,212,180,52,52,206,201,11,170,220,36,139,5,205,242,155,131,162,40,0,192,141,153,153,104,150,223,2,169,105,233,
18,117,52,180,93,74,81,20,50,179,178,253,174,105,50,155,145,223,188,5,76,38,83,208,121,249,205,91,34,57,165,33,0,192,98,177,34,191,121,75,228,55,111,169,193,54,160,110,59,150,117,146,88,174,117,128,34,
67,15,181,232,253,242,3,176,112,251,54,9,133,48,41,32,22,143,64,198,5,90,106,133,22,42,1,93,114,92,79,44,116,105,29,188,180,232,245,34,240,69,192,139,72,28,194,151,206,116,220,111,98,24,76,216,247,168,
151,148,132,166,205,242,145,145,145,201,26,229,25,25,153,72,74,178,248,29,247,246,201,215,92,99,66,211,102,249,104,144,156,204,1,29,249,184,33,213,31,176,76,38,79,249,198,141,115,69,3,87,224,99,91,44,
22,52,72,78,9,123,126,131,228,20,88,44,22,113,224,197,211,235,21,106,76,75,178,88,49,251,157,5,24,246,244,72,172,250,224,35,204,126,103,62,146,107,158,247,145,71,31,199,252,69,75,145,220,192,191,254,6,
60,58,8,179,231,46,132,197,98,5,3,6,207,141,30,135,167,134,63,139,113,227,39,226,171,194,211,152,51,119,33,82,211,210,101,93,247,213,169,115,23,44,95,181,22,117,234,80,236,177,172,172,108,204,157,191,
4,15,245,127,212,239,188,204,172,108,204,93,176,4,221,123,246,6,0,180,190,237,118,76,126,245,13,12,25,246,52,118,127,246,5,118,127,246,5,58,117,238,162,78,251,142,224,248,80,163,217,10,157,51,231,165,
135,42,195,151,208,208,195,104,142,245,146,214,128,169,153,5,49,84,37,233,101,29,25,163,33,183,183,86,175,43,125,17,165,122,39,70,55,201,6,63,240,226,55,81,66,192,139,64,24,17,157,104,164,96,93,141,21,
248,226,213,201,214,216,2,77,155,53,199,160,193,79,226,228,137,66,52,104,144,140,245,235,214,192,233,114,98,230,219,239,226,108,105,9,206,156,46,66,151,174,61,176,236,189,133,216,177,125,43,146,146,44,
152,183,96,9,190,60,122,4,201,201,41,40,47,47,195,194,249,115,97,52,82,88,244,222,42,156,57,125,10,20,101,4,69,81,152,60,105,60,0,96,198,156,119,81,237,112,192,229,116,162,65,114,10,46,84,86,136,170,98,
147,201,132,140,204,44,208,52,29,12,60,73,22,164,166,165,225,108,105,9,92,46,23,167,22,100,100,102,193,100,50,215,148,113,178,0,231,125,30,47,120,158,57,93,36,125,76,99,0,135,195,142,167,135,14,2,0,244,
125,176,63,250,62,248,48,230,204,154,14,0,248,242,200,33,116,237,222,19,239,45,94,192,222,187,89,243,22,56,91,82,236,119,237,137,19,198,160,178,178,2,38,147,25,221,122,244,194,91,51,222,70,255,7,239,11,
170,3,177,155,45,63,248,208,0,108,221,188,17,221,123,246,198,134,245,235,216,227,133,199,143,161,83,231,46,88,188,96,30,123,172,91,143,222,56,114,248,11,191,243,183,110,217,196,150,201,111,222,18,147,
95,125,3,14,187,29,133,5,199,100,31,95,188,182,118,184,223,35,53,95,169,38,176,162,235,13,21,207,128,41,111,183,166,166,247,11,136,16,130,200,240,136,175,22,18,22,160,132,103,39,148,247,72,13,111,18,223,
123,51,26,216,111,66,233,186,151,10,94,82,211,162,170,225,241,18,243,172,209,130,47,18,114,72,132,8,129,175,88,107,215,190,253,90,80,63,92,115,188,89,126,115,84,86,84,96,233,146,133,120,253,213,151,81,
90,90,2,192,19,94,246,242,75,47,96,217,123,139,48,126,236,115,184,167,107,15,0,192,173,173,90,99,211,198,143,241,206,156,25,24,63,110,20,58,222,213,25,20,69,225,166,198,185,56,81,88,128,233,211,222,192,
235,175,190,140,140,204,44,212,171,103,193,141,55,102,130,118,211,24,63,118,20,94,127,245,101,84,86,86,120,130,192,25,97,30,4,138,162,176,98,245,58,220,211,181,7,30,232,251,16,238,239,247,112,45,88,101,
100,98,230,219,239,162,105,179,230,152,242,218,155,184,167,107,247,160,239,124,75,171,214,232,213,251,62,52,203,111,142,149,239,175,69,70,102,22,146,147,83,240,230,91,179,216,250,104,219,174,3,30,240,
185,174,216,49,141,235,125,170,29,246,218,80,66,6,248,116,207,46,116,233,222,147,253,253,214,214,183,225,232,145,67,33,175,236,114,57,241,225,7,171,225,116,58,145,223,188,133,232,201,47,223,178,153,89,
217,112,56,236,248,240,131,85,232,115,95,95,191,230,99,183,87,193,86,118,14,173,218,220,198,214,127,243,22,45,113,244,240,161,144,215,46,44,56,134,21,203,222,67,159,251,251,42,170,203,161,42,90,72,56,
45,95,189,19,90,94,172,205,47,43,124,41,20,122,24,114,223,47,149,36,228,62,96,91,118,29,36,99,30,17,222,210,187,235,29,130,65,38,26,179,54,98,79,86,98,135,120,37,192,75,72,167,77,224,139,95,93,146,108,
173,68,162,173,131,4,190,66,247,203,126,251,31,49,12,86,175,92,134,71,30,125,28,107,62,218,136,175,142,30,198,130,119,223,1,195,0,14,187,157,245,36,157,61,91,138,198,57,158,240,193,91,110,109,141,27,210,
210,113,123,219,14,0,128,202,202,10,164,166,166,225,150,91,91,227,214,86,173,145,145,185,4,0,224,112,56,96,177,88,208,52,191,57,78,158,40,96,111,249,245,137,2,220,220,172,185,224,103,111,156,147,139,179,
165,37,120,227,213,151,1,0,19,38,78,97,127,187,243,174,187,177,108,201,66,124,126,112,63,146,44,22,204,154,243,46,118,108,219,226,119,254,87,71,143,224,171,163,71,0,0,78,167,19,183,182,106,141,213,43,
151,179,0,119,182,180,4,93,186,118,103,143,137,29,215,124,85,198,108,54,227,137,33,195,0,0,93,186,245,192,156,89,111,177,5,28,14,59,78,23,157,194,173,173,218,224,203,163,135,209,187,207,253,152,246,198,
43,184,235,238,123,194,94,215,102,43,131,197,98,229,161,239,145,151,194,220,123,95,95,108,221,178,9,54,91,25,220,180,27,153,89,217,40,45,254,142,253,125,195,250,181,232,222,163,55,142,30,62,132,246,119,
116,196,129,253,251,96,177,90,195,190,191,173,236,28,58,117,190,71,93,61,150,48,6,41,27,162,40,61,201,137,82,240,21,57,244,80,192,239,42,245,147,97,247,1,187,202,104,68,66,66,2,25,1,137,132,148,132,132,
4,212,173,91,151,128,151,162,224,197,15,190,100,1,47,2,95,18,191,19,17,34,4,190,148,158,16,241,77,205,29,104,188,210,110,55,150,46,89,136,213,43,151,97,204,184,23,209,247,193,135,177,119,207,110,88,172,
86,92,115,141,9,46,151,19,153,153,89,56,91,227,25,59,115,166,8,229,229,54,44,123,111,145,223,189,26,164,164,32,201,98,101,1,201,43,38,179,25,15,244,173,245,42,5,174,15,227,15,96,121,56,123,182,132,253,
251,219,211,69,72,170,89,223,213,241,174,187,209,180,89,115,60,208,239,33,0,30,111,81,160,220,210,170,53,58,118,188,27,22,139,21,13,82,82,176,111,207,46,128,97,176,233,147,143,209,165,107,15,172,118,44,
67,106,90,186,31,44,10,29,215,66,169,204,137,194,227,158,80,195,128,2,59,182,110,70,239,62,247,163,180,180,24,20,69,161,146,35,52,51,240,186,105,105,233,248,116,207,78,201,240,101,50,155,209,189,71,111,
100,102,101,163,207,125,125,97,181,212,71,159,251,250,98,234,107,147,217,50,71,15,31,194,132,151,38,195,98,177,162,123,143,222,120,227,181,201,236,250,175,80,146,150,222,8,14,187,93,77,37,15,138,43,140,
246,214,77,114,237,43,167,22,124,9,181,161,162,213,53,134,4,176,43,12,6,24,141,70,36,38,38,130,8,145,80,98,168,209,19,37,161,75,82,3,17,121,162,158,192,75,54,248,34,160,65,0,140,8,129,47,141,214,69,224,
154,153,80,251,35,117,233,214,3,38,179,25,23,42,43,145,154,150,142,143,214,173,1,0,184,221,110,188,242,218,155,248,242,203,35,232,210,181,59,246,125,186,27,12,195,224,211,61,187,177,120,233,42,152,76,
102,124,241,249,126,52,206,201,195,251,171,150,227,171,163,71,48,236,169,103,240,252,184,9,216,183,119,55,110,185,181,53,22,206,159,139,147,39,10,49,102,220,139,24,250,212,8,208,52,141,182,237,58,224,
52,143,53,86,129,82,94,94,134,142,29,239,102,223,231,134,212,84,214,67,119,162,176,0,95,125,121,4,159,238,221,205,121,46,108,59,115,146,0,0,32,0,73,68,65,84,69,81,24,59,238,69,12,30,52,0,213,118,59,6,
13,30,90,11,65,219,54,99,197,7,31,161,188,220,134,77,159,124,44,106,108,11,165,46,78,167,147,19,188,188,242,229,209,195,24,59,97,34,158,24,50,12,27,55,172,143,120,237,156,220,60,36,167,52,68,209,169,83,
146,211,205,119,239,222,27,31,175,95,139,89,211,223,100,235,104,207,190,47,48,243,173,55,252,202,237,222,181,3,143,62,62,24,0,80,89,113,62,108,61,80,20,133,62,247,247,197,138,101,75,20,111,227,126,19,
9,33,148,59,156,55,172,255,128,129,232,213,251,62,220,215,187,171,98,125,80,131,228,20,12,123,250,25,164,214,76,58,156,60,81,128,183,103,207,144,180,173,65,184,62,43,53,45,29,79,13,31,9,147,185,46,140,
20,133,210,210,98,76,155,250,170,136,190,83,64,104,103,205,111,125,238,239,135,9,19,39,35,63,47,91,121,0,243,141,235,36,33,55,68,228,106,184,82,215,118,169,125,178,146,224,37,204,152,87,17,188,8,124,133,
53,248,248,200,225,195,95,43,254,76,228,30,241,115,15,2,95,161,223,157,171,77,250,26,175,103,138,78,33,35,43,27,13,146,147,49,253,205,215,112,246,108,41,26,36,167,160,218,97,199,219,179,167,163,99,167,
206,88,246,222,34,124,126,112,63,0,79,232,220,224,65,143,160,227,93,157,209,180,89,115,124,245,165,39,172,143,166,105,12,25,52,128,61,126,242,68,33,123,191,135,250,222,139,59,239,186,27,0,48,100,208,0,
36,89,172,130,191,237,151,71,14,99,232,83,207,176,240,116,75,171,54,216,87,3,92,155,54,126,140,23,39,189,130,228,148,20,148,219,108,104,144,156,130,143,214,126,192,94,131,166,105,184,105,26,109,219,118,
0,101,164,112,123,187,14,248,162,230,125,104,154,198,87,71,14,99,208,224,161,120,180,255,3,130,199,54,169,19,117,123,119,239,196,189,247,61,128,217,51,167,113,254,222,239,161,254,112,185,92,72,74,178,
32,191,121,11,140,29,253,12,156,206,139,146,224,139,97,128,110,61,122,97,242,164,9,181,117,228,118,99,247,174,237,232,222,243,94,216,202,206,177,199,183,109,222,136,13,91,118,98,252,216,81,156,119,204,
111,222,2,67,134,13,7,224,201,168,184,103,215,14,236,217,181,67,25,251,40,220,24,19,6,194,2,237,244,169,211,102,225,204,153,34,197,251,30,151,211,137,77,159,124,204,122,85,87,172,94,139,166,77,243,57,
189,172,124,244,46,82,159,229,176,219,49,127,222,219,40,183,149,1,0,222,156,49,7,237,218,223,129,131,7,62,11,121,13,161,161,135,92,197,211,210,210,209,173,71,47,84,156,255,81,209,254,44,81,154,145,72,
132,136,188,58,35,249,50,4,188,100,29,76,137,94,147,53,96,68,180,223,207,198,77,182,67,248,236,47,25,34,20,177,188,220,134,242,114,27,231,185,229,229,54,44,93,178,48,232,120,181,195,225,7,56,94,113,58,
157,216,180,241,99,206,107,237,243,241,78,249,102,41,228,219,71,211,52,141,199,250,247,69,199,78,157,65,187,221,24,242,248,0,152,204,102,15,68,158,46,194,248,49,207,161,109,251,14,72,77,75,195,151,53,
107,189,182,111,219,2,218,253,7,192,48,24,245,204,83,104,219,190,3,170,29,14,12,121,252,17,63,8,252,242,203,35,104,144,146,130,106,135,67,54,240,114,56,236,152,56,97,44,231,111,171,87,46,131,221,94,5,
6,12,86,173,88,138,189,123,118,178,89,13,39,78,24,3,187,221,14,134,1,102,207,152,6,147,185,118,201,194,236,153,211,224,118,187,37,195,23,69,81,152,250,218,20,148,150,20,251,189,204,226,5,239,194,108,54,
195,238,176,195,177,172,10,128,103,205,217,99,253,251,162,164,102,109,216,214,205,27,217,107,29,57,244,5,42,206,215,122,197,182,110,222,24,209,75,38,117,108,241,76,246,121,244,152,107,77,35,12,134,136,
125,193,219,115,166,227,66,101,37,122,245,190,79,81,251,205,229,114,214,194,22,195,160,220,86,134,140,172,172,144,0,38,5,190,188,247,243,134,223,50,96,144,146,210,208,79,95,164,134,30,114,221,155,162,
40,188,252,234,84,76,153,56,30,139,150,173,82,31,192,136,16,81,119,64,213,7,116,41,11,94,242,194,23,241,122,105,111,114,129,8,17,37,116,48,158,224,139,171,46,194,173,7,99,97,199,253,7,78,23,157,210,92,
251,167,105,218,47,185,134,239,90,175,242,114,91,109,2,141,154,103,184,224,3,3,23,42,43,240,209,135,31,248,253,237,149,71,30,29,136,247,22,47,144,213,235,69,187,221,56,19,162,14,109,182,115,126,239,224,
93,95,7,0,69,167,106,207,41,245,57,30,190,110,133,237,61,75,211,52,138,138,190,9,122,25,135,195,14,135,195,179,126,203,229,116,250,60,211,55,236,127,251,2,150,111,121,181,116,57,104,123,39,1,16,86,251,
14,21,181,103,135,137,100,147,77,151,25,6,13,146,83,144,147,155,135,183,103,79,87,4,190,188,146,145,153,133,222,125,238,71,179,252,22,88,181,114,41,190,60,122,56,228,53,132,134,30,114,165,157,31,50,108,
56,246,238,222,1,155,173,12,86,107,125,69,191,63,201,176,65,36,170,224,229,109,0,25,153,89,24,240,216,227,226,47,192,33,207,142,26,139,36,139,133,179,99,16,227,241,18,186,206,75,152,215,75,190,212,242,
188,188,94,4,42,136,16,33,240,165,87,232,2,19,176,113,109,248,254,189,218,225,192,203,47,189,160,175,190,79,68,174,240,46,221,122,96,222,194,247,240,229,209,195,97,195,194,4,143,23,97,71,47,113,201,13,
228,128,175,88,82,243,64,29,150,58,102,43,178,29,82,205,115,52,72,78,193,172,119,230,227,173,55,95,71,181,195,33,200,190,18,218,103,217,29,85,216,187,103,23,246,238,217,133,222,247,62,128,102,249,45,228,
135,175,26,201,111,222,18,105,233,141,176,230,253,85,170,124,243,4,53,102,121,136,16,9,28,79,2,213,205,108,54,243,203,38,37,96,80,202,200,202,134,145,50,42,6,94,20,69,33,51,51,139,179,211,235,214,189,
23,186,117,239,197,121,222,107,83,167,195,98,177,194,98,177,224,181,169,211,69,131,87,102,86,118,64,250,94,129,189,60,17,81,70,47,17,34,4,190,162,91,23,236,127,134,217,199,39,108,95,47,101,35,36,141,65,
151,239,251,110,223,182,25,79,15,29,132,213,43,151,201,3,94,122,131,47,157,232,190,191,46,215,214,115,88,8,139,214,251,5,232,164,23,190,222,158,53,29,95,213,120,163,120,181,57,17,125,22,3,6,213,14,71,
77,214,205,249,248,252,224,103,232,125,239,253,242,143,215,53,231,143,30,59,30,41,41,13,177,104,233,74,44,90,186,18,0,176,104,233,74,63,59,75,78,73,36,70,7,17,181,102,117,132,136,201,100,70,102,86,22,
108,182,50,84,219,237,48,153,204,72,78,73,241,11,109,104,156,155,135,242,178,50,184,92,78,164,166,165,195,98,177,224,68,97,129,160,217,24,41,207,109,54,155,209,247,161,71,240,234,228,151,130,218,78,114,
74,74,200,243,114,243,154,128,162,234,176,255,45,116,144,234,214,163,23,30,27,56,24,246,154,212,184,180,219,141,17,79,15,33,224,165,208,32,73,214,128,17,209,18,124,133,109,202,49,222,206,125,67,182,216,
80,45,159,16,173,192,104,45,111,223,31,106,143,165,160,58,83,187,173,203,240,189,132,140,111,252,131,50,24,73,247,12,117,122,120,27,51,62,224,43,80,151,153,8,27,22,7,233,174,183,188,210,186,202,241,92,
161,224,75,52,120,69,128,175,212,180,116,54,1,7,69,81,104,150,223,194,47,161,74,104,29,17,30,122,8,0,15,61,224,191,37,65,225,169,98,60,57,232,81,197,170,152,172,1,35,162,154,225,192,87,146,44,22,204,121,
103,1,142,30,57,132,190,253,30,198,153,211,69,88,187,102,53,230,204,93,136,190,125,122,192,229,114,34,201,98,193,91,51,222,70,239,238,119,163,107,247,158,232,210,173,39,78,22,30,199,179,163,199,97,226,
132,177,53,49,233,226,18,107,152,76,102,152,205,102,80,70,35,44,22,43,42,43,206,163,178,178,2,205,242,91,128,166,221,236,90,2,167,211,137,181,31,172,102,223,215,98,177,34,53,45,221,47,214,220,43,153,153,
217,48,153,77,108,103,194,37,20,69,33,39,183,9,28,246,42,216,66,148,107,223,225,78,60,248,208,0,60,54,160,31,156,23,157,178,141,184,38,179,25,230,154,84,175,22,107,125,84,84,156,71,101,197,121,228,55,
111,9,154,118,251,197,204,3,30,87,61,0,148,148,124,199,190,111,90,90,58,236,14,59,210,211,27,129,162,140,40,44,56,230,95,7,89,217,48,155,235,178,215,142,23,125,38,66,132,192,151,188,117,227,169,31,3,96,
8,54,80,185,150,204,132,219,232,54,98,29,202,97,232,202,252,109,20,75,26,37,33,157,120,164,211,9,124,133,129,48,48,48,48,8,74,44,19,86,119,149,152,56,136,80,143,131,6,15,69,90,90,58,102,191,51,159,61,
118,162,240,56,158,30,58,72,118,248,2,128,118,237,239,192,128,199,6,161,178,162,2,153,89,217,216,186,101,19,150,44,90,16,81,67,121,195,87,148,133,0,24,17,237,24,169,140,167,117,12,24,240,56,182,111,219,
140,117,107,222,7,69,81,88,181,102,61,214,174,89,141,189,187,119,226,174,187,239,193,198,13,31,225,174,187,239,193,222,221,158,76,75,125,31,236,143,167,134,60,14,167,235,34,236,14,59,58,118,234,140,247,
22,207,23,221,231,100,102,101,227,133,9,19,81,90,82,12,135,195,142,118,29,238,68,97,193,113,208,110,55,204,102,51,152,7,129,151,198,143,65,82,146,5,163,198,140,195,208,193,3,97,177,88,177,126,227,86,28,
216,191,15,20,101,68,86,86,54,246,236,246,108,46,217,170,117,27,76,154,242,58,182,109,217,132,225,207,140,10,249,28,211,103,205,69,69,13,152,88,45,86,60,63,106,68,80,153,118,237,239,196,129,253,251,34,
195,151,192,239,145,149,245,111,140,159,56,25,165,197,223,193,110,183,163,195,29,119,162,224,248,49,208,180,27,102,115,93,60,248,48,48,97,220,104,0,192,27,211,102,214,128,212,143,24,63,113,50,38,140,29,
133,210,146,98,60,250,248,96,88,173,245,97,183,87,129,166,221,152,252,234,27,232,126,79,71,207,204,82,255,1,184,247,254,126,56,240,217,167,184,187,115,23,44,94,248,174,95,246,41,34,68,136,136,108,210,
113,58,65,224,151,192,32,192,106,13,5,97,0,248,129,152,6,235,87,209,164,81,18,193,75,60,124,49,210,39,32,98,98,143,187,26,125,229,200,238,201,107,2,193,187,119,85,207,46,252,129,76,68,189,189,254,202,
36,188,254,202,36,222,250,40,5,190,0,96,213,138,165,88,181,98,105,200,226,146,215,125,69,168,3,37,247,0,243,3,48,50,203,75,36,170,86,133,143,254,37,167,164,160,89,243,22,104,219,238,14,0,128,195,225,128,
201,108,198,246,109,155,49,110,252,68,15,128,221,213,25,175,78,153,136,6,201,201,72,77,75,199,180,153,115,216,243,217,84,180,18,250,30,139,213,138,151,250,120,82,238,90,44,86,184,156,23,49,107,198,52,
80,20,133,205,219,247,4,149,111,213,250,54,28,216,191,15,175,188,252,18,0,96,225,146,229,108,135,112,239,125,15,224,197,23,158,199,137,194,227,72,78,78,193,150,29,159,6,117,18,57,185,77,224,116,94,196,
155,175,79,1,0,44,95,245,33,146,83,82,124,178,27,121,158,57,39,175,9,214,124,176,74,17,131,193,106,177,226,190,26,200,178,90,173,112,58,157,152,53,125,42,40,138,194,214,93,251,216,186,72,78,105,136,129,
143,244,3,0,116,239,217,27,221,122,244,198,172,233,83,1,0,118,123,21,166,212,236,197,50,119,254,18,228,230,53,65,209,169,111,112,239,253,253,240,88,255,190,112,57,157,216,182,121,35,94,126,117,170,166,
1,140,244,135,68,8,124,105,205,72,229,200,24,199,160,214,11,198,3,194,2,13,60,193,48,166,3,232,18,172,22,4,190,162,62,190,48,12,3,198,224,175,143,162,194,105,85,168,23,201,224,197,19,190,34,21,143,4,95,
122,152,88,225,92,3,70,214,60,16,137,142,53,225,145,51,167,139,112,230,116,17,59,243,225,149,106,135,3,148,209,136,182,237,59,192,77,187,217,212,183,14,135,29,47,77,24,19,148,141,71,74,155,43,44,56,206,
254,183,155,118,163,180,196,179,246,140,166,105,118,127,19,95,201,201,205,195,137,130,2,182,35,240,13,217,203,111,222,18,83,38,189,8,0,168,172,172,96,189,92,158,76,94,158,50,173,219,220,134,220,220,155,
177,112,137,103,225,167,219,77,35,45,173,17,11,96,222,231,174,60,127,30,230,154,189,98,228,238,80,10,124,66,6,221,110,55,206,150,22,215,190,115,205,222,27,249,45,90,250,189,91,193,241,99,120,244,241,193,
44,128,249,166,122,182,219,171,64,81,70,164,165,165,131,118,187,217,80,69,155,173,12,233,233,141,96,50,155,57,195,53,181,52,72,242,237,11,219,180,185,89,210,189,248,108,234,75,238,17,127,247,96,24,70,
211,6,68,180,219,164,95,8,87,8,8,243,216,52,225,141,73,45,129,152,88,232,210,15,120,197,55,124,69,28,95,194,172,105,244,126,19,181,245,85,182,117,134,17,214,187,137,135,47,62,144,171,61,29,32,27,49,19,
209,4,116,249,202,222,221,59,49,123,238,2,92,99,50,225,100,97,1,50,179,178,177,114,197,123,0,128,79,54,124,132,103,71,143,195,252,185,181,30,175,117,31,190,143,215,222,152,142,237,219,54,3,240,236,247,
17,110,167,116,62,226,116,94,20,212,14,202,109,231,144,154,150,198,14,44,105,233,233,44,180,149,150,20,35,37,165,33,74,75,139,65,81,20,44,22,107,80,103,83,88,112,12,57,185,77,240,76,64,50,141,192,71,40,
41,249,142,13,137,148,187,103,113,94,188,24,177,140,195,110,71,86,118,173,91,62,43,43,27,133,199,191,98,255,166,233,224,77,53,43,42,206,35,37,165,33,40,138,2,77,211,176,88,172,176,59,236,154,134,47,210,
23,18,209,172,238,197,185,78,122,234,165,102,13,152,143,145,234,7,97,1,212,197,23,196,188,162,150,129,43,5,182,68,171,131,12,224,165,22,124,197,186,231,215,47,169,76,8,111,110,184,112,90,37,117,85,246,
117,134,138,194,151,188,161,135,81,5,48,0,48,26,141,184,242,202,43,145,144,64,182,10,35,18,90,18,18,18,64,81,84,232,70,194,83,209,75,75,74,224,112,120,82,231,86,86,86,224,185,17,195,208,182,195,29,104,
156,155,135,47,143,30,98,203,109,223,186,9,229,182,50,156,46,170,245,194,172,93,179,26,165,37,197,104,150,223,2,14,135,29,165,37,199,193,48,192,236,25,211,216,76,129,66,58,68,225,70,55,131,3,251,247,225,
157,119,23,215,36,8,177,34,51,235,223,44,128,109,219,178,9,47,188,248,50,246,236,222,129,156,220,60,214,155,228,15,96,199,49,252,153,81,120,225,197,151,113,112,255,62,228,228,54,193,134,245,235,130,54,
133,252,240,131,213,88,180,116,37,58,116,232,136,221,187,182,131,166,105,148,22,127,135,3,251,247,169,242,189,139,78,125,13,167,211,137,241,47,189,140,114,91,25,238,189,191,31,166,76,28,31,246,28,154,
166,177,230,131,85,120,227,173,89,56,122,248,16,58,117,190,7,159,172,95,75,26,15,17,34,4,190,36,212,135,7,194,188,9,57,252,32,44,132,229,202,99,79,219,176,6,161,24,99,87,14,200,146,69,13,120,158,36,41,
163,29,132,135,28,198,43,124,5,190,123,56,111,110,56,189,149,19,198,20,89,103,40,19,120,197,26,124,249,1,88,224,67,231,230,230,34,41,41,137,0,24,145,136,0,86,167,78,29,252,249,199,47,146,174,227,114,57,
225,114,57,217,6,89,81,121,30,107,215,172,230,52,230,79,20,30,15,58,126,162,240,184,191,87,8,64,169,79,202,122,33,3,70,105,73,49,86,46,175,5,159,149,203,151,250,121,107,94,124,225,121,0,12,28,142,42,204,
154,254,38,11,141,3,7,60,136,78,157,187,160,180,164,24,171,150,191,199,130,233,214,45,27,81,80,240,21,218,119,232,136,37,11,223,197,202,229,239,193,81,3,134,19,94,24,205,94,119,248,83,67,208,161,195,157,
200,201,109,130,210,146,98,56,157,193,30,41,135,195,142,135,238,239,133,252,22,45,145,155,119,51,104,183,59,40,45,171,24,41,41,249,14,142,101,85,181,239,188,108,9,156,62,239,236,77,192,65,211,52,38,79,
28,143,230,53,27,22,62,63,242,105,54,99,99,224,57,43,151,45,129,189,6,32,23,47,152,135,252,230,45,145,223,162,37,22,47,120,55,40,67,162,110,12,96,34,68,162,167,148,113,15,165,193,33,91,190,251,39,241,
135,48,214,96,21,154,143,3,140,190,62,191,128,147,148,3,175,208,240,37,56,219,103,12,121,190,184,39,19,2,116,50,0,194,34,233,44,159,137,3,185,116,88,234,190,163,241,14,95,126,0,22,248,240,150,127,80,192,
95,46,48,127,69,190,136,193,96,96,59,198,192,14,50,220,26,10,161,107,205,200,218,52,237,24,144,236,245,255,2,254,12,163,35,215,95,103,193,79,63,59,20,27,212,196,190,102,184,250,241,194,160,87,106,83,199,
123,206,241,122,223,104,154,70,105,105,177,223,121,159,124,188,142,243,30,149,21,21,248,144,35,121,198,233,162,111,216,119,112,57,157,216,186,101,83,196,151,165,105,26,71,15,31,194,209,195,135,100,251,
158,46,167,211,15,50,3,211,224,251,174,251,114,57,157,30,143,91,128,215,45,240,156,192,191,11,11,142,105,2,188,234,91,235,9,210,17,210,239,16,137,122,95,78,38,4,34,244,219,254,6,108,16,132,133,176,92,
213,218,82,73,117,230,150,17,188,52,1,95,49,168,255,193,19,10,12,59,145,224,183,189,130,192,117,141,74,79,28,72,5,47,181,224,75,15,146,200,167,65,69,50,64,184,50,20,213,198,181,134,54,98,132,220,35,84,
5,199,179,113,164,150,194,49,81,222,195,68,206,190,88,220,187,48,138,92,95,206,20,193,68,244,223,206,136,16,33,186,39,214,120,5,24,230,111,24,12,9,220,16,198,19,196,106,237,10,157,1,151,142,192,139,192,
87,120,8,99,215,54,242,244,228,170,161,171,106,108,224,45,55,124,105,221,251,21,22,192,196,128,146,47,108,5,130,87,164,217,100,161,48,198,167,3,208,59,156,69,107,240,149,251,190,215,213,79,194,79,85,14,
117,58,128,40,64,23,1,47,253,8,95,239,23,17,34,4,188,180,93,31,92,54,5,187,142,198,7,194,0,240,6,177,112,93,175,156,230,132,172,159,87,224,197,148,7,175,208,227,103,188,134,28,10,169,75,134,249,27,64,
2,175,112,90,37,65,76,238,228,46,66,225,139,239,90,66,189,194,151,31,128,133,218,103,67,40,40,249,94,39,28,136,241,133,49,41,32,37,100,16,83,26,214,180,56,160,42,245,76,92,215,189,190,190,5,0,4,129,88,
92,66,23,1,47,77,128,23,223,254,144,8,17,34,234,142,87,193,158,47,223,227,222,77,109,253,215,211,112,166,238,22,104,185,106,170,91,86,0,186,248,94,86,21,175,87,140,143,131,92,251,128,5,219,201,6,81,225,
180,98,38,13,148,12,117,149,207,235,21,91,240,229,7,96,124,1,139,47,40,241,241,128,9,245,172,41,9,75,177,62,227,168,228,251,9,185,182,23,196,148,120,70,49,231,43,125,14,223,178,100,198,59,254,218,13,17,
34,68,228,107,147,181,54,6,66,66,24,128,208,32,38,196,98,213,56,112,169,15,94,161,225,43,220,121,241,22,114,24,88,47,92,147,9,222,137,132,144,107,26,5,76,30,40,86,149,10,238,33,199,39,228,48,22,224,139,
19,192,164,128,79,32,80,113,1,86,164,89,44,49,224,167,36,152,17,99,81,185,123,68,3,184,180,4,93,196,192,39,66,132,8,17,241,246,31,247,62,182,76,72,8,11,11,98,90,130,49,9,227,130,108,155,230,74,4,175,240,
176,172,54,49,232,199,102,171,13,163,13,14,167,149,234,197,85,75,39,197,120,189,226,13,190,252,0,76,232,206,210,124,33,41,82,114,142,72,48,38,4,172,200,122,48,109,223,83,142,235,105,21,186,8,120,145,201,
12,34,68,136,40,215,30,185,61,6,220,246,39,23,132,5,150,11,11,98,161,12,184,168,47,180,145,14,93,209,4,47,2,95,220,245,19,52,73,192,110,54,158,16,121,77,163,146,19,7,50,135,186,74,13,57,140,37,248,242,
3,48,177,128,197,167,124,184,44,137,190,215,82,114,77,24,89,15,166,254,51,69,19,184,8,116,17,145,75,247,200,58,48,34,68,180,61,78,113,2,86,0,132,113,25,186,188,64,76,163,134,157,18,208,37,21,188,248,124,
163,120,6,175,224,73,130,72,233,229,5,172,105,12,85,159,10,47,4,147,154,212,37,94,225,43,8,192,164,64,143,152,228,26,129,229,249,222,83,233,116,244,76,28,45,254,212,226,181,99,9,186,8,120,197,71,155,57,
124,248,107,197,159,131,220,35,254,238,65,36,130,69,23,193,35,230,11,97,94,59,33,164,215,204,199,224,227,5,99,26,7,46,173,128,23,129,47,254,16,22,56,73,16,42,28,81,208,196,129,82,201,214,84,4,175,88,132,
47,63,0,139,212,248,228,78,206,225,251,155,82,235,194,226,113,230,90,143,123,131,169,13,92,106,128,26,129,46,210,142,136,16,33,162,112,123,140,0,98,94,131,206,243,179,193,207,224,13,187,142,44,192,8,84,
27,200,212,220,43,147,95,95,71,194,13,149,210,103,118,163,245,144,222,48,198,79,119,67,173,105,84,67,79,229,74,234,66,224,43,0,192,164,116,134,98,147,102,132,2,177,72,48,198,23,174,248,222,151,24,133,
202,221,115,219,158,47,136,245,16,67,114,119,135,91,73,37,16,33,66,68,107,131,99,72,154,242,252,228,15,97,190,99,63,159,229,51,161,12,79,41,6,175,84,200,138,38,116,241,185,6,129,47,17,31,144,115,34,33,
120,2,65,45,47,174,220,97,174,66,183,48,224,5,94,58,214,169,68,121,244,70,60,28,137,73,210,33,246,158,90,129,154,120,131,187,171,140,70,36,36,36,144,74,214,177,36,36,36,160,110,221,186,164,34,136,16,33,
162,59,16,11,52,100,125,199,188,200,235,112,212,131,40,165,128,75,78,240,138,116,29,2,94,242,234,175,199,65,198,192,99,66,137,247,226,70,83,23,197,236,29,23,235,240,37,27,128,73,133,163,80,25,142,132,
172,13,35,169,233,163,15,155,92,215,191,194,96,128,209,104,68,98,98,34,249,0,58,22,67,205,119,36,66,132,136,118,36,57,165,33,250,220,223,23,52,77,99,241,130,121,146,174,213,189,103,111,0,192,214,205,27,
49,106,204,120,108,219,178,17,165,37,197,146,174,57,106,204,120,172,92,182,4,14,135,61,122,116,226,103,172,50,62,198,106,100,16,11,184,132,38,108,115,229,198,125,133,192,139,192,151,76,32,198,29,74,43,
102,226,64,45,125,148,115,211,238,88,220,51,46,81,157,15,197,111,125,22,159,125,196,184,202,137,85,128,88,131,51,53,189,121,196,115,72,132,8,17,34,209,149,9,47,77,198,129,253,251,80,116,74,122,66,144,
228,148,134,236,127,103,101,103,227,224,126,233,30,239,172,236,108,80,209,158,184,241,177,78,107,33,204,224,99,0,134,223,163,52,148,157,167,148,249,32,199,208,42,108,124,86,40,212,144,128,151,60,202,192,
57,129,96,136,56,113,16,205,29,19,228,222,180,59,22,214,123,69,13,192,132,64,89,40,111,23,31,24,19,10,86,90,74,77,175,85,192,33,160,69,132,8,17,34,218,147,228,148,134,200,204,206,198,138,101,75,224,176,
123,60,76,22,139,21,105,233,141,96,43,59,231,231,117,50,153,205,200,202,250,55,42,42,206,163,178,226,60,123,220,91,222,233,188,200,121,143,252,230,45,1,0,69,167,190,6,77,211,65,199,11,11,142,249,149,207,
204,202,134,217,92,23,78,231,197,32,239,153,197,98,133,197,106,149,236,85,147,106,65,50,158,237,149,124,198,117,134,133,176,112,182,134,150,217,66,248,56,205,72,190,38,1,175,104,78,32,68,158,56,144,226,
197,85,206,219,74,188,94,154,0,176,72,31,192,87,169,188,138,37,118,125,152,28,0,69,82,211,147,58,35,66,132,8,17,173,72,247,158,189,97,164,140,200,111,209,18,78,231,69,152,205,102,188,241,214,44,28,57,
252,5,134,12,123,26,123,118,237,196,134,245,107,97,177,88,177,104,217,42,28,57,244,5,82,26,54,132,173,236,28,230,189,61,11,20,69,97,197,251,107,81,112,252,24,140,70,35,210,210,27,225,192,254,125,236,245,
31,123,124,48,42,42,126,132,217,92,23,195,71,142,194,192,71,250,1,240,132,21,186,92,78,80,20,133,241,19,39,227,190,158,93,0,0,67,134,13,71,167,206,93,176,103,215,14,80,20,229,7,90,201,41,13,49,115,206,
60,76,158,56,94,27,99,222,223,94,207,66,160,55,12,162,96,76,31,99,54,35,203,181,9,120,69,23,196,24,24,192,55,148,86,141,79,163,84,22,205,88,135,47,205,1,88,168,143,35,117,125,88,36,37,137,245,117,98,106,
128,16,129,45,34,68,136,16,81,79,22,47,152,135,238,61,122,177,107,191,94,126,229,13,124,248,193,42,108,221,188,17,38,179,25,43,222,95,135,13,235,215,162,245,109,183,227,147,245,107,177,230,253,85,160,
40,10,27,54,239,192,226,5,243,208,186,205,237,40,56,126,12,83,38,77,0,0,44,90,186,210,239,250,133,5,199,176,98,217,18,246,183,204,172,108,148,150,20,99,214,244,169,108,153,252,230,45,217,227,221,123,244,
66,159,158,93,252,60,101,0,144,146,210,16,163,199,142,199,228,137,227,163,231,253,10,97,225,121,141,89,127,59,128,9,2,177,80,99,156,210,182,131,244,113,149,145,237,62,4,188,180,167,183,82,60,184,234,232,
37,1,47,93,3,88,36,16,11,84,50,49,157,164,150,67,17,181,6,59,4,180,136,16,33,66,68,123,146,150,222,8,105,233,141,208,173,71,47,0,128,195,94,5,139,197,138,86,173,111,67,90,122,35,180,235,112,39,0,160,162,
226,60,204,230,186,104,214,188,37,78,20,30,247,1,174,227,126,215,243,13,97,44,41,46,70,243,22,45,81,90,82,140,225,35,71,193,98,177,34,165,97,67,164,167,55,130,217,92,23,185,121,77,80,81,113,62,8,190,0,
96,244,216,241,112,187,221,176,149,157,211,176,49,139,48,32,22,12,99,66,199,195,72,54,138,140,47,36,251,120,78,192,75,47,16,22,121,210,64,140,253,42,119,120,171,224,68,46,49,172,99,137,250,211,59,225,
123,123,201,57,115,21,15,0,66,32,139,8,17,34,68,244,37,133,5,199,80,110,43,195,214,205,27,253,142,159,46,250,6,54,91,89,80,150,196,179,165,197,200,200,204,174,5,184,180,116,216,108,101,236,223,190,73,
57,210,211,27,97,239,238,29,104,213,230,54,100,101,253,27,35,158,26,12,0,248,120,243,14,15,212,157,63,143,172,172,127,115,62,215,228,137,227,209,173,71,111,12,31,57,218,207,123,166,37,99,214,99,54,26,
96,48,132,10,229,138,12,99,209,25,83,25,69,158,37,242,82,30,98,35,104,9,194,106,245,149,145,101,194,64,9,93,36,91,23,196,0,128,69,250,152,225,18,121,8,81,196,88,13,75,140,70,42,122,34,68,136,16,33,162,
172,236,217,181,3,51,231,204,67,114,74,67,20,157,250,26,185,121,55,99,241,130,121,216,189,115,7,86,188,191,22,20,69,225,232,225,67,200,205,107,130,21,203,150,96,255,103,159,98,200,208,167,81,237,176,35,
201,98,69,110,94,19,63,0,107,223,225,78,184,156,23,209,32,185,33,44,86,43,138,78,125,131,204,172,108,36,55,108,136,246,29,238,68,179,154,68,28,128,199,91,182,255,179,79,49,99,206,60,28,61,124,8,70,35,
133,53,239,175,2,0,56,157,78,204,123,123,38,22,45,93,133,246,29,238,244,91,103,166,77,131,22,126,32,22,30,198,196,67,153,210,160,37,102,76,38,224,165,79,8,243,234,171,16,239,173,90,58,73,192,43,180,36,
196,134,14,50,156,255,194,253,46,246,154,66,175,19,173,119,87,250,121,181,90,31,68,136,16,33,18,47,50,97,220,104,246,191,75,75,138,49,100,208,163,160,221,110,100,101,253,27,71,15,127,193,194,209,99,253,
251,161,218,97,71,126,139,150,108,230,66,151,211,137,62,61,187,192,225,176,227,68,193,49,60,214,191,31,235,61,155,249,214,84,12,25,52,0,78,167,19,23,42,207,99,200,160,1,236,61,38,140,29,133,204,236,127,
99,239,238,29,24,49,108,48,74,74,190,3,0,76,153,52,1,243,230,204,100,97,205,123,29,135,189,10,52,77,227,249,103,135,163,194,39,3,163,210,66,81,20,114,243,154,4,29,79,78,105,136,252,230,45,217,127,126,
101,24,6,150,36,11,154,229,183,64,157,58,20,231,120,231,149,215,166,206,8,48,70,149,252,39,126,124,230,3,93,222,127,17,11,213,72,102,86,54,50,179,178,67,22,207,247,129,243,64,201,205,107,2,138,162,56,
127,235,222,179,55,90,181,185,141,52,108,161,16,198,212,110,216,28,252,205,165,235,147,208,107,68,210,191,144,250,22,81,17,99,75,18,99,79,23,249,37,219,144,107,227,102,146,29,145,8,145,255,111,239,236,
131,162,56,243,60,254,225,109,192,72,115,134,153,220,149,144,171,204,248,135,96,85,6,45,17,111,69,172,85,99,197,132,85,163,149,151,141,47,133,41,179,98,18,241,146,5,53,59,102,77,200,198,64,98,2,27,35,
217,92,100,141,134,179,18,56,196,247,53,198,10,193,212,26,173,32,100,5,172,146,113,235,132,189,219,25,178,161,199,23,122,146,203,180,2,247,199,204,180,51,204,12,162,17,29,224,249,84,89,74,219,244,76,119,
255,158,167,159,111,255,222,4,2,193,157,192,43,116,188,116,216,109,90,225,12,95,28,14,89,243,72,249,162,170,106,64,200,162,87,104,1,33,255,47,84,49,141,190,161,142,190,251,57,28,242,109,109,200,172,55,
220,67,209,155,37,204,127,120,78,192,34,127,254,130,133,154,24,116,200,50,45,205,5,140,77,74,166,232,205,18,76,166,113,88,173,103,153,146,241,111,236,216,190,141,178,119,127,239,87,194,219,251,92,52,155,
39,14,217,231,244,192,10,215,245,6,21,86,69,155,75,80,20,5,0,131,222,64,225,70,139,230,213,156,255,200,34,45,7,81,111,40,100,219,251,101,28,61,226,14,83,205,95,103,97,138,86,177,51,129,163,71,14,7,216,
170,111,216,171,224,6,111,168,167,76,125,255,222,91,95,65,117,251,237,112,164,123,188,134,189,0,11,101,16,193,66,19,131,25,231,72,8,77,20,85,17,5,130,27,35,54,54,150,184,184,56,34,35,35,197,197,16,12,
26,145,145,145,33,189,3,130,91,199,193,3,251,2,114,226,10,214,91,56,121,226,56,171,158,206,65,85,85,244,122,3,69,155,75,120,244,177,95,82,179,187,202,103,13,16,248,156,51,26,77,184,84,23,29,118,187,199,
243,54,9,135,67,230,111,158,144,206,201,233,25,126,5,79,238,51,154,112,42,74,128,32,141,143,151,24,159,146,138,170,186,56,211,210,204,253,230,52,218,219,218,112,58,21,122,123,123,209,235,13,72,146,132,
75,117,161,186,84,244,6,247,207,45,205,77,126,5,80,140,70,19,122,131,1,112,23,87,25,120,209,186,208,59,234,116,58,138,54,151,176,97,125,129,230,69,53,167,77,100,235,31,202,53,79,106,222,243,249,44,121,
124,81,192,121,229,175,179,144,148,156,204,146,39,22,13,248,30,121,175,163,247,179,220,231,116,143,214,147,206,43,214,188,125,237,116,58,29,41,169,19,2,94,74,192,181,254,116,222,66,48,146,36,249,133,219,
154,211,38,98,109,61,75,74,234,4,218,218,206,147,146,50,1,135,220,233,183,143,247,251,0,1,125,246,194,70,132,129,95,24,109,176,117,112,216,136,174,17,44,188,70,132,0,235,207,80,66,149,177,191,158,145,
222,172,176,184,19,134,47,68,150,64,112,107,48,155,205,36,38,38,10,1,38,24,116,1,22,19,19,195,247,151,191,21,23,227,54,18,47,73,204,154,61,135,146,205,197,154,144,113,56,100,190,172,171,101,222,130,133,
212,84,87,106,234,171,239,99,111,90,102,22,185,207,172,102,205,234,92,0,222,254,125,153,230,93,51,232,13,172,205,95,195,202,85,171,169,252,184,66,243,20,189,93,186,149,181,191,206,67,150,59,253,68,130,
229,165,87,56,121,226,56,137,122,3,241,241,18,211,50,179,184,224,112,176,115,71,185,71,200,252,134,47,235,190,192,104,50,49,45,51,11,187,221,134,67,150,121,253,141,18,30,91,244,11,156,138,194,180,204,
44,94,249,93,17,7,247,239,69,146,18,104,110,106,10,90,157,242,70,23,193,153,211,103,160,40,138,95,243,237,150,230,38,90,154,155,152,251,112,54,31,239,170,192,110,179,145,158,49,85,243,122,121,153,53,251,
1,10,55,110,184,33,241,85,242,206,123,28,253,236,48,141,13,48,109,122,22,185,207,228,113,242,196,113,242,158,207,167,108,75,41,224,238,85,231,45,8,51,115,246,28,102,206,122,128,150,230,2,191,99,189,253,
78,25,237,109,231,81,85,149,121,11,22,82,182,165,148,173,239,151,107,45,19,140,70,19,175,188,86,204,99,143,100,179,245,15,229,52,52,212,211,222,118,30,115,218,68,90,154,155,124,122,230,85,97,183,219,56,
103,109,69,111,48,4,156,99,88,9,49,31,59,13,246,194,224,70,215,166,183,220,203,42,214,142,35,75,128,13,84,144,93,207,216,70,82,245,68,33,176,4,2,55,99,226,163,232,81,47,211,19,198,223,177,191,185,105,
40,120,238,127,202,124,51,156,230,42,85,12,183,65,103,254,130,133,164,79,201,0,224,228,87,199,105,108,168,199,110,251,187,230,77,241,21,24,139,151,230,4,46,28,61,227,233,103,153,89,172,124,102,53,121,
207,229,226,84,20,204,230,137,40,74,23,111,188,254,42,0,59,42,62,97,108,82,18,149,31,87,48,111,254,66,142,213,213,114,191,121,34,178,44,251,121,87,188,2,204,229,82,169,169,174,210,188,43,29,118,27,239,
190,87,206,206,29,229,196,75,18,102,243,36,94,125,121,3,79,153,86,226,114,169,188,244,155,181,244,246,130,78,23,203,180,204,25,28,61,114,152,197,75,114,88,243,92,110,255,61,215,110,98,188,24,77,227,104,
60,245,117,192,246,198,134,122,238,51,154,0,88,251,66,30,150,141,133,172,121,62,159,173,91,74,57,122,228,176,199,107,151,224,39,220,250,23,95,177,154,248,242,134,192,46,89,186,156,13,47,22,208,97,183,
209,210,124,154,121,11,22,242,234,203,27,176,108,44,100,108,82,50,29,118,27,243,23,44,10,26,126,107,78,155,200,39,187,42,2,132,99,230,244,25,28,171,171,101,222,35,139,216,227,17,216,82,66,130,182,175,
183,129,121,217,150,82,230,62,252,11,236,118,27,107,95,200,27,42,139,183,160,47,12,130,61,6,130,205,157,222,252,73,151,75,229,76,75,83,191,66,217,155,67,233,61,140,213,218,138,211,19,162,234,37,125,74,
134,123,123,87,151,152,124,132,0,27,215,70,187,129,0,0,10,243,73,68,65,84,248,3,252,70,67,20,195,125,193,115,187,22,42,66,188,9,4,119,118,108,15,246,75,165,59,57,39,136,249,69,240,83,104,56,85,207,161,
3,251,0,119,191,52,69,81,72,74,190,151,120,73,242,91,60,26,77,227,180,124,167,190,11,92,73,146,216,240,210,43,212,125,81,171,253,206,180,233,89,152,205,147,248,143,114,119,83,107,151,75,229,62,227,56,
234,190,168,37,127,173,5,189,222,192,188,5,11,57,116,48,48,183,238,208,129,125,24,244,247,240,193,31,63,162,161,161,158,178,119,75,105,111,111,195,106,61,203,207,50,179,72,74,74,166,174,238,115,92,46,
149,222,94,104,105,62,173,45,122,29,14,153,88,79,232,106,186,167,95,219,79,21,92,125,177,219,109,65,11,100,220,103,52,105,161,150,14,135,204,218,23,242,48,26,77,88,54,22,34,73,9,212,84,87,162,139,141,
29,240,231,44,94,150,131,36,73,20,109,42,212,22,249,233,25,83,41,124,173,72,219,199,33,187,5,234,158,234,74,30,125,252,151,124,178,171,2,163,201,20,84,228,173,125,33,143,197,75,115,40,88,111,97,235,150,
18,78,126,117,156,157,31,150,147,251,108,30,199,234,106,153,251,80,54,79,45,123,210,71,156,157,14,184,166,99,147,146,249,102,128,2,50,92,133,88,48,51,8,53,245,235,13,6,222,125,175,156,150,230,38,36,73,
194,225,144,41,126,189,144,14,187,221,239,208,137,250,107,251,121,41,121,171,152,115,214,86,226,37,137,194,223,21,161,215,27,80,85,151,59,124,180,233,180,214,248,253,102,121,240,161,236,240,245,62,10,
1,118,103,132,217,72,91,32,136,5,144,64,48,116,5,217,80,25,203,98,158,17,12,6,29,29,246,128,197,186,181,245,44,83,166,76,245,43,147,63,57,61,131,134,83,193,23,222,46,213,197,226,39,22,177,109,123,5,51,
103,206,230,88,93,45,141,167,234,49,155,39,106,225,136,190,212,236,174,226,193,185,217,100,102,206,160,100,115,113,80,77,180,227,195,114,118,124,88,78,222,191,255,154,149,171,86,83,250,214,27,28,58,176,
143,153,51,31,192,104,26,71,137,79,31,181,80,97,133,45,205,167,25,63,62,165,127,15,216,77,208,120,170,158,194,215,138,253,68,170,78,167,99,214,236,57,90,149,76,47,237,237,109,28,58,176,143,201,233,25,
212,84,87,114,226,171,63,243,232,227,79,94,11,229,236,135,61,213,149,40,138,66,209,155,37,90,62,94,75,243,105,202,182,148,6,228,119,29,216,191,151,157,187,170,112,42,10,53,213,85,33,174,71,147,86,100,
101,231,174,74,30,125,36,219,29,70,168,55,240,224,67,217,88,173,173,126,249,92,193,174,171,83,233,242,235,153,55,100,133,88,31,213,21,106,122,237,237,117,139,220,85,191,90,14,192,146,165,57,60,185,36,
135,210,183,222,8,216,215,119,63,223,3,108,251,227,71,212,84,87,13,232,158,123,195,61,205,105,110,239,112,135,221,134,94,111,192,104,26,231,151,139,55,109,122,22,79,173,88,137,67,150,181,60,188,120,73,
34,37,101,2,118,187,45,192,131,45,4,216,8,21,32,67,181,88,135,88,240,8,4,35,107,206,186,211,115,149,152,115,4,131,133,36,37,144,251,236,181,176,177,96,85,29,189,20,111,42,164,232,205,18,238,79,155,200,
153,230,38,126,62,203,45,122,86,245,17,23,218,66,221,229,194,169,40,20,188,144,199,182,237,31,97,181,182,210,216,80,79,222,243,249,88,94,122,153,99,117,181,152,211,38,81,243,95,149,56,28,50,7,247,239,
101,231,127,186,197,72,176,69,254,76,207,231,89,173,103,73,73,153,160,21,254,56,86,87,203,242,21,191,2,184,190,103,171,183,151,67,7,246,97,249,109,33,123,118,87,49,54,41,57,160,216,200,205,226,112,200,
148,108,46,102,231,174,42,246,84,87,226,114,185,115,170,118,87,87,114,206,218,202,248,148,84,230,45,88,196,95,207,181,34,73,18,139,151,230,176,213,147,171,85,246,78,9,31,124,88,193,188,5,11,57,121,226,
184,251,188,190,248,60,168,72,84,20,133,131,251,247,50,57,61,67,107,222,253,201,174,10,44,191,45,164,166,186,10,69,233,34,54,54,150,131,251,247,226,84,20,26,79,125,205,226,101,57,44,121,60,120,129,143,
252,117,22,206,180,52,33,73,9,216,109,54,77,60,238,217,93,69,193,122,11,133,27,45,215,61,247,207,62,61,204,188,5,139,200,95,103,225,111,237,109,180,183,157,31,112,72,229,80,17,99,253,33,59,100,166,101,
102,13,236,152,184,195,105,21,165,107,64,226,11,160,232,205,18,78,120,218,102,28,61,242,41,14,185,147,130,245,22,218,219,219,24,159,146,138,221,102,163,244,173,98,30,156,155,141,36,73,164,123,170,105,
130,59,199,175,165,185,9,131,193,128,44,203,225,217,236,93,8,176,225,177,168,8,85,205,81,32,16,8,110,247,92,37,230,35,65,184,226,144,59,41,232,147,179,163,40,93,33,69,88,75,115,19,79,45,123,146,244,140,
169,140,79,157,192,209,207,14,115,242,171,227,33,143,239,237,193,214,97,183,177,230,217,149,218,246,53,207,173,100,214,236,57,152,211,38,113,174,245,172,182,80,116,200,157,200,114,167,123,81,26,100,204,
88,91,207,66,111,47,38,163,137,173,91,74,252,196,73,251,249,243,156,105,105,214,126,175,239,57,28,220,191,23,213,229,210,254,125,226,248,159,153,251,112,182,22,78,119,171,168,169,174,164,241,212,215,100,
102,205,32,54,86,71,241,166,66,237,123,218,237,54,190,172,171,37,37,53,21,89,150,89,252,196,34,77,236,180,183,183,241,232,35,217,100,78,159,129,209,52,14,167,210,165,133,17,134,58,143,226,77,133,90,213,
193,99,117,181,56,28,50,211,166,207,0,240,203,69,107,108,56,133,222,112,79,200,170,132,222,239,100,183,217,180,130,29,128,230,217,244,189,199,185,43,114,130,222,99,135,67,102,201,19,238,30,101,73,73,247,
222,214,126,118,183,77,140,249,109,119,87,138,204,125,102,53,224,206,153,44,217,92,28,68,240,187,195,21,63,216,254,145,182,105,213,211,203,49,167,77,210,42,78,130,187,61,193,216,164,100,156,74,87,208,
118,24,238,151,11,86,63,193,230,189,246,241,146,196,206,93,85,148,190,85,204,161,3,251,72,74,190,246,82,33,247,89,119,97,22,239,207,7,63,253,156,178,45,37,253,23,157,17,2,76,16,110,194,78,32,16,8,196,
124,36,24,46,168,170,26,212,75,225,12,150,211,229,21,109,14,121,192,249,37,190,225,112,190,5,53,156,30,15,78,95,210,167,76,197,169,40,33,67,3,59,66,132,80,141,77,74,102,74,198,84,138,55,21,250,237,219,
247,119,251,158,71,168,133,238,79,165,189,189,45,160,128,136,247,188,27,27,234,67,122,134,156,138,114,221,107,235,123,30,125,239,159,183,226,162,47,58,157,142,197,75,115,250,245,122,132,250,78,185,207,
172,102,219,251,239,5,236,27,234,30,247,21,107,35,133,198,83,245,253,122,81,29,114,39,171,158,94,30,176,45,125,89,142,223,203,5,187,205,70,225,107,69,33,237,210,87,84,235,245,6,242,158,207,7,32,41,57,
89,203,197,235,139,209,104,98,124,234,4,173,160,142,221,110,195,104,26,119,203,195,111,133,0,19,8,4,2,129,64,48,226,137,140,138,164,167,187,103,200,124,223,15,182,127,132,67,150,7,20,238,230,75,254,58,
11,38,211,56,54,188,88,48,100,222,234,223,46,188,77,159,247,236,174,10,218,251,43,20,227,83,82,41,88,111,193,218,218,58,224,16,185,27,34,98,120,216,179,162,116,221,116,248,106,75,115,19,73,73,201,24,141,
38,218,219,219,6,36,136,92,62,246,189,124,197,74,254,122,174,149,143,119,85,48,62,37,149,146,119,202,60,162,220,229,47,136,79,28,71,81,186,40,222,244,106,248,26,106,68,255,2,172,151,193,104,139,45,16,
8,4,67,139,222,62,127,11,4,130,48,28,143,49,49,209,184,186,135,142,32,233,235,33,24,40,67,41,159,229,118,115,112,255,222,126,115,250,66,113,206,218,122,211,247,99,32,196,68,71,15,123,123,246,37,88,158,
101,135,221,70,225,70,11,91,223,47,231,224,129,125,180,52,159,214,194,73,7,130,211,169,48,121,202,84,100,217,157,123,230,173,68,106,109,61,139,209,52,142,7,31,202,166,241,84,61,159,125,250,39,150,175,
88,73,254,58,11,95,214,213,242,243,89,15,132,221,152,233,207,30,188,2,172,71,12,103,193,96,16,27,27,75,92,92,156,104,96,59,196,137,140,140,68,23,34,12,96,152,208,237,153,7,35,196,124,40,8,55,238,49,220,
77,167,124,113,216,158,219,141,142,71,93,116,52,46,209,49,77,16,134,196,196,68,15,121,123,118,200,157,90,14,214,245,246,11,150,103,9,112,244,200,97,90,154,155,152,146,49,21,115,218,36,156,74,87,200,99,
110,120,177,0,135,79,83,242,157,219,183,49,115,246,28,12,6,3,101,91,74,145,36,9,112,135,165,174,90,145,195,204,217,115,56,231,41,143,191,106,69,14,115,31,206,38,61,99,42,95,250,84,48,13,115,123,232,241,
10,176,110,224,170,24,54,130,193,192,108,54,147,152,152,40,4,216,48,16,96,49,49,49,124,127,249,219,225,122,138,61,222,73,81,204,135,2,65,120,143,199,184,184,88,126,116,169,92,185,34,134,170,32,124,136,
138,138,226,174,81,113,67,222,158,221,165,255,155,6,180,95,127,213,32,59,236,182,1,121,41,251,126,150,170,170,126,249,130,190,5,86,218,219,219,252,26,110,15,102,190,227,32,218,67,55,208,19,237,49,6,213,
102,179,95,50,24,244,99,196,16,18,220,74,198,196,71,209,163,94,22,46,133,97,192,112,125,223,220,209,209,113,21,112,1,87,60,15,72,151,152,15,5,225,198,112,244,130,5,243,126,13,116,60,198,199,223,133,162,
252,192,213,171,66,132,9,194,99,177,157,32,221,21,208,214,67,216,179,176,135,32,246,160,2,221,145,30,195,248,254,226,165,203,255,29,21,21,37,174,154,64,32,24,57,162,82,85,137,136,140,250,95,224,255,128,
31,61,127,139,249,80,16,182,130,37,68,200,222,176,56,143,27,25,143,209,81,81,220,61,70,98,244,232,81,68,71,139,122,98,130,59,67,116,116,20,163,70,197,113,247,24,41,192,14,133,61,11,123,232,199,30,212,
104,143,81,40,81,49,186,51,157,157,157,233,137,137,137,226,10,10,4,130,17,67,47,145,103,129,203,192,247,184,223,80,118,137,249,80,16,238,2,70,140,71,55,119,141,138,11,21,230,35,16,8,123,22,132,163,61,
40,192,143,81,64,44,16,115,225,98,215,119,177,49,17,25,221,221,87,239,30,61,122,180,184,74,2,129,96,216,243,205,55,127,233,236,188,244,195,219,61,61,61,54,224,18,238,136,128,136,11,23,187,254,33,230,67,
129,64,140,71,129,64,216,179,224,86,217,195,119,23,191,127,187,167,167,231,239,192,133,40,220,165,232,35,128,8,157,46,246,219,238,171,106,106,79,119,247,63,93,186,116,137,132,132,4,113,197,4,2,193,176,
195,102,179,97,181,158,235,140,142,139,223,162,56,127,104,6,190,3,156,158,7,100,143,152,15,5,2,49,30,5,2,97,207,130,65,176,135,78,64,137,0,226,128,209,192,221,192,191,0,99,141,247,254,243,178,238,171,
234,228,43,170,235,95,175,92,185,34,174,158,64,32,24,54,68,69,199,116,196,232,98,207,124,119,193,89,113,229,202,213,255,1,254,1,92,0,126,192,29,34,50,74,204,135,2,129,24,143,2,129,176,103,193,32,217,131,
51,2,183,7,76,7,220,5,72,30,195,24,3,36,120,182,233,128,40,110,170,191,183,64,32,16,132,5,189,184,75,191,170,184,19,96,21,220,33,33,23,129,46,207,195,209,229,217,87,204,135,2,129,24,143,2,129,176,103,
193,160,217,67,132,231,70,71,1,49,184,243,193,70,121,254,196,121,126,142,6,188,77,156,132,81,8,4,130,161,58,33,246,224,105,187,193,181,138,84,222,234,84,87,184,214,167,69,204,135,2,129,24,143,2,129,176,
103,193,160,217,195,255,3,131,90,39,61,40,12,10,116,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* GlobalEditor::globalEditor_864x144_png = (const char*) resource_GlobalEditor_globalEditor_864x144_png;
const int GlobalEditor::globalEditor_864x144_pngSize = 27571;


//[EndFile] You can add extra defines here...
//[/EndFile]
