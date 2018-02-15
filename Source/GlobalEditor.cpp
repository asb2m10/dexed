/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

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
 *
class MidiMonitor : public Component {
    SysexComm *midi;
    Image light;

public:
    MidiMonitor(SysexComm *sysexComm) {
        midi = sysexComm;
        light = DXLookNFeel::getLookAndFeel()->imageLight;
    }

    void paint(Graphics &g) {
        if ( ! (midi->isInputActive() || midi->isOutputActive() ) )
            return;
        g.setColour(Colours::white);

        if ( midi->isInputActive() ) {
            g.drawSingleLineText("DX7 IN", 17,14);
            g.drawImage(light, 0, 3, 14, 14, 0, midi->inActivity ? 14 : 0, 14, 14);
            midi->inActivity = false;
        }

        if ( midi->isOutputActive() ) {
            g.drawSingleLineText("DX7 OUT", 17, 28);
            g.drawImage(light, 0, 17, 14, 14, 0, midi->outActivity ? 14 : 0, 14, 14);
            midi->outActivity = false;
        }
    }
};*/

class AboutBox : public DialogWindow {
public:
    Image about_png;

    AboutBox(Component *parent) : DialogWindow("About", Colour(0xFF000000), true) {
        setUsingNativeTitleBar(false);
        setAlwaysOnTop(true);
        about_png = ImageCache::getFromMemory(BinaryData::about_png, BinaryData::about_pngSize);
        setSize(about_png.getWidth(), about_png.getHeight());
        centreAroundComponent (parent, getWidth(), getHeight());
    }

    void closeButtonPressed() {
        setVisible (false);
    }

    void paint(Graphics &g) {
        g.drawImage (about_png, 0, 0, about_png.getWidth(), about_png.getHeight(),
                     0, 0, about_png.getWidth(), about_png.getHeight());
        g.setColour(Colour(0xFF000000));
        String ver("Version " DEXED_VERSION " ; build date " __DATE__ );
        g.drawSingleLineText(ver, 18, 130);
    }
};
//[/MiscUserDefs]

//==============================================================================
GlobalEditor::GlobalEditor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

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
    oscSync->setButtonText (String());
    oscSync->addListener (this);

    addAndMakeVisible (pitchModSens = new Slider ("pitchModSens"));
    pitchModSens->setRange (0, 7, 1);
    pitchModSens->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchModSens->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pitchModSens->addListener (this);

    addAndMakeVisible (lfoSync = new ToggleButton ("lfoSync"));
    lfoSync->setButtonText (String());
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

    addAndMakeVisible (cartButton = new TextButton ("cartButton"));
    cartButton->setButtonText (TRANS("CART"));
    cartButton->addListener (this);

    addAndMakeVisible (storeButton = new TextButton ("storeButton"));
    storeButton->setButtonText (TRANS("STORE"));
    storeButton->addListener (this);

    addAndMakeVisible (monoMode = new ToggleButton ("monoMode"));
    monoMode->setButtonText (String());
    monoMode->addListener (this);

    addAndMakeVisible (lfoType = new ComboBoxImage());
    lfoType->setName ("lfoType");

    addAndMakeVisible (programSelector = new ProgramSelector());
    programSelector->setName ("programSelector");

    addAndMakeVisible (aboutButton = new ImageButton ("aboutButton"));
    aboutButton->setButtonText (String());
    aboutButton->addListener (this);

    aboutButton->setImages (false, true, false,
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000),
                            Image(), 1.000f, Colour (0x00000000));
    addAndMakeVisible (tune = new Slider ("tune"));
    tune->setRange (0, 1, 0);
    tune->setSliderStyle (Slider::RotaryVerticalDrag);
    tune->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    tune->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (864, 144);


    //[Constructor] You can add your own custom stuff here..
    SharedResourcePointer<DXLookNFeel> lookAndFeel;
    lfoType->addItem("TRIANGLE", 1);
    lfoType->addItem("SAW DOWN", 2);
    lfoType->addItem("SAW UP", 3);
    lfoType->addItem("SQUARE", 4);
    lfoType->addItem("SINE", 5);
    lfoType->addItem("S&HOLD", 6);
    lfoType->setImage(lookAndFeel->imageLFO);

    programs = programSelector;

    background = lookAndFeel->imageGlobal;
    imageLight = lookAndFeel->imageLight;
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
    cartButton = nullptr;
    storeButton = nullptr;
    monoMode = nullptr;
    lfoType = nullptr;
    programSelector = nullptr;
    aboutButton = nullptr;
    tune = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GlobalEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.drawImage(background, 0, 0, 864, 144, 0, 0, 864, 144);
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    g.drawImage(imageLight, 300, 70, 14, 14, 0, monoMode->getToggleState() ? 14 : 0, 14, 14);
    g.drawImage(imageLight, 619, 102, 14, 14, 0, lfoSync->getToggleState() ? 14 : 0, 14, 14);
    g.drawImage(imageLight, 705, 102, 14, 14, 0, oscSync->getToggleState() ? 14 : 0, 14, 14);
    //[/UserPaint]
}

void GlobalEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

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
    initButton->setBounds (100, 111, 50, 30);
    parmButton->setBounds (52, 111, 50, 30);
    cartButton->setBounds (3, 111, 50, 30);
    storeButton->setBounds (270, 109, 50, 30);
    monoMode->setBounds (249, 65, 48, 26);
    lfoType->setBounds (583, 8, 36, 26);
    programSelector->setBounds (153, 115, 112, 18);
    aboutButton->setBounds (8, 11, 135, 46);
    tune->setBounds (190, 9, 34, 34);
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
    else if (sliderThatWasMoved == tune)
    {
        //[UserSliderCode_tune] -- add your slider handling code here..
        //[/UserSliderCode_tune]
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
    else if (buttonThatWasClicked == cartButton)
    {
        //[UserButtonCode_cartButton] -- add your button handler code here..
        editor->cartShow();
        //[/UserButtonCode_cartButton]
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
    else if (buttonThatWasClicked == aboutButton)
    {
        //[UserButtonCode_aboutButton] -- add your button handler code here..
        AboutBox about(this->getParentComponent());
        about.runModalLoop();
        //[/UserButtonCode_aboutButton]
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
    processor->tune->bind(tune);
    algoDisplay->algo = (char *) &(processor->data[134]);
    pitchEnvDisplay->pvalues = &(processor->data[126]);

    algoDisplay->opStatus = processor->controllers.opSwitch;

    editor = edit;

    //midiMonitor = new MidiMonitor(&(processor->sysexComm));
    //addAndMakeVisible(midiMonitor);
    //midiMonitor->setBounds(155, 21, 80, 45);

    repaint();
}

void GlobalEditor::setSystemMessage(String msg) {
    lcdDisplay->setSystemMsg(msg);
}

void GlobalEditor::setParamMessage(String msg) {
    lcdDisplay->paramMsg = msg;
}

void GlobalEditor::updateDisplay() {
    repaint();
}

void GlobalEditor::updatePitchPos(int pos) {
    pitchEnvDisplay->vPos = pos;
    pitchEnvDisplay->repaint();
}

void GlobalEditor::updateVu(float f) {
    vuOutput->v = f;
    vuOutput->repaint();
    //midiMonitor->repaint();
}

void GlobalEditor::setMonoState(bool state)  {
    monoMode->setToggleState(state ? Button::buttonDown : Button::buttonNormal, dontSendNotification);
}

void GlobalEditor::mouseDown(const MouseEvent &e) {
    if ( e.mods.isRightButtonDown() || e.mods.isAnyModifierKeyDown() ) {
        PopupMenu popup;
        popup.addItem(1, "Send current program to DX7");
        if ( popup.show() == 1 )
           processor->sendCurrentSysexProgram();
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GlobalEditor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="864" initialHeight="144">
  <BACKGROUND backgroundColour="ffffff"/>
  <SLIDER name="lfoSpeed" id="b10eaf327ab3bff5" memberName="lfoSpeed" virtualName=""
          explicitFocusOrder="0" pos="564 50 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="lfoAmDepth" id="3e95a6206fa4a891" memberName="lfoAmDepth"
          virtualName="" explicitFocusOrder="0" pos="686 50 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="lfoPitchDepth" id="6ead769ca786c813" memberName="lfoPitchDepth"
          virtualName="" explicitFocusOrder="0" pos="646 50 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="lfoDelay" id="1fce68dc81619ef5" memberName="lfoDelay" virtualName=""
          explicitFocusOrder="0" pos="603 50 34 34" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="cutoff" id="40531f16bb0bd225" memberName="cutoff" virtualName=""
          explicitFocusOrder="0" pos="234 9 34 34" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="reso" id="c8c13464e81a8d83" memberName="reso" virtualName=""
          explicitFocusOrder="0" pos="278 9 34 34" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="pitchRate2" id="73f386b3c91d3de4" memberName="pitchRate2"
          virtualName="" explicitFocusOrder="0" pos="767 96 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="pitchRate3" id="187c25b24413fccf" memberName="pitchRate3"
          virtualName="" explicitFocusOrder="0" pos="795 96 35 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="pitchRate4" id="1aeb2a8fbbcbeaab" memberName="pitchRate4"
          virtualName="" explicitFocusOrder="0" pos="823 96 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="pitchRate1" id="419d613b3fb9604e" memberName="pitchRate1"
          virtualName="" explicitFocusOrder="0" pos="739 96 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="pitchLevel2" id="3a355ad2221887d9" memberName="pitchLevel2"
          virtualName="" explicitFocusOrder="0" pos="767 57 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="pitchLevel3" id="5c5d782e8a7f3ad7" memberName="pitchLevel3"
          virtualName="" explicitFocusOrder="0" pos="795 56 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="pitchLevel4" id="eb900b141fbad8ff" memberName="pitchLevel4"
          virtualName="" explicitFocusOrder="0" pos="823 56 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="pitchLevel1" id="344cbe26ec9fa128" memberName="pitchLevel1"
          virtualName="" explicitFocusOrder="0" pos="739 57 34 34" min="0"
          max="99" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="transpose" id="7d1266b1c1534947" memberName="transpose"
          virtualName="" explicitFocusOrder="0" pos="202 60 34 34" min="0"
          max="48" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TOGGLEBUTTON name="oscSync" id="8f3fe641537cd00" memberName="oscSync" virtualName=""
                explicitFocusOrder="0" pos="650 96 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="pitchModSens" id="904f73df85a9f886" memberName="pitchModSens"
          virtualName="" explicitFocusOrder="0" pos="666 5 34 34" min="0"
          max="7" int="1" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
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
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="algo" id="8a226ddf9bbff752" memberName="algo" virtualName=""
          explicitFocusOrder="0" pos="501 22 34 34" min="1" max="32" int="1"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <GENERICCOMPONENT name="lcdDisplay" id="30c7bb8f114cbbe3" memberName="lcdDisplay"
                    virtualName="" explicitFocusOrder="0" pos="6 87 140 13" class="LcdDisplay"
                    params=""/>
  <SLIDER name="output" id="7697fdd54fd1593e" memberName="output" virtualName=""
          explicitFocusOrder="0" pos="157 60 34 34" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <GENERICCOMPONENT name="vuOutput" id="dac75af912267f51" memberName="vuOutput" virtualName=""
                    explicitFocusOrder="0" pos="6 103 140 8" class="VuMeter" params=""/>
  <TEXTBUTTON name="initButton" id="92b278163c42e21d" memberName="initButton"
              virtualName="" explicitFocusOrder="0" pos="100 111 50 30" buttonText="INIT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="parmButton" id="d5cd6260b34be817" memberName="parmButton"
              virtualName="" explicitFocusOrder="0" pos="52 111 50 30" buttonText="PARM"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="cartButton" id="465a101c3a0e744" memberName="cartButton"
              virtualName="" explicitFocusOrder="0" pos="3 111 50 30" buttonText="CART"
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
  <IMAGEBUTTON name="aboutButton" id="d195a60b29440aa1" memberName="aboutButton"
               virtualName="" explicitFocusOrder="0" pos="8 11 135 46" buttonText=""
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="0"
               resourceNormal="" opacityNormal="1" colourNormal="0" resourceOver=""
               opacityOver="1" colourOver="0" resourceDown="" opacityDown="1"
               colourDown="0"/>
  <SLIDER name="tune" id="d22c34aa3363a28a" memberName="tune" virtualName=""
          explicitFocusOrder="0" pos="190 9 34 34" min="0" max="1" int="0"
          style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
