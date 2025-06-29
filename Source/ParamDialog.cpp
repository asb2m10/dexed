/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.11

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "Dexed.h"
#include "DXComponents.h"
//[/Headers]

#include "ParamDialog.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
ParamDialog::ParamDialog ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    pitchRangeDn.reset (new juce::Slider ("pitchRangeDn"));
    addAndMakeVisible (pitchRangeDn.get());
    pitchRangeDn->setExplicitFocusOrder (2);
    pitchRangeDn->setRange (0, 48, 1);
    pitchRangeDn->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRangeDn->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    pitchRangeDn->addListener (this);

    pitchRangeDn->setBounds (264, 16, 72, 24);

    pitchStep.reset (new juce::Slider ("pitchStep"));
    addAndMakeVisible (pitchStep.get());
    pitchStep->setExplicitFocusOrder (3);
    pitchStep->setRange (0, 12, 1);
    pitchStep->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchStep->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    pitchStep->addListener (this);

    pitchStep->setBounds (264, 56, 72, 24);

    sysexIn.reset (new juce::ComboBox ("sysexIn"));
    addAndMakeVisible (sysexIn.get());
    sysexIn->setExplicitFocusOrder (7);
    sysexIn->setEditableText (false);
    sysexIn->setJustificationType (juce::Justification::centredLeft);
    sysexIn->setTextWhenNothingSelected (juce::String());
    sysexIn->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    sysexIn->addListener (this);

    sysexIn->setBounds (104, 244, 224, 24);

    sysexOut.reset (new juce::ComboBox ("sysexOut"));
    addAndMakeVisible (sysexOut.get());
    sysexOut->setExplicitFocusOrder (8);
    sysexOut->setEditableText (false);
    sysexOut->setJustificationType (juce::Justification::centredLeft);
    sysexOut->setTextWhenNothingSelected (juce::String());
    sysexOut->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    sysexOut->addListener (this);

    sysexOut->setBounds (104, 280, 224, 24);

    sysexChl.reset (new juce::Slider ("sysexChl"));
    addAndMakeVisible (sysexChl.get());
    sysexChl->setExplicitFocusOrder (9);
    sysexChl->setRange (1, 16, 1);
    sysexChl->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sysexChl->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    sysexChl->addListener (this);

    sysexChl->setBounds (252, 320, 72, 24);

    engineReso.reset (new juce::ComboBox ("new combo box"));
    addAndMakeVisible (engineReso.get());
    engineReso->setExplicitFocusOrder (6);
    engineReso->setEditableText (false);
    engineReso->setJustificationType (juce::Justification::centredLeft);
    engineReso->setTextWhenNothingSelected (juce::String());
    engineReso->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    engineReso->addItem (TRANS ("Modern (24-bit)"), 1);
    engineReso->addItem (TRANS ("Mark I"), 2);
    engineReso->addItem (TRANS ("OPL Series"), 3);
    engineReso->addListener (this);

    engineReso->setBounds (160, 188, 168, 24);

    showKeyboard.reset (new LightedToggleButton ("showKeyboard"));
    addAndMakeVisible (showKeyboard.get());
    showKeyboard->setExplicitFocusOrder (4);
    showKeyboard->setButtonText (juce::String());
    showKeyboard->addListener (this);

    showKeyboard->setBounds (264, 96, 56, 24);

    whlRange.reset (new juce::Slider ("whlRange"));
    addAndMakeVisible (whlRange.get());
    whlRange->setExplicitFocusOrder (10);
    whlRange->setRange (0, 99, 1);
    whlRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    whlRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    whlRange->addListener (this);

    whlRange->setBounds (448, 16, 72, 24);

    ftRange.reset (new juce::Slider ("ftRange"));
    addAndMakeVisible (ftRange.get());
    ftRange->setExplicitFocusOrder (14);
    ftRange->setRange (0, 99, 1);
    ftRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    ftRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    ftRange->addListener (this);

    ftRange->setBounds (448, 56, 72, 24);

    brRange.reset (new juce::Slider ("brRange"));
    addAndMakeVisible (brRange.get());
    brRange->setExplicitFocusOrder (18);
    brRange->setRange (0, 99, 1);
    brRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    brRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    brRange->addListener (this);

    brRange->setBounds (448, 96, 72, 24);

    atRange.reset (new juce::Slider ("atRange"));
    addAndMakeVisible (atRange.get());
    atRange->setExplicitFocusOrder (22);
    atRange->setRange (0, 99, 1);
    atRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    atRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    atRange->addListener (this);

    atRange->setBounds (448, 136, 72, 24);

    whlEg.reset (new LightedToggleButton ("whlEg"));
    addAndMakeVisible (whlEg.get());
    whlEg->setExplicitFocusOrder (13);
    whlEg->setButtonText (juce::String());
    whlEg->addListener (this);

    whlEg->setBounds (640, 16, 56, 24);

    ftEg.reset (new LightedToggleButton ("ftEg"));
    addAndMakeVisible (ftEg.get());
    ftEg->setExplicitFocusOrder (17);
    ftEg->setButtonText (juce::String());
    ftEg->addListener (this);

    ftEg->setBounds (640, 56, 56, 24);

    brEg.reset (new LightedToggleButton ("brEg"));
    addAndMakeVisible (brEg.get());
    brEg->setExplicitFocusOrder (21);
    brEg->setButtonText (juce::String());
    brEg->addListener (this);

    brEg->setBounds (640, 96, 56, 24);

    atEg.reset (new LightedToggleButton ("atEg"));
    addAndMakeVisible (atEg.get());
    atEg->setExplicitFocusOrder (25);
    atEg->setButtonText (juce::String());
    atEg->addListener (this);

    atEg->setBounds (640, 136, 56, 24);

    whlAmp.reset (new LightedToggleButton ("whlAmp"));
    addAndMakeVisible (whlAmp.get());
    whlAmp->setExplicitFocusOrder (12);
    whlAmp->setButtonText (juce::String());
    whlAmp->addListener (this);

    whlAmp->setBounds (584, 16, 56, 24);

    ftAmp.reset (new LightedToggleButton ("ftAmp"));
    addAndMakeVisible (ftAmp.get());
    ftAmp->setExplicitFocusOrder (16);
    ftAmp->setButtonText (juce::String());
    ftAmp->addListener (this);

    ftAmp->setBounds (584, 56, 56, 24);

    brAmp.reset (new LightedToggleButton ("brAmp"));
    addAndMakeVisible (brAmp.get());
    brAmp->setExplicitFocusOrder (20);
    brAmp->setButtonText (juce::String());
    brAmp->addListener (this);

    brAmp->setBounds (584, 96, 56, 24);

    atAmp.reset (new LightedToggleButton ("atAmp"));
    addAndMakeVisible (atAmp.get());
    atAmp->setExplicitFocusOrder (24);
    atAmp->setButtonText (juce::String());
    atAmp->addListener (this);

    atAmp->setBounds (584, 136, 56, 24);

    whlPitch.reset (new LightedToggleButton ("whlPitch"));
    addAndMakeVisible (whlPitch.get());
    whlPitch->setExplicitFocusOrder (11);
    whlPitch->setButtonText (juce::String());
    whlPitch->addListener (this);

    whlPitch->setBounds (528, 16, 56, 24);

    ftPitch.reset (new LightedToggleButton ("ftPitch"));
    addAndMakeVisible (ftPitch.get());
    ftPitch->setExplicitFocusOrder (15);
    ftPitch->setButtonText (juce::String());
    ftPitch->addListener (this);

    ftPitch->setBounds (528, 56, 56, 24);

    brPitch.reset (new LightedToggleButton ("brPitch"));
    addAndMakeVisible (brPitch.get());
    brPitch->setExplicitFocusOrder (19);
    brPitch->setButtonText (juce::String());
    brPitch->addListener (this);

    brPitch->setBounds (528, 96, 56, 24);

    atPitch.reset (new LightedToggleButton ("atPitch"));
    addAndMakeVisible (atPitch.get());
    atPitch->setExplicitFocusOrder (23);
    atPitch->setButtonText (juce::String());
    atPitch->addListener (this);

    atPitch->setBounds (528, 136, 56, 24);

    sclButton.reset (new juce::TextButton ("scl button"));
    addAndMakeVisible (sclButton.get());
    sclButton->setExplicitFocusOrder (26);
    sclButton->setButtonText (TRANS ("SCL"));
    sclButton->addListener (this);

    sclButton->setBounds (448, 205, 56, 30);

    kbmButton.reset (new juce::TextButton ("kbm button"));
    addAndMakeVisible (kbmButton.get());
    kbmButton->setExplicitFocusOrder (27);
    kbmButton->setButtonText (TRANS ("KBM"));
    kbmButton->addListener (this);

    kbmButton->setBounds (512, 205, 56, 30);

    showTunButton.reset (new juce::TextButton ("show tuning button"));
    addAndMakeVisible (showTunButton.get());
    showTunButton->setExplicitFocusOrder (28);
    showTunButton->setButtonText (TRANS ("Show"));
    showTunButton->addListener (this);

    showTunButton->setBounds (576, 205, 48, 30);

    resetTuningButton.reset (new juce::TextButton ("reset tuning button"));
    addAndMakeVisible (resetTuningButton.get());
    resetTuningButton->setExplicitFocusOrder (29);
    resetTuningButton->setButtonText (TRANS ("Reset"));
    resetTuningButton->addListener (this);

    resetTuningButton->setBounds (632, 205, 48, 30);

    transposeScale.reset (new LightedToggleButton ("transposeScale"));
    addAndMakeVisible (transposeScale.get());
    transposeScale->setExplicitFocusOrder (30);
    transposeScale->setButtonText (juce::String());
    transposeScale->addListener (this);
    transposeScale->setToggleState (true, juce::dontSendNotification);

    transposeScale->setBounds (576, 240, 56, 30);

    mpePBRange.reset (new juce::Slider ("mpePBRange"));
    addAndMakeVisible (mpePBRange.get());
    mpePBRange->setExplicitFocusOrder (32);
    mpePBRange->setRange (0, 96, 1);
    mpePBRange->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    mpePBRange->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    mpePBRange->addListener (this);

    mpePBRange->setBounds (616, 319, 72, 24);

    mpeEnabled.reset (new LightedToggleButton ("mpeEnabled"));
    addAndMakeVisible (mpeEnabled.get());
    mpeEnabled->setExplicitFocusOrder (31);
    mpeEnabled->setButtonText (juce::String());
    mpeEnabled->addListener (this);

    mpeEnabled->setBounds (456, 320, 48, 30);

    transposeHelp.reset (new juce::ImageButton ("new button"));
    addAndMakeVisible (transposeHelp.get());
    transposeHelp->addListener (this);

    transposeHelp->setImages (false, true, true,
                              juce::ImageCache::getFromMemory (BinaryData::HelpButton_png, BinaryData::HelpButton_pngSize), 1.000f, juce::Colour (0x00000000),
                              juce::Image(), 1.000f, juce::Colour (0x00000000),
                              juce::Image(), 1.000f, juce::Colour (0x00000000));
    transposeHelp->setBounds (500, 245, 20, 20);

    pitchRangeUp.reset (new juce::Slider ("pitchRangeUp"));
    addAndMakeVisible (pitchRangeUp.get());
    pitchRangeUp->setExplicitFocusOrder (1);
    pitchRangeUp->setRange (0, 48, 1);
    pitchRangeUp->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRangeUp->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    pitchRangeUp->addListener (this);

    pitchRangeUp->setBounds (168, 16, 72, 24);

    scalingFactor.reset (new juce::ComboBox ("scalingFactor"));
    addAndMakeVisible (scalingFactor.get());
    scalingFactor->setExplicitFocusOrder (5);
    scalingFactor->setEditableText (false);
    scalingFactor->setJustificationType (juce::Justification::centredLeft);
    scalingFactor->setTextWhenNothingSelected (juce::String());
    scalingFactor->setTextWhenNoChoicesAvailable (TRANS ("(no choices)"));
    scalingFactor->addItem (TRANS ("100 %"), 1);
    scalingFactor->addItem (TRANS ("125 %"), 2);
    scalingFactor->addItem (TRANS ("150 %"), 3);
    scalingFactor->addItem (TRANS ("200 %"), 4);
    scalingFactor->addItem (TRANS ("300 %"), 5);
    scalingFactor->addItem (TRANS ("400 %"), 6);
    scalingFactor->addListener (this);

    scalingFactor->setBounds (236, 136, 90, 24);

    glissando.reset (new LightedToggleButton ("glissando"));
    addAndMakeVisible (glissando.get());
    glissando->setExplicitFocusOrder (30);
    glissando->setButtonText (juce::String());
    glissando->setTooltip ("Enable or disable glissando effect.");
    glissando->addListener (this);
    glissando->setToggleState (true, juce::dontSendNotification);

    glissando->setBounds (576, 276, 56, 30);

    portamentoTm.reset (new juce::Slider ("portamentoTm"));
    addAndMakeVisible (portamentoTm.get());
    portamentoTm->setExplicitFocusOrder (22);
    portamentoTm->setRange (0, 99, 1);
    portamentoTm->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    portamentoTm->setTooltip ("Adjust the portamento time.");
    portamentoTm->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    portamentoTm->addListener (this);

    portamentoTm->setBounds (504, 276, 72, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (710, 370);


    //[Constructor] You can add your own custom stuff here..
    pitchRangeUp->setEnabled(pitchStep->getValue() == 0);
    pitchRangeDn->setEnabled(pitchStep->getValue() == 0);

    StringArray input;
    input.add("None");
    input.addArray(MidiInput::getDevices());
    sysexIn->addItemList(input, 2);

    StringArray output;
    output.add("None");
    output.addArray(MidiOutput::getDevices());
    sysexOut->addItemList(output, 2);

    if ( JUCEApplication::isStandaloneApp() ) {
        sysexIn->setVisible(false);
    }

    // ACCESSIBLITY
    pitchRangeUp->setTitle("Pitch Bend Range Up");
    pitchRangeDn->setTitle("Pitch Bend Range Down");
    pitchStep->setTitle("Pitch Bend Step");
    sysexIn->setTitle("Sysex Input");
    sysexOut->setTitle("Sysex Output");
    sysexChl->setTitle("Sysex Channel");
    engineReso->setTitle("Engine Resolution");
    showKeyboard->setTitle("Show Keyboard");
    whlRange->setTitle("Wheel Range");
    ftRange->setTitle("Foot Range");
    brRange->setTitle("Breath Range");
    atRange->setTitle("After Touch Range");
    whlEg->setTitle("Wheel EG");
    ftEg->setTitle("Foot EG");
    brEg->setTitle("Breath EG");
    atEg->setTitle("After Touch EG");
    whlAmp->setTitle("Wheel Amp");
    ftAmp->setTitle("Foot Amp");
    brAmp->setTitle("Breath Amp");
    atAmp->setTitle("After Touch Amp");
    whlPitch->setTitle("Wheel Pitch");
    ftPitch->setTitle("Foot Pitch");
    brPitch->setTitle("Breath Pitch");
    atPitch->setTitle("After Touch Pitch");
    sclButton->setTitle("Scale Button");
    kbmButton->setTitle("Keyboard Mapping Button");
    showTunButton->setTitle("Show Tuning Button");
    resetTuningButton->setTitle("Reset Tuning Button");
    transposeScale->setTitle("Transpose Scale");
    mpePBRange->setTitle("MPE Pitch Bend Range");
    mpeEnabled->setTitle("MPE Enabled");
    transposeHelp->setTitle("Transpose Help");
    scalingFactor->setTitle("Scaling Factor");

    pitchRangeUp->setWantsKeyboardFocus(true);
    pitchRangeDn->setWantsKeyboardFocus(true);
    pitchStep->setWantsKeyboardFocus(true);
    sysexIn->setWantsKeyboardFocus(true);
    sysexOut->setWantsKeyboardFocus(true);
    sysexChl->setWantsKeyboardFocus(true);
    engineReso->setWantsKeyboardFocus(true);
    showKeyboard->setWantsKeyboardFocus(true);
    whlRange->setWantsKeyboardFocus(true);
    ftRange->setWantsKeyboardFocus(true);
    brRange->setWantsKeyboardFocus(true);
    atRange->setWantsKeyboardFocus(true);
    whlEg->setWantsKeyboardFocus(true);
    ftEg->setWantsKeyboardFocus(true);
    brEg->setWantsKeyboardFocus(true);
    atEg->setWantsKeyboardFocus(true);
    whlAmp->setWantsKeyboardFocus(true);
    ftAmp->setWantsKeyboardFocus(true);
    brAmp->setWantsKeyboardFocus(true);
    atAmp->setWantsKeyboardFocus(true);
    whlPitch->setWantsKeyboardFocus(true);
    ftPitch->setWantsKeyboardFocus(true);
    brPitch->setWantsKeyboardFocus(true);
    atPitch->setWantsKeyboardFocus(true);
    sclButton->setWantsKeyboardFocus(true);
    kbmButton->setWantsKeyboardFocus(true);
    showTunButton->setWantsKeyboardFocus(true);
    resetTuningButton->setWantsKeyboardFocus(true);
    transposeScale->setWantsKeyboardFocus(true);
    mpePBRange->setWantsKeyboardFocus(true);
    mpeEnabled->setWantsKeyboardFocus(true);
    transposeHelp->setWantsKeyboardFocus(true);
    scalingFactor->setWantsKeyboardFocus(true);

    setWantsKeyboardFocus(true);
    startTimer(100);
    //[/Constructor]
}

