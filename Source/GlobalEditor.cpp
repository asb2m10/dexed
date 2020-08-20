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
#include "PluginEditor.h"
#include "DXLookNFeel.h"
//[/Headers]

#include "GlobalEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

/**
 * Ugly but useful midi monitor to know if you are really sending/receiving something from the DX7
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
    Image logo_png;
    HyperlinkButton dexed;
    HyperlinkButton surge;

    AboutBox(Component *parent) : DialogWindow("About", Colour(0xFF000000), true), 
            dexed("https://asb2m10.github.io/dexed/", URL("https://asb2m10.github.io/dexed/")),
            surge("https://surge-synthesizer.github.io/", URL("https://surge-synthesizer.github.io/")) {
        setUsingNativeTitleBar(false);
        setAlwaysOnTop(true);
        logo_png = ImageCache::getFromMemory(BinaryData::dexedlogo_png, BinaryData::dexedlogo_pngSize);
        setSize(logo_png.getWidth()+ 8, 500);
        centreAroundComponent(parent, getWidth(), getHeight());

        dexed.setColour(HyperlinkButton::ColourIds::textColourId, Colour(0xFF4ea097));
        dexed.setJustificationType(Justification::left);        
        dexed.setBounds(18, 433, getWidth() - 36, 30);
        addAndMakeVisible(&dexed);
        surge.setColour(HyperlinkButton::ColourIds::textColourId, Colour(0xFF4ea097));
        surge.setJustificationType(Justification::left);        
        surge.setBounds(18, 458, getWidth() - 36, 30);
        addAndMakeVisible(&surge);
    }

    void closeButtonPressed() {
        setVisible (false);
    }

    void paint(Graphics &g) {
        g.fillAll(Colour(0xFF554F46));
        g.drawImage (logo_png, 0, 10, logo_png.getWidth(), logo_png.getHeight(),
                     0, 0, logo_png.getWidth(), logo_png.getHeight());
        g.setFont(20);
        g.setColour(Colour(0xFFFFFFFF));
        const char *credits = "Version " DEXED_VERSION " build date: " __DATE__ "\n"
                            "This software is released under the GPL V3.\n\n"
                            "DSP Engine: orignal project (msfa) Raph Levin, enhancements Pascal Gauthier\n"
                            "UI Programming: Pascal Gauthier\n"
                            "UI Design: AZur Studio\n\n"
                            "Credits to Surge Synthesizer Team for MPE and microtuning support\n"
                            "Credits to GitHub users: tico-tico, Sentinel77, jeremybernstein; filters based on OB-Xd";
        g.drawMultiLineText(credits, 18, 260, logo_png.getWidth()-18);
    }
};
//[/MiscUserDefs]

//==============================================================================
GlobalEditor::GlobalEditor ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    lfoSpeed.reset (new juce::Slider ("lfoSpeed"));
    addAndMakeVisible (lfoSpeed.get());
    lfoSpeed->setRange (0, 99, 1);
    lfoSpeed->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lfoSpeed->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    lfoSpeed->addListener (this);

    lfoSpeed->setBounds (564, 50, 34, 34);

    lfoAmDepth.reset (new juce::Slider ("lfoAmDepth"));
    addAndMakeVisible (lfoAmDepth.get());
    lfoAmDepth->setRange (0, 99, 1);
    lfoAmDepth->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lfoAmDepth->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    lfoAmDepth->addListener (this);

    lfoAmDepth->setBounds (686, 50, 34, 34);

    lfoPitchDepth.reset (new juce::Slider ("lfoPitchDepth"));
    addAndMakeVisible (lfoPitchDepth.get());
    lfoPitchDepth->setRange (0, 99, 1);
    lfoPitchDepth->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lfoPitchDepth->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    lfoPitchDepth->addListener (this);

    lfoPitchDepth->setBounds (646, 50, 34, 34);

    lfoDelay.reset (new juce::Slider ("lfoDelay"));
    addAndMakeVisible (lfoDelay.get());
    lfoDelay->setRange (0, 99, 1);
    lfoDelay->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lfoDelay->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    lfoDelay->addListener (this);

    lfoDelay->setBounds (603, 50, 34, 34);

    cutoff.reset (new juce::Slider ("cutoff"));
    addAndMakeVisible (cutoff.get());
    cutoff->setRange (0, 1, 0);
    cutoff->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    cutoff->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    cutoff->addListener (this);

    cutoff->setBounds (234, 9, 34, 34);

    reso.reset (new juce::Slider ("reso"));
    addAndMakeVisible (reso.get());
    reso->setRange (0, 1, 0);
    reso->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    reso->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    reso->addListener (this);

    reso->setBounds (278, 9, 34, 34);

    pitchRate2.reset (new juce::Slider ("pitchRate2"));
    addAndMakeVisible (pitchRate2.get());
    pitchRate2->setRange (0, 99, 1);
    pitchRate2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRate2->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchRate2->addListener (this);

    pitchRate2->setBounds (767, 96, 34, 34);

    pitchRate3.reset (new juce::Slider ("pitchRate3"));
    addAndMakeVisible (pitchRate3.get());
    pitchRate3->setRange (0, 99, 1);
    pitchRate3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRate3->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchRate3->addListener (this);

    pitchRate3->setBounds (795, 96, 35, 34);

    pitchRate4.reset (new juce::Slider ("pitchRate4"));
    addAndMakeVisible (pitchRate4.get());
    pitchRate4->setRange (0, 99, 1);
    pitchRate4->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRate4->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchRate4->addListener (this);

    pitchRate4->setBounds (823, 96, 34, 34);

    pitchRate1.reset (new juce::Slider ("pitchRate1"));
    addAndMakeVisible (pitchRate1.get());
    pitchRate1->setRange (0, 99, 1);
    pitchRate1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRate1->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchRate1->addListener (this);

    pitchRate1->setBounds (739, 96, 34, 34);

    pitchLevel2.reset (new juce::Slider ("pitchLevel2"));
    addAndMakeVisible (pitchLevel2.get());
    pitchLevel2->setRange (0, 99, 1);
    pitchLevel2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchLevel2->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchLevel2->addListener (this);

    pitchLevel2->setBounds (767, 57, 34, 34);

    pitchLevel3.reset (new juce::Slider ("pitchLevel3"));
    addAndMakeVisible (pitchLevel3.get());
    pitchLevel3->setRange (0, 99, 1);
    pitchLevel3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchLevel3->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchLevel3->addListener (this);

    pitchLevel3->setBounds (795, 56, 34, 34);

    pitchLevel4.reset (new juce::Slider ("pitchLevel4"));
    addAndMakeVisible (pitchLevel4.get());
    pitchLevel4->setRange (0, 99, 1);
    pitchLevel4->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchLevel4->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchLevel4->addListener (this);

    pitchLevel4->setBounds (823, 56, 34, 34);

    pitchLevel1.reset (new juce::Slider ("pitchLevel1"));
    addAndMakeVisible (pitchLevel1.get());
    pitchLevel1->setRange (0, 99, 1);
    pitchLevel1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchLevel1->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchLevel1->addListener (this);

    pitchLevel1->setBounds (739, 57, 34, 34);

    transpose.reset (new juce::Slider ("transpose"));
    addAndMakeVisible (transpose.get());
    transpose->setRange (0, 48, 1);
    transpose->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    transpose->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    transpose->addListener (this);

    transpose->setBounds (202, 60, 34, 34);

    oscSync.reset (new juce::ToggleButton ("oscSync"));
    addAndMakeVisible (oscSync.get());
    oscSync->setButtonText (juce::String());
    oscSync->addListener (this);

    oscSync->setBounds (650, 96, 48, 26);

    pitchModSens.reset (new juce::Slider ("pitchModSens"));
    addAndMakeVisible (pitchModSens.get());
    pitchModSens->setRange (0, 7, 1);
    pitchModSens->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchModSens->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchModSens->addListener (this);

    pitchModSens->setBounds (666, 5, 34, 34);

    lfoSync.reset (new juce::ToggleButton ("lfoSync"));
    addAndMakeVisible (lfoSync.get());
    lfoSync->setButtonText (juce::String());
    lfoSync->addListener (this);

    lfoSync->setBounds (565, 96, 48, 26);

    pitchEnvDisplay.reset (new PitchEnvDisplay());
    addAndMakeVisible (pitchEnvDisplay.get());
    pitchEnvDisplay->setName ("pitchEnvDisplay");

    pitchEnvDisplay->setBounds (751, 10, 93, 30);

    algoDisplay.reset (new AlgoDisplay());
    addAndMakeVisible (algoDisplay.get());
    algoDisplay->setName ("algoDisplay");

    algoDisplay->setBounds (335, 30, 152, 91);

    feedback.reset (new juce::Slider ("feedback"));
    addAndMakeVisible (feedback.get());
    feedback->setRange (0, 7, 1);
    feedback->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    feedback->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    feedback->addListener (this);

    feedback->setBounds (501, 81, 34, 34);

    algo.reset (new juce::Slider ("algo"));
    addAndMakeVisible (algo.get());
    algo->setRange (1, 32, 1);
    algo->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    algo->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    algo->addListener (this);

    algo->setBounds (501, 22, 34, 34);

    lcdDisplay.reset (new LcdDisplay());
    addAndMakeVisible (lcdDisplay.get());
    lcdDisplay->setName ("lcdDisplay");

    lcdDisplay->setBounds (6, 87, 140, 13);

    output.reset (new juce::Slider ("output"));
    addAndMakeVisible (output.get());
    output->setRange (0, 1, 0);
    output->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    output->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    output->addListener (this);

    output->setBounds (157, 60, 34, 34);

    vuOutput.reset (new VuMeter());
    addAndMakeVisible (vuOutput.get());
    vuOutput->setName ("vuOutput");

    vuOutput->setBounds (6, 103, 140, 8);

    initButton.reset (new juce::TextButton ("initButton"));
    addAndMakeVisible (initButton.get());
    initButton->setButtonText (TRANS("INIT"));
    initButton->addListener (this);

    initButton->setBounds (100, 111, 50, 30);

    parmButton.reset (new juce::TextButton ("parmButton"));
    addAndMakeVisible (parmButton.get());
    parmButton->setButtonText (TRANS("PARM"));
    parmButton->addListener (this);

    parmButton->setBounds (52, 111, 50, 30);

    cartButton.reset (new juce::TextButton ("cartButton"));
    addAndMakeVisible (cartButton.get());
    cartButton->setButtonText (TRANS("CART"));
    cartButton->addListener (this);

    cartButton->setBounds (3, 111, 50, 30);

    storeButton.reset (new juce::TextButton ("storeButton"));
    addAndMakeVisible (storeButton.get());
    storeButton->setButtonText (TRANS("STORE"));
    storeButton->addListener (this);

    storeButton->setBounds (270, 109, 50, 30);

    monoMode.reset (new juce::ToggleButton ("monoMode"));
    addAndMakeVisible (monoMode.get());
    monoMode->setButtonText (juce::String());
    monoMode->addListener (this);

    monoMode->setBounds (249, 65, 48, 26);

    lfoType.reset (new ComboBoxImage());
    addAndMakeVisible (lfoType.get());
    lfoType->setName ("lfoType");

    lfoType->setBounds (583, 8, 36, 26);

    programSelector.reset (new ProgramSelector());
    addAndMakeVisible (programSelector.get());
    programSelector->setName ("programSelector");

    programSelector->setBounds (153, 115, 112, 18);

    aboutButton.reset (new juce::ImageButton ("aboutButton"));
    addAndMakeVisible (aboutButton.get());
    aboutButton->setButtonText (juce::String());
    aboutButton->addListener (this);

    aboutButton->setImages (false, true, false,
                            juce::Image(), 1.000f, juce::Colour (0x00000000),
                            juce::Image(), 1.000f, juce::Colour (0x00000000),
                            juce::Image(), 1.000f, juce::Colour (0x00000000));
    aboutButton->setBounds (8, 11, 135, 46);

    tune.reset (new juce::Slider ("tune"));
    addAndMakeVisible (tune.get());
    tune->setRange (0, 1, 0);
    tune->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    tune->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    tune->addListener (this);

    tune->setBounds (190, 9, 34, 34);


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

    programs = programSelector.get();

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
void GlobalEditor::paint (juce::Graphics& g)
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

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GlobalEditor::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == lfoSpeed.get())
    {
        //[UserSliderCode_lfoSpeed] -- add your slider handling code here..
        //[/UserSliderCode_lfoSpeed]
    }
    else if (sliderThatWasMoved == lfoAmDepth.get())
    {
        //[UserSliderCode_lfoAmDepth] -- add your slider handling code here..
        //[/UserSliderCode_lfoAmDepth]
    }
    else if (sliderThatWasMoved == lfoPitchDepth.get())
    {
        //[UserSliderCode_lfoPitchDepth] -- add your slider handling code here..
        //[/UserSliderCode_lfoPitchDepth]
    }
    else if (sliderThatWasMoved == lfoDelay.get())
    {
        //[UserSliderCode_lfoDelay] -- add your slider handling code here..
        //[/UserSliderCode_lfoDelay]
    }
    else if (sliderThatWasMoved == cutoff.get())
    {
        //[UserSliderCode_cutoff] -- add your slider handling code here..
        //[/UserSliderCode_cutoff]
    }
    else if (sliderThatWasMoved == reso.get())
    {
        //[UserSliderCode_reso] -- add your slider handling code here..
        //[/UserSliderCode_reso]
    }
    else if (sliderThatWasMoved == pitchRate2.get())
    {
        //[UserSliderCode_pitchRate2] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchRate2]
    }
    else if (sliderThatWasMoved == pitchRate3.get())
    {
        //[UserSliderCode_pitchRate3] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchRate3]
    }
    else if (sliderThatWasMoved == pitchRate4.get())
    {
        //[UserSliderCode_pitchRate4] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchRate4]
    }
    else if (sliderThatWasMoved == pitchRate1.get())
    {
        //[UserSliderCode_pitchRate1] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchRate1]
    }
    else if (sliderThatWasMoved == pitchLevel2.get())
    {
        //[UserSliderCode_pitchLevel2] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchLevel2]
    }
    else if (sliderThatWasMoved == pitchLevel3.get())
    {
        //[UserSliderCode_pitchLevel3] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchLevel3]
    }
    else if (sliderThatWasMoved == pitchLevel4.get())
    {
        //[UserSliderCode_pitchLevel4] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchLevel4]
    }
    else if (sliderThatWasMoved == pitchLevel1.get())
    {
        //[UserSliderCode_pitchLevel1] -- add your slider handling code here..
        pitchEnvDisplay->repaint();
        //[/UserSliderCode_pitchLevel1]
    }
    else if (sliderThatWasMoved == transpose.get())
    {
        //[UserSliderCode_transpose] -- add your slider handling code here..
        //[/UserSliderCode_transpose]
    }
    else if (sliderThatWasMoved == pitchModSens.get())
    {
        //[UserSliderCode_pitchModSens] -- add your slider handling code here..
        //[/UserSliderCode_pitchModSens]
    }
    else if (sliderThatWasMoved == feedback.get())
    {
        //[UserSliderCode_feedback] -- add your slider handling code here..
        //[/UserSliderCode_feedback]
    }
    else if (sliderThatWasMoved == algo.get())
    {
        //[UserSliderCode_algo] -- add your slider handling code here..
        algoDisplay->repaint();
        //[/UserSliderCode_algo]
    }
    else if (sliderThatWasMoved == output.get())
    {
        //[UserSliderCode_output] -- add your slider handling code here..
        //[/UserSliderCode_output]
    }
    else if (sliderThatWasMoved == tune.get())
    {
        //[UserSliderCode_tune] -- add your slider handling code here..
        //[/UserSliderCode_tune]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void GlobalEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == oscSync.get())
    {
        //[UserButtonCode_oscSync] -- add your button handler code here..
        repaint();
        //[/UserButtonCode_oscSync]
    }
    else if (buttonThatWasClicked == lfoSync.get())
    {
        //[UserButtonCode_lfoSync] -- add your button handler code here..
        repaint();
        //[/UserButtonCode_lfoSync]
    }
    else if (buttonThatWasClicked == initButton.get())
    {
        //[UserButtonCode_initButton] -- add your button handler code here..
        editor->initProgram();
        //[/UserButtonCode_initButton]
    }
    else if (buttonThatWasClicked == parmButton.get())
    {
        //[UserButtonCode_parmButton] -- add your button handler code here..
        editor->parmShow();
        //[/UserButtonCode_parmButton]
    }
    else if (buttonThatWasClicked == cartButton.get())
    {
        //[UserButtonCode_cartButton] -- add your button handler code here..
        editor->cartShow();
        //[/UserButtonCode_cartButton]
    }
    else if (buttonThatWasClicked == storeButton.get())
    {
        //[UserButtonCode_storeButton] -- add your button handler code here..
        editor->storeProgram();
        //[/UserButtonCode_storeButton]
    }
    else if (buttonThatWasClicked == monoMode.get())
    {
        //[UserButtonCode_monoMode] -- add your button handler code here..
        editor->processor->setMonoMode(monoMode->getToggleState());
        repaint();
        //[/UserButtonCode_monoMode]
    }
    else if (buttonThatWasClicked == aboutButton.get())
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
	processor->algo->bind(algo.get());
	processor->lfoRate->bind(lfoSpeed.get());
	processor->lfoDelay->bind(lfoDelay.get());
	processor->lfoWaveform->bind(lfoType.get());
	processor->lfoAmpDepth->bind(lfoAmDepth.get());
	processor->lfoPitchDepth->bind(lfoPitchDepth.get());
    processor->lfoSync->bind(lfoSync.get());
    processor->oscSync->bind(oscSync.get());
    processor->transpose->bind(transpose.get());
    processor->feedback->bind(feedback.get());
    processor->pitchModSens->bind(pitchModSens.get());
    processor->pitchEgLevel[0]->bind(pitchLevel1.get());
    processor->pitchEgLevel[1]->bind(pitchLevel2.get());
    processor->pitchEgLevel[2]->bind(pitchLevel3.get());
    processor->pitchEgLevel[3]->bind(pitchLevel4.get());
    processor->pitchEgRate[0]->bind(pitchRate1.get());
    processor->pitchEgRate[1]->bind(pitchRate2.get());
    processor->pitchEgRate[2]->bind(pitchRate3.get());
    processor->pitchEgRate[3]->bind(pitchRate4.get());
    processor->fxCutoff->bind(cutoff.get());
    processor->fxReso->bind(reso.get());
    processor->output->bind(output.get());
    processor->tune->bind(tune.get());
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
    lcdDisplay->repaint();
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
    if ( e.mods.isPopupMenu()) {
        PopupMenu popup;
        popup.addItem(1, "Send current program to DX7");

        auto p = popup.show();
        switch( p )
        {
        case 1:
           processor->sendCurrentSysexProgram();
           break;
        default:
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

<JUCER_COMPONENT documentType="Component" className="GlobalEditor" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="0" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="864" initialHeight="144">
  <BACKGROUND backgroundColour="ffffff"/>
  <SLIDER name="lfoSpeed" id="b10eaf327ab3bff5" memberName="lfoSpeed" virtualName=""
          explicitFocusOrder="0" pos="564 50 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="lfoAmDepth" id="3e95a6206fa4a891" memberName="lfoAmDepth"
          virtualName="" explicitFocusOrder="0" pos="686 50 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="lfoPitchDepth" id="6ead769ca786c813" memberName="lfoPitchDepth"
          virtualName="" explicitFocusOrder="0" pos="646 50 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="lfoDelay" id="1fce68dc81619ef5" memberName="lfoDelay" virtualName=""
          explicitFocusOrder="0" pos="603 50 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="cutoff" id="40531f16bb0bd225" memberName="cutoff" virtualName=""
          explicitFocusOrder="0" pos="234 9 34 34" min="0.0" max="1.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="reso" id="c8c13464e81a8d83" memberName="reso" virtualName=""
          explicitFocusOrder="0" pos="278 9 34 34" min="0.0" max="1.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="pitchRate2" id="73f386b3c91d3de4" memberName="pitchRate2"
          virtualName="" explicitFocusOrder="0" pos="767 96 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchRate3" id="187c25b24413fccf" memberName="pitchRate3"
          virtualName="" explicitFocusOrder="0" pos="795 96 35 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchRate4" id="1aeb2a8fbbcbeaab" memberName="pitchRate4"
          virtualName="" explicitFocusOrder="0" pos="823 96 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchRate1" id="419d613b3fb9604e" memberName="pitchRate1"
          virtualName="" explicitFocusOrder="0" pos="739 96 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchLevel2" id="3a355ad2221887d9" memberName="pitchLevel2"
          virtualName="" explicitFocusOrder="0" pos="767 57 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchLevel3" id="5c5d782e8a7f3ad7" memberName="pitchLevel3"
          virtualName="" explicitFocusOrder="0" pos="795 56 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchLevel4" id="eb900b141fbad8ff" memberName="pitchLevel4"
          virtualName="" explicitFocusOrder="0" pos="823 56 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchLevel1" id="344cbe26ec9fa128" memberName="pitchLevel1"
          virtualName="" explicitFocusOrder="0" pos="739 57 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="transpose" id="7d1266b1c1534947" memberName="transpose"
          virtualName="" explicitFocusOrder="0" pos="202 60 34 34" min="0.0"
          max="48.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="oscSync" id="8f3fe641537cd00" memberName="oscSync" virtualName=""
                explicitFocusOrder="0" pos="650 96 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="pitchModSens" id="904f73df85a9f886" memberName="pitchModSens"
          virtualName="" explicitFocusOrder="0" pos="666 5 34 34" min="0.0"
          max="7.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
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
          explicitFocusOrder="0" pos="501 81 34 34" min="0.0" max="7.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="algo" id="8a226ddf9bbff752" memberName="algo" virtualName=""
          explicitFocusOrder="0" pos="501 22 34 34" min="1.0" max="32.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="lcdDisplay" id="30c7bb8f114cbbe3" memberName="lcdDisplay"
                    virtualName="" explicitFocusOrder="0" pos="6 87 140 13" class="LcdDisplay"
                    params=""/>
  <SLIDER name="output" id="7697fdd54fd1593e" memberName="output" virtualName=""
          explicitFocusOrder="0" pos="157 60 34 34" min="0.0" max="1.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
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
               resourceNormal="" opacityNormal="1.0" colourNormal="0" resourceOver=""
               opacityOver="1.0" colourOver="0" resourceDown="" opacityDown="1.0"
               colourDown="0"/>
  <SLIDER name="tune" id="d22c34aa3363a28a" memberName="tune" virtualName=""
          explicitFocusOrder="0" pos="190 9 34 34" min="0.0" max="1.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