ParamDialog::~ParamDialog()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    pitchRangeDn = nullptr;
    pitchStep = nullptr;
    sysexIn = nullptr;
    sysexOut = nullptr;
    sysexChl = nullptr;
    engineReso = nullptr;
    showKeyboard = nullptr;
    whlRange = nullptr;
    ftRange = nullptr;
    brRange = nullptr;
    atRange = nullptr;
    whlEg = nullptr;
    ftEg = nullptr;
    brEg = nullptr;
    atEg = nullptr;
    whlAmp = nullptr;
    ftAmp = nullptr;
    brAmp = nullptr;
    atAmp = nullptr;
    whlPitch = nullptr;
    ftPitch = nullptr;
    brPitch = nullptr;
    atPitch = nullptr;
    sclButton = nullptr;
    kbmButton = nullptr;
    showTunButton = nullptr;
    resetTuningButton = nullptr;
    transposeScale = nullptr;
    mpePBRange = nullptr;
    mpeEnabled = nullptr;
    transposeHelp = nullptr;
    pitchRangeUp = nullptr;
    scalingFactor = nullptr;
    glissando = nullptr;
    portamentoTm = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    stopTimer();
    //[/Destructor]
}

//==============================================================================
void ParamDialog::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff3c322f));

    {
        int x = 20, y = 16, width = 276, height = 23;
        juce::String text (TRANS ("Pitch Bend Range"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 20, y = 56, width = 276, height = 23;
        juce::String text (TRANS ("Pitch Bend Step"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 20, y = 319, width = 245, height = 23;
        juce::String text (TRANS ("DX7 Channel"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 20, y = 190, width = 276, height = 23;
        juce::String text (TRANS ("Engine Resolution"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 22, y = 174, width = 306, height = 1;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 22, y = 227, width = 306, height = 1;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 20, y = 96, width = 276, height = 23;
        juce::String text (TRANS ("Show Keyboard"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 352, y = 12, width = 1, height = 349;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 368, y = 16, width = 276, height = 23;
        juce::String text (TRANS ("Wheel"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 368, y = 96, width = 276, height = 23;
        juce::String text (TRANS ("Breath"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 368, y = 56, width = 276, height = 23;
        juce::String text (TRANS ("Foot"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 368, y = 136, width = 276, height = 23;
        juce::String text (TRANS ("After Touch"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 528, y = 163, width = 48, height = 23;
        juce::String text (TRANS ("PITCH"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 584, y = 163, width = 48, height = 23;
        juce::String text (TRANS ("AMP"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 640, y = 163, width = 48, height = 23;
        juce::String text (TRANS ("EG BIAS"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 376, y = 194, width = 325, height = 1;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 371, y = 208, width = 276, height = 25;
        juce::String text (TRANS ("Tuning"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 371, y = 242, width = 157, height = 25;
        juce::String text (TRANS ("Transposition 12 as:"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 368, y = 310, width = 328, height = 1;
        juce::Colour fillColour = juce::Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 368, y = 318, width = 276, height = 27;
        juce::String text (TRANS ("MPE"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 528, y = 318, width = 119, height = 27;
        juce::String text (TRANS ("Bend Range"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 548, y = 242, width = 37, height = 25;
        juce::String text (TRANS ("12"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 628, y = 242, width = 45, height = 25;
        juce::String text (TRANS ("SCL"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 147, y = 16, width = 20, height = 23;
        juce::String text (TRANS ("up"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 240, y = 16, width = 20, height = 23;
        juce::String text (TRANS ("dn"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 20, y = 136, width = 276, height = 23;
        juce::String text (TRANS ("UI Scaling"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 371, y = 277, width = 184, height = 23;
        juce::String text (TRANS ("Portamento Rate"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    {
        int x = 628, y = 274, width = 75, height = 27;
        juce::String text (TRANS ("Glissando"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centredLeft, true);
    }

    //[UserPaint] Add your own custom painting code here..
    if ( ! JUCEApplication::isStandaloneApp() ) {
        g.setColour (Colours::white);
        g.setFont (Font (15.00f, Font::plain));
        g.drawText (translate("DX7 In"),
                    20, 245, 131, 23,
                    Justification::centredLeft, true);

    }
    g.setColour (Colours::white);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (translate("DX7 Out"),
                20, 280, 131, 23,
                Justification::centredLeft, true);
    //[/UserPaint]
}

void ParamDialog::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ParamDialog::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    bool handled = false;
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == pitchRangeDn.get())
    {
        //[UserSliderCode_pitchRangeDn] -- add your slider handling code here..
        //[/UserSliderCode_pitchRangeDn]
    }
    else if (sliderThatWasMoved == pitchStep.get())
    {
        //[UserSliderCode_pitchStep] -- add your slider handling code here..
        pitchRangeUp->setEnabled(pitchStep->getValue() == 0);
        pitchRangeDn->setEnabled(pitchStep->getValue() == 0);
        //[/UserSliderCode_pitchStep]
    }
    else if (sliderThatWasMoved == sysexChl.get())
    {
        //[UserSliderCode_sysexChl] -- add your slider handling code here..
        //[/UserSliderCode_sysexChl]
    }
    else if (sliderThatWasMoved == whlRange.get())
    {
        //[UserSliderCode_whlRange] -- add your slider handling code here..
        //[/UserSliderCode_whlRange]
    }
    else if (sliderThatWasMoved == ftRange.get())
    {
        //[UserSliderCode_ftRange] -- add your slider handling code here..
        //[/UserSliderCode_ftRange]
    }
    else if (sliderThatWasMoved == brRange.get())
    {
        //[UserSliderCode_brRange] -- add your slider handling code here..
        //[/UserSliderCode_brRange]
    }
    else if (sliderThatWasMoved == atRange.get())
    {
        //[UserSliderCode_atRange] -- add your slider handling code here..
        //[/UserSliderCode_atRange]
    }
    else if (sliderThatWasMoved == mpePBRange.get())
    {
        //[UserSliderCode_mpePBRange] -- add your slider handling code here..
        //[/UserSliderCode_mpePBRange]
    }
    else if (sliderThatWasMoved == pitchRangeUp.get())
    {
        //[UserSliderCode_pitchRangeUp] -- add your slider handling code here..
        //[/UserSliderCode_pitchRangeUp]
    }
    else if (sliderThatWasMoved == portamentoTm.get())
    {
        //[UserSliderCode_portamentoTm] -- add your slider handling code here..
        //[/UserSliderCode_portamentoTm]
    }

    //[UsersliderValueChanged_Post]
    if( ! handled )
        general_callback_(this);
    //[/UsersliderValueChanged_Post]
}

void ParamDialog::comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    bool handled = false;
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == sysexIn.get())
    {
        //[UserComboBoxCode_sysexIn] -- add your combo box handling code here..
        //[/UserComboBoxCode_sysexIn]
    }
    else if (comboBoxThatHasChanged == sysexOut.get())
    {
        //[UserComboBoxCode_sysexOut] -- add your combo box handling code here..
        //[/UserComboBoxCode_sysexOut]
    }
    else if (comboBoxThatHasChanged == engineReso.get())
    {
        //[UserComboBoxCode_engineReso] -- add your combo box handling code here..
        //[/UserComboBoxCode_engineReso]
    }
    else if (comboBoxThatHasChanged == scalingFactor.get())
    {
        //[UserComboBoxCode_scalingFactor] -- add your combo box handling code here..
        //[/UserComboBoxCode_scalingFactor]
    }

    //[UsercomboBoxChanged_Post]
    if( ! handled )
        general_callback_(this);
    //[/UsercomboBoxChanged_Post]
}

void ParamDialog::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    bool handled = false;
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == showKeyboard.get())
    {
        //[UserButtonCode_showKeyboard] -- add your button handler code here..
        //[/UserButtonCode_showKeyboard]
    }
    else if (buttonThatWasClicked == whlEg.get())
    {
        //[UserButtonCode_whlEg] -- add your button handler code here..
        //[/UserButtonCode_whlEg]
    }
    else if (buttonThatWasClicked == ftEg.get())
    {
        //[UserButtonCode_ftEg] -- add your button handler code here..
        //[/UserButtonCode_ftEg]
    }
    else if (buttonThatWasClicked == brEg.get())
    {
        //[UserButtonCode_brEg] -- add your button handler code here..
        //[/UserButtonCode_brEg]
    }
    else if (buttonThatWasClicked == atEg.get())
    {
        //[UserButtonCode_atEg] -- add your button handler code here..
        //[/UserButtonCode_atEg]
    }
    else if (buttonThatWasClicked == whlAmp.get())
    {
        //[UserButtonCode_whlAmp] -- add your button handler code here..
        //[/UserButtonCode_whlAmp]
    }
    else if (buttonThatWasClicked == ftAmp.get())
    {
        //[UserButtonCode_ftAmp] -- add your button handler code here..
        //[/UserButtonCode_ftAmp]
    }
    else if (buttonThatWasClicked == brAmp.get())
    {
        //[UserButtonCode_brAmp] -- add your button handler code here..
        //[/UserButtonCode_brAmp]
    }
    else if (buttonThatWasClicked == atAmp.get())
    {
        //[UserButtonCode_atAmp] -- add your button handler code here..
        //[/UserButtonCode_atAmp]
    }
    else if (buttonThatWasClicked == whlPitch.get())
    {
        //[UserButtonCode_whlPitch] -- add your button handler code here..
        //[/UserButtonCode_whlPitch]
    }
    else if (buttonThatWasClicked == ftPitch.get())
    {
        //[UserButtonCode_ftPitch] -- add your button handler code here..
        //[/UserButtonCode_ftPitch]
    }
    else if (buttonThatWasClicked == brPitch.get())
    {
        //[UserButtonCode_brPitch] -- add your button handler code here..
        //[/UserButtonCode_brPitch]
    }
    else if (buttonThatWasClicked == atPitch.get())
    {
        //[UserButtonCode_atPitch] -- add your button handler code here..
        //[/UserButtonCode_atPitch]
    }
    else if (buttonThatWasClicked == sclButton.get())
    {
        //[UserButtonCode_sclButton] -- add your button handler code here..
        tuning_callback_(this, TuningAction::LOAD_SCL);
        handled = true;
        //[/UserButtonCode_sclButton]
    }
    else if (buttonThatWasClicked == kbmButton.get())
    {
        //[UserButtonCode_kbmButton] -- add your button handler code here..
        tuning_callback_(this, TuningAction::LOAD_KBM);
        handled = true;
        //[/UserButtonCode_kbmButton]
    }
    else if (buttonThatWasClicked == showTunButton.get())
    {
        //[UserButtonCode_showTunButton] -- add your button handler code here..
        tuning_callback_(this, TuningAction::SHOW_TUNING);
        handled = true;
        //[/UserButtonCode_showTunButton]
    }
    else if (buttonThatWasClicked == resetTuningButton.get())
    {
        //[UserButtonCode_resetTuningButton] -- add your button handler code here..
        tuning_callback_(this, TuningAction::RESET_TUNING);
        handled = true;
        //[/UserButtonCode_resetTuningButton]
    }
    else if (buttonThatWasClicked == transposeScale.get())
    {
        //[UserButtonCode_transposeScale] -- add your button handler code here..
        //[/UserButtonCode_transposeScale]
    }
    else if (buttonThatWasClicked == mpeEnabled.get())
    {
        //[UserButtonCode_mpeEnabled] -- add your button handler code here..
        //[/UserButtonCode_mpeEnabled]
    }
    else if (buttonThatWasClicked == transposeHelp.get())
    {
        //[UserButtonCode_transposeHelp] -- add your button handler code here..
        auto te = new TextEditor();
        te->setReadOnly(true);
        te->setMultiLine(true);
        te->setBounds( 0, 0, 250, 200 );
        te->setText( R"TUNINGHELP(
Help on Tuning Transposition

Many DX7 cartridges use a transposition of -12 or -24 to acheive a bass sound. The DX7 transposes in key space, and so this transposition remaps C4 to C3 and so on, leading to an octave drop.

When applying non-12-note scales, though, this transposition results in an irregular retuning. If you have a 15 note scale, a transposition of -12 will move you to the  third note of the scale, not the root.

To allow existing patches to work, we have added a feature which replaces transpositions of -24, -12, 12, and 24 with the length of the scale. This behavior is controlled by the switch here.

With the switch in the SCL (lighted) position, a transposition of -24 is replaced with  -2 scale length, -12 by - scale length, and so on. This results in octave transpositions being scale aware, but has the strange effect that the jump from -11 to -12 to -13 in  transposition space is no longer continuous in frequency space. With an 18 note octave transpose -11 will play note 7, -12 note 0, and -13 note 5.

With the switch in the 12 (unlighted) position, transposition stays with the keyboard. In this case transposition through 11, 12 and 13 is continous, but transposition by 12 is no a single scale length drop.
)TUNINGHELP" );

        DialogWindow::LaunchOptions options;
        options.content.setOwned(te);
        options.dialogTitle = "Transposition Help";
        options.dialogBackgroundColour = Colour(0xFF323E44);
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = false;
        options.resizable = false;

        auto dialogwindow = options.launchAsync();

        //[/UserButtonCode_transposeHelp]
    }
    else if (buttonThatWasClicked == glissando.get())
    {
        //[UserButtonCode_glissando] -- add your button handler code here..
        //[/UserButtonCode_glissando]
    }

    //[UserbuttonClicked_Post]
    if( ! handled )
    {
        general_callback_(this);
    }
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ParamDialog::setDialogValues(Controllers &c, SysexComm &mgr, int reso, bool showKey, float dpiScaleFactor) {
    pitchRangeUp->setValue(c.values_[kControllerPitchRangeUp]);
    pitchRangeDn->setValue(c.values_[kControllerPitchRangeDn]);
    pitchStep->setValue(c.values_[kControllerPitchStep]);
    sysexChl->setValue(mgr.getChl() + 1);

    whlRange->setValue(c.wheel.range);
    whlPitch->setToggleState(c.wheel.pitch, dontSendNotification);
    whlPitch->setToggleState(c.wheel.pitch, dontSendNotification);
    whlAmp->setToggleState(c.wheel.amp, dontSendNotification);
    whlEg->setToggleState(c.wheel.eg, dontSendNotification);

    ftRange->setValue(c.foot.range);
    ftPitch->setToggleState(c.foot.pitch, dontSendNotification);
    ftAmp->setToggleState(c.foot.amp, dontSendNotification);
    ftEg->setToggleState(c.foot.eg, dontSendNotification);

    brRange->setValue(c.breath.range);
    brPitch->setToggleState(c.breath.pitch, dontSendNotification);
    brAmp->setToggleState(c.breath.amp, dontSendNotification);
    brEg->setToggleState(c.breath.eg, dontSendNotification);

    atRange->setValue(c.at.range);
    atPitch->setToggleState(c.at.pitch, dontSendNotification);
    atAmp->setToggleState(c.at.amp, dontSendNotification);
    atEg->setToggleState(c.at.eg, dontSendNotification);

    transposeScale->setToggleState(c.transpose12AsScale ? 1 : 0, dontSendNotification );

    mpeEnabled->setToggleState(c.mpeEnabled, dontSendNotification);
    mpePBRange->setValue(c.mpePitchBendRange, dontSendNotification);

    portamentoTm->setValue(c.portamento_cc * 100.0f / 127.0f); // Convert from 0-127 range to 0-100%
    glissando->setToggleState(c.portamento_gliss_cc, dontSendNotification);

    StringArray inputs = MidiInput::getDevices();
    int idx = inputs.indexOf(mgr.getInput());
    idx = idx == -1 ? 0 : idx + 1;
    sysexIn->setSelectedItemIndex(idx);

    StringArray outputs = MidiOutput::getDevices();
    idx = outputs.indexOf(mgr.getOutput());
    idx = idx == -1 ? 0 : idx + 1;
    sysexOut->setSelectedItemIndex(idx);

    engineReso->setSelectedItemIndex(reso);
    showKeyboard->setToggleState(showKey, NotificationType::dontSendNotification);

    if ( dpiScaleFactor == 1.25 ) {
        scalingFactor->setSelectedItemIndex(1);
    } else if ( dpiScaleFactor == 1.50 ) {
        scalingFactor->setSelectedItemIndex(2);
    }
    else if (dpiScaleFactor == 2.00) {
        scalingFactor->setSelectedItemIndex(3);
    }
    else if (dpiScaleFactor == 3.00) {
        scalingFactor->setSelectedItemIndex(4);
    }
    else if (dpiScaleFactor == 4.00) {
        scalingFactor->setSelectedItemIndex(5);
    } else {
        scalingFactor->setSelectedItemIndex(0);
    }
}

bool ParamDialog::getDialogValues(Controllers &c, SysexComm &mgr, int *reso, bool *showKey, float *dpiScaleFactor) {
    bool ret = true;

    c.values_[kControllerPitchRangeUp] = pitchRangeUp->getValue();
    c.values_[kControllerPitchRangeDn] = pitchRangeDn->getValue();
    c.values_[kControllerPitchStep] = pitchStep->getValue();

    c.wheel.range = whlRange->getValue();
    c.wheel.pitch = whlPitch->getToggleState();
    c.wheel.amp = whlAmp->getToggleState();
    c.wheel.eg = whlEg->getToggleState();

    c.foot.range = ftRange->getValue();
    c.foot.pitch = ftPitch->getToggleState();
    c.foot.amp = ftAmp->getToggleState();
    c.foot.eg = ftEg->getToggleState();

    c.breath.range = brRange->getValue();
    c.breath.pitch = brPitch->getToggleState();
    c.breath.amp = brAmp->getToggleState();
    c.breath.eg = brEg->getToggleState();

    c.at.range = atRange->getValue();
    c.at.pitch = atPitch->getToggleState();
    c.at.amp = atAmp->getToggleState();
    c.at.eg = atEg->getToggleState();

    c.transpose12AsScale = transposeScale->getToggleState();

    c.mpeEnabled = mpeEnabled->getToggleState();
    c.mpePitchBendRange = mpePBRange->getValue();

    c.portamento_cc = portamentoTm->getValue() * 127.0f / 100.0f; // Convert to 0-127 range
    c.portamento_enable_cc = c.portamento_cc > 0;
    c.portamento_gliss_cc = glissando->getToggleState();

    c.refresh();

    if ( ! JUCEApplication::isStandaloneApp() ) {
        ret &= mgr.setInput(sysexIn->getItemText(sysexIn->getSelectedItemIndex()));
    }

    ret &= mgr.setOutput(sysexOut->getItemText(sysexOut->getSelectedItemIndex()));
    mgr.setChl(sysexChl->getValue() - 1);

    *reso = engineReso->getSelectedItemIndex();
    *showKey = showKeyboard->getToggleState();

    switch(scalingFactor->getSelectedItemIndex()) {
        case 1 :
            *dpiScaleFactor = 1.25;
            break;
        case 2 :
            *dpiScaleFactor = 1.5;
            break;
        case 3:
            *dpiScaleFactor = 2.0;
            break;
        case 4:
            *dpiScaleFactor = 3.0;
            break;
        case 5:
            *dpiScaleFactor = 4.0;
            break;
        default:
            *dpiScaleFactor = 1.0;
    }
    return ret;
}

void ParamDialog::setIsStandardTuning( bool b )
{
    is_standard_tuning_ = b;

    // Allow to see the actual tuning always.
/*
    // Enable to ''showTunButton'' only if the tuning
    // is not the standard one.
    if( showTunButton != nullptr )
        showTunButton->setEnabled( ! b );
*/
    if( resetTuningButton != nullptr )
        resetTuningButton->setEnabled( ! b );
}

// Force the internal component dialog to have keyboard focus. Ugly, but it is
// the only way I have found (including on the JUCE forum).
void ParamDialog::timerCallback() {
    stopTimer();
    grabKeyboardFocus();
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ParamDialog" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="4" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="710" initialHeight="370">
  <BACKGROUND backgroundColour="ff3c322f">
    <TEXT pos="20 16 276 23" fill="solid: ffffffff" hasStroke="0" text="Pitch Bend Range"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="20 56 276 23" fill="solid: ffffffff" hasStroke="0" text="Pitch Bend Step"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="24 319 245 23" fill="solid: ffffffff" hasStroke="0" text="DX7 Channel"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="20 190 276 23" fill="solid: ffffffff" hasStroke="0" text="Engine Resolution"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <RECT pos="22 174 306 1" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="22 227 306 1" fill="solid: ff000000" hasStroke="0"/>
    <TEXT pos="20 96 276 23" fill="solid: ffffffff" hasStroke="0" text="Show Keyboard"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <RECT pos="352 12 1 349" fill="solid: ff000000" hasStroke="0"/>
    <TEXT pos="368 16 276 23" fill="solid: ffffffff" hasStroke="0" text="Wheel"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="368 96 276 23" fill="solid: ffffffff" hasStroke="0" text="Breath"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="368 56 276 23" fill="solid: ffffffff" hasStroke="0" text="Foot"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="368 136 276 23" fill="solid: ffffffff" hasStroke="0" text="After Touch"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="528 163 48 23" fill="solid: ffffffff" hasStroke="0" text="PITCH"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="584 163 48 23" fill="solid: ffffffff" hasStroke="0" text="AMP"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="640 163 48 23" fill="solid: ffffffff" hasStroke="0" text="EG BIAS"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <RECT pos="376 194 325 1" fill="solid: ff000000" hasStroke="0"/>
    <TEXT pos="371 208 276 25" fill="solid: ffffffff" hasStroke="0" text="Tuning"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="371 242 157 25" fill="solid: ffffffff" hasStroke="0" text="Transposition 12 as:"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <RECT pos="368 310 328 1" fill="solid: ff000000" hasStroke="0"/>
    <TEXT pos="368 322 276 27" fill="solid: ffffffff" hasStroke="0" text="MPE"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="528 322 119 27" fill="solid: ffffffff" hasStroke="0" text="Bend Range"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="548 242 37 25" fill="solid: ffffffff" hasStroke="0" text="12"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="628 242 45 25" fill="solid: ffffffff" hasStroke="0" text="SCL"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="147 16 20 23" fill="solid: ffffffff" hasStroke="0" text="up"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="240 16 20 23" fill="solid: ffffffff" hasStroke="0" text="dn"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="20 136 276 23" fill="solid: ffffffff" hasStroke="0" text="UI Scaling"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="371 277 184 23" fill="solid: ffffffff" hasStroke="0" text="Portamento Rate"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
    <TEXT pos="628 274 75 27" fill="solid: ffffffff" hasStroke="0" text="Glissando"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="33"/>
  </BACKGROUND>
  <SLIDER name="pitchRangeDn" id="7409be5a8dfaa91" memberName="pitchRangeDn"
          virtualName="" explicitFocusOrder="2" pos="264 16 72 24" min="0.0"
          max="48.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchStep" id="b86af4b792e768ca" memberName="pitchStep"
          virtualName="" explicitFocusOrder="3" pos="264 56 72 24" min="0.0"
          max="12.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <COMBOBOX name="sysexIn" id="3750642d8b5be11" memberName="sysexIn" virtualName=""
            explicitFocusOrder="7" pos="104 244 224 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="sysexOut" id="44730115841c2214" memberName="sysexOut" virtualName=""
            explicitFocusOrder="8" pos="104 280 224 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="sysexChl" id="7fdc8830f90a7c86" memberName="sysexChl" virtualName=""
          explicitFocusOrder="9" pos="252 320 72 24" min="1.0" max="16.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <COMBOBOX name="new combo box" id="4087ff978c3d9e8d" memberName="engineReso"
            virtualName="" explicitFocusOrder="6" pos="160 188 168 24" editable="0"
            layout="33" items="Modern (24-bit)&#10;Mark I&#10;OPL Series"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="showKeyboard" id="c963d2cb8e49ffd7" memberName="showKeyboard"
                virtualName="LightedToggleButton" explicitFocusOrder="4" pos="264 96 56 24"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <SLIDER name="whlRange" id="3d6522f5f581e580" memberName="whlRange" virtualName=""
          explicitFocusOrder="10" pos="448 16 72 24" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="ftRange" id="cf553f74c3fb0d12" memberName="ftRange" virtualName=""
          explicitFocusOrder="14" pos="448 56 72 24" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="brRange" id="c4aa6814f75016a7" memberName="brRange" virtualName=""
          explicitFocusOrder="18" pos="448 96 72 24" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="atRange" id="d0aa1ebb24284577" memberName="atRange" virtualName=""
          explicitFocusOrder="22" pos="448 136 72 24" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="whlEg" id="d8242ae592c912a" memberName="whlEg" virtualName="LightedToggleButton"
                explicitFocusOrder="13" pos="640 16 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="ftEg" id="69d816607bd71cb0" memberName="ftEg" virtualName="LightedToggleButton"
                explicitFocusOrder="17" pos="640 56 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="brEg" id="ba89ae54d676983f" memberName="brEg" virtualName="LightedToggleButton"
                explicitFocusOrder="21" pos="640 96 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="atEg" id="371934a58ce5f1bc" memberName="atEg" virtualName="LightedToggleButton"
                explicitFocusOrder="25" pos="640 136 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="whlAmp" id="3d4e46e63c3ddd86" memberName="whlAmp" virtualName="LightedToggleButton"
                explicitFocusOrder="12" pos="584 16 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="ftAmp" id="f631892e209b094a" memberName="ftAmp" virtualName="LightedToggleButton"
                explicitFocusOrder="16" pos="584 56 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="brAmp" id="d0a68d37220638f1" memberName="brAmp" virtualName="LightedToggleButton"
                explicitFocusOrder="20" pos="584 96 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="atAmp" id="4220c7b22e7845ea" memberName="atAmp" virtualName="LightedToggleButton"
                explicitFocusOrder="24" pos="584 136 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="whlPitch" id="b7a626ec1e45af16" memberName="whlPitch" virtualName="LightedToggleButton"
                explicitFocusOrder="11" pos="528 16 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="ftPitch" id="1acedf6f16a5a3" memberName="ftPitch" virtualName="LightedToggleButton"
                explicitFocusOrder="15" pos="528 56 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="brPitch" id="23fa82533e004b96" memberName="brPitch" virtualName="LightedToggleButton"
                explicitFocusOrder="19" pos="528 96 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="atPitch" id="43805c6a4673e291" memberName="atPitch" virtualName="LightedToggleButton"
                explicitFocusOrder="23" pos="528 136 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TEXTBUTTON name="scl button" id="7a6b689f406ae37d" memberName="sclButton"
              virtualName="" explicitFocusOrder="26" pos="448 205 56 30" buttonText="SCL"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="kbm button" id="181d5b67bc897076" memberName="kbmButton"
              virtualName="" explicitFocusOrder="27" pos="512 205 56 30" buttonText="KBM"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="show tuning button" id="565d5c630087f71" memberName="showTunButton"
              virtualName="" explicitFocusOrder="28" pos="576 205 48 30" buttonText="Show"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="reset tuning button" id="44a4ec0c1d200987" memberName="resetTuningButton"
              virtualName="" explicitFocusOrder="29" pos="632 205 48 30" buttonText="Reset"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="transposeScale" id="9d4dd65775ed9e38" memberName="transposeScale"
                virtualName="LightedToggleButton" explicitFocusOrder="30" pos="576 240 56 30"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="1"/>
  <SLIDER name="mpePBRange" id="a13948d1cc74a51a" memberName="mpePBRange"
          virtualName="" explicitFocusOrder="32" pos="616 323 72 24" min="0.0"
          max="96.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="mpeEnabled" id="e1a11d0372879dd8" memberName="mpeEnabled"
                virtualName="LightedToggleButton" explicitFocusOrder="31" pos="456 320 48 30"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <IMAGEBUTTON name="new button" id="1c9edab9992cbeef" memberName="transposeHelp"
               virtualName="" explicitFocusOrder="0" pos="500 245 20 20" buttonText="new button"
               connectedEdges="0" needsCallback="1" radioGroupId="0" keepProportions="1"
               resourceNormal="BinaryData::HelpButton_png" opacityNormal="1.0"
               colourNormal="0" resourceOver="" opacityOver="1.0" colourOver="0"
               resourceDown="" opacityDown="1.0" colourDown="0"/>
  <SLIDER name="pitchRangeUp" id="56a2aeddd2444bea" memberName="pitchRangeUp"
          virtualName="" explicitFocusOrder="1" pos="168 16 72 24" min="0.0"
          max="48.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <COMBOBOX name="scalingFactor" id="843af1bae988d4d6" memberName="scalingFactor"
            virtualName="" explicitFocusOrder="5" pos="236 136 90 24" editable="0"
            layout="33" items="100 %&#10;125 %&#10;150 %&#10;200 %&#10;300 %&#10;400 %"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="glissando" id="f531397d4cf195a6" memberName="glissando"
                virtualName="LightedToggleButton" explicitFocusOrder="30" pos="576 272 56 30"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="1"/>
  <SLIDER name="portamentoTm" id="12588a21a3ec6c02" memberName="portamentoTm"
          virtualName="" explicitFocusOrder="22" pos="504 276 72 24" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

