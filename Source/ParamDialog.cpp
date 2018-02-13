/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

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
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (pitchRange = new Slider ("pitchRange"));
    pitchRange->setRange (0, 12, 1);
    pitchRange->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchRange->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    pitchRange->addListener (this);

    addAndMakeVisible (pitchStep = new Slider ("pitchStep"));
    pitchStep->setRange (0, 12, 1);
    pitchStep->setSliderStyle (Slider::RotaryVerticalDrag);
    pitchStep->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    pitchStep->addListener (this);

    addAndMakeVisible (sysexIn = new ComboBox ("sysexIn"));
    sysexIn->setEditableText (false);
    sysexIn->setJustificationType (Justification::centredLeft);
    sysexIn->setTextWhenNothingSelected (String());
    sysexIn->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    sysexIn->addListener (this);

    addAndMakeVisible (sysexOut = new ComboBox ("sysexOut"));
    sysexOut->setEditableText (false);
    sysexOut->setJustificationType (Justification::centredLeft);
    sysexOut->setTextWhenNothingSelected (String());
    sysexOut->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    sysexOut->addListener (this);

    addAndMakeVisible (sysexChl = new Slider ("sysexChl"));
    sysexChl->setRange (1, 16, 1);
    sysexChl->setSliderStyle (Slider::RotaryVerticalDrag);
    sysexChl->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    sysexChl->addListener (this);

    addAndMakeVisible (engineReso = new ComboBox ("new combo box"));
    engineReso->setEditableText (false);
    engineReso->setJustificationType (Justification::centredLeft);
    engineReso->setTextWhenNothingSelected (String());
    engineReso->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    engineReso->addItem (TRANS("Modern (24-bit)"), 1);
    engineReso->addItem (TRANS("Mark I"), 2);
    engineReso->addItem (TRANS("OPL Series"), 3);
    engineReso->addListener (this);

    addAndMakeVisible (showKeyboard = new ToggleButton ("showKeyboard"));
    showKeyboard->setButtonText (String());

    addAndMakeVisible (whlRange = new Slider ("whlRange"));
    whlRange->setRange (0, 99, 1);
    whlRange->setSliderStyle (Slider::RotaryVerticalDrag);
    whlRange->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    whlRange->addListener (this);

    addAndMakeVisible (ftRange = new Slider ("ftRange"));
    ftRange->setRange (0, 99, 1);
    ftRange->setSliderStyle (Slider::RotaryVerticalDrag);
    ftRange->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    ftRange->addListener (this);

    addAndMakeVisible (brRange = new Slider ("brRange"));
    brRange->setRange (0, 99, 1);
    brRange->setSliderStyle (Slider::RotaryVerticalDrag);
    brRange->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    brRange->addListener (this);

    addAndMakeVisible (atRange = new Slider ("atRange"));
    atRange->setRange (0, 99, 1);
    atRange->setSliderStyle (Slider::RotaryVerticalDrag);
    atRange->setTextBoxStyle (Slider::TextBoxLeft, false, 80, 20);
    atRange->addListener (this);

    addAndMakeVisible (whlEg = new ToggleButton ("whlEg"));
    whlEg->setButtonText (String());
    whlEg->addListener (this);

    addAndMakeVisible (ftEg = new ToggleButton ("ftEg"));
    ftEg->setButtonText (String());
    ftEg->addListener (this);

    addAndMakeVisible (brEg = new ToggleButton ("brEg"));
    brEg->setButtonText (String());
    brEg->addListener (this);

    addAndMakeVisible (atEg = new ToggleButton ("atEg"));
    atEg->setButtonText (String());
    atEg->addListener (this);

    addAndMakeVisible (whlAmp = new ToggleButton ("whlAmp"));
    whlAmp->setButtonText (String());
    whlAmp->addListener (this);

    addAndMakeVisible (ftAmp = new ToggleButton ("ftAmp"));
    ftAmp->setButtonText (String());
    ftAmp->addListener (this);

    addAndMakeVisible (brAmp = new ToggleButton ("brAmp"));
    brAmp->setButtonText (String());
    brAmp->addListener (this);

    addAndMakeVisible (atAmp = new ToggleButton ("atAmp"));
    atAmp->setButtonText (String());
    atAmp->addListener (this);

    addAndMakeVisible (whlPitch = new ToggleButton ("whlPitch"));
    whlPitch->setButtonText (String());
    whlPitch->addListener (this);

    addAndMakeVisible (ftPitch = new ToggleButton ("ftPitch"));
    ftPitch->setButtonText (String());
    ftPitch->addListener (this);

    addAndMakeVisible (brPitch = new ToggleButton ("brPitch"));
    brPitch->setButtonText (String());
    brPitch->addListener (this);

    addAndMakeVisible (atPitch = new ToggleButton ("atPitch"));
    atPitch->setButtonText (String());
    atPitch->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (710, 350);


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

    if ( JUCEApplication::isStandaloneApp() ) {
        sysexIn->setVisible(false);
    }
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


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ParamDialog::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff3c322f));

    {
        int x = 20, y = 16, width = 276, height = 23;
        String text (TRANS("Pitch Bend Range"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 20, y = 56, width = 276, height = 23;
        String text (TRANS("Pitch Bend Step"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 20, y = 304, width = 245, height = 23;
        String text (TRANS("DX7 Channel"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 20, y = 156, width = 276, height = 23;
        String text (TRANS("Engine Resolution"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 22, y = 138, width = 306, height = 1;
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 22, y = 195, width = 306, height = 1;
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 20, y = 96, width = 276, height = 23;
        String text (TRANS("Show Keyboard"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 352, y = 11, width = 1, height = 325;
        Colour fillColour = Colours::black;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
    }

    {
        int x = 368, y = 16, width = 276, height = 23;
        String text (TRANS("Wheel"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 368, y = 96, width = 276, height = 23;
        String text (TRANS("Foot"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 368, y = 56, width = 276, height = 23;
        String text (TRANS("Breath"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 368, y = 136, width = 276, height = 23;
        String text (TRANS("After Touch"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 533, y = 163, width = 48, height = 23;
        String text (TRANS("PITCH"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 589, y = 163, width = 48, height = 23;
        String text (TRANS("AMP"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    {
        int x = 645, y = 163, width = 48, height = 23;
        String text (TRANS("EG BIAS"));
        Colour fillColour = Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    Justification::centredLeft, true);
    }

    //[UserPaint] Add your own custom painting code here..
    if ( ! JUCEApplication::isStandaloneApp() ) {
        g.setColour (Colours::white);
        g.setFont (Font (15.00f, Font::plain));
        g.drawText (TRANS("DX7 In"),
                    20, 224, 131, 23,
                    Justification::centredLeft, true);

    }
    g.setColour (Colours::white);
    g.setFont (Font (15.00f, Font::plain));
    g.drawText (TRANS("DX7 Out"),
                20, 264, 131, 23,
                Justification::centredLeft, true);
    //[/UserPaint]
}

void ParamDialog::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    pitchRange->setBounds (264, 16, 72, 24);
    pitchStep->setBounds (264, 56, 72, 24);
    sysexIn->setBounds (104, 224, 224, 24);
    sysexOut->setBounds (104, 264, 224, 24);
    sysexChl->setBounds (264, 304, 72, 24);
    engineReso->setBounds (160, 156, 168, 24);
    showKeyboard->setBounds (264, 96, 56, 24);
    whlRange->setBounds (448, 16, 72, 24);
    ftRange->setBounds (448, 56, 72, 24);
    brRange->setBounds (448, 96, 72, 24);
    atRange->setBounds (448, 136, 72, 24);
    whlEg->setBounds (640, 16, 56, 24);
    ftEg->setBounds (640, 56, 56, 24);
    brEg->setBounds (640, 96, 56, 24);
    atEg->setBounds (640, 136, 56, 24);
    whlAmp->setBounds (584, 16, 56, 24);
    ftAmp->setBounds (584, 56, 56, 24);
    brAmp->setBounds (584, 96, 56, 24);
    atAmp->setBounds (584, 136, 56, 24);
    whlPitch->setBounds (528, 16, 56, 24);
    ftPitch->setBounds (528, 56, 56, 24);
    brPitch->setBounds (528, 96, 56, 24);
    atPitch->setBounds (528, 136, 56, 24);
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
    else if (sliderThatWasMoved == whlRange)
    {
        //[UserSliderCode_whlRange] -- add your slider handling code here..
        //[/UserSliderCode_whlRange]
    }
    else if (sliderThatWasMoved == ftRange)
    {
        //[UserSliderCode_ftRange] -- add your slider handling code here..
        //[/UserSliderCode_ftRange]
    }
    else if (sliderThatWasMoved == brRange)
    {
        //[UserSliderCode_brRange] -- add your slider handling code here..
        //[/UserSliderCode_brRange]
    }
    else if (sliderThatWasMoved == atRange)
    {
        //[UserSliderCode_atRange] -- add your slider handling code here..
        //[/UserSliderCode_atRange]
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
    else if (comboBoxThatHasChanged == engineReso)
    {
        //[UserComboBoxCode_engineReso] -- add your combo box handling code here..
        //[/UserComboBoxCode_engineReso]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void ParamDialog::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == whlEg)
    {
        //[UserButtonCode_whlEg] -- add your button handler code here..
        //[/UserButtonCode_whlEg]
    }
    else if (buttonThatWasClicked == ftEg)
    {
        //[UserButtonCode_ftEg] -- add your button handler code here..
        //[/UserButtonCode_ftEg]
    }
    else if (buttonThatWasClicked == brEg)
    {
        //[UserButtonCode_brEg] -- add your button handler code here..
        //[/UserButtonCode_brEg]
    }
    else if (buttonThatWasClicked == atEg)
    {
        //[UserButtonCode_atEg] -- add your button handler code here..
        //[/UserButtonCode_atEg]
    }
    else if (buttonThatWasClicked == whlAmp)
    {
        //[UserButtonCode_whlAmp] -- add your button handler code here..
        //[/UserButtonCode_whlAmp]
    }
    else if (buttonThatWasClicked == ftAmp)
    {
        //[UserButtonCode_ftAmp] -- add your button handler code here..
        //[/UserButtonCode_ftAmp]
    }
    else if (buttonThatWasClicked == brAmp)
    {
        //[UserButtonCode_brAmp] -- add your button handler code here..
        //[/UserButtonCode_brAmp]
    }
    else if (buttonThatWasClicked == atAmp)
    {
        //[UserButtonCode_atAmp] -- add your button handler code here..
        //[/UserButtonCode_atAmp]
    }
    else if (buttonThatWasClicked == whlPitch)
    {
        //[UserButtonCode_whlPitch] -- add your button handler code here..
        //[/UserButtonCode_whlPitch]
    }
    else if (buttonThatWasClicked == ftPitch)
    {
        //[UserButtonCode_ftPitch] -- add your button handler code here..
        //[/UserButtonCode_ftPitch]
    }
    else if (buttonThatWasClicked == brPitch)
    {
        //[UserButtonCode_brPitch] -- add your button handler code here..
        //[/UserButtonCode_brPitch]
    }
    else if (buttonThatWasClicked == atPitch)
    {
        //[UserButtonCode_atPitch] -- add your button handler code here..
        //[/UserButtonCode_atPitch]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void ParamDialog::setDialogValues(Controllers &c, SysexComm &mgr, int reso, bool showKey) {
    pitchRange->setValue(c.values_[kControllerPitchRange]);
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
}

bool ParamDialog::getDialogValues(Controllers &c, SysexComm &mgr, int *reso, bool *showKey) {
    bool ret = true;

    c.values_[kControllerPitchRange] = pitchRange->getValue();
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

    c.refresh();

    if ( ! JUCEApplication::isStandaloneApp() ) {
        ret &= mgr.setInput(sysexIn->getItemText(sysexIn->getSelectedItemIndex()));
    }
    ret &= mgr.setOutput(sysexOut->getItemText(sysexOut->getSelectedItemIndex()));
    mgr.setChl(sysexChl->getValue() - 1);

    *reso = engineReso->getSelectedItemIndex();
    *showKey = showKeyboard->getToggleState();
    return ret;
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
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="710" initialHeight="350">
  <BACKGROUND backgroundColour="ff3c322f">
    <TEXT pos="20 16 276 23" fill="solid: ffffffff" hasStroke="0" text="Pitch Bend Range"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <TEXT pos="20 56 276 23" fill="solid: ffffffff" hasStroke="0" text="Pitch Bend Step"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <TEXT pos="20 304 245 23" fill="solid: ffffffff" hasStroke="0" text="DX7 Channel"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <TEXT pos="20 156 276 23" fill="solid: ffffffff" hasStroke="0" text="Engine Resolution"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <RECT pos="22 138 306 1" fill="solid: ff000000" hasStroke="0"/>
    <RECT pos="22 195 306 1" fill="solid: ff000000" hasStroke="0"/>
    <TEXT pos="20 96 276 23" fill="solid: ffffffff" hasStroke="0" text="Show Keyboard"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <RECT pos="352 11 1 325" fill="solid: ff000000" hasStroke="0"/>
    <TEXT pos="368 16 276 23" fill="solid: ffffffff" hasStroke="0" text="Wheel"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <TEXT pos="368 96 276 23" fill="solid: ffffffff" hasStroke="0" text="Foot"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <TEXT pos="368 56 276 23" fill="solid: ffffffff" hasStroke="0" text="Breath"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <TEXT pos="368 136 276 23" fill="solid: ffffffff" hasStroke="0" text="After Touch"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <TEXT pos="533 163 48 23" fill="solid: ffffffff" hasStroke="0" text="PITCH"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <TEXT pos="589 163 48 23" fill="solid: ffffffff" hasStroke="0" text="AMP"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
    <TEXT pos="645 163 48 23" fill="solid: ffffffff" hasStroke="0" text="EG BIAS"
          fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
          justification="33"/>
  </BACKGROUND>
  <SLIDER name="pitchRange" id="7409be5a8dfaa91" memberName="pitchRange"
          virtualName="" explicitFocusOrder="0" pos="264 16 72 24" min="0"
          max="12" int="1" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <SLIDER name="pitchStep" id="b86af4b792e768ca" memberName="pitchStep"
          virtualName="" explicitFocusOrder="0" pos="264 56 72 24" min="0"
          max="12" int="1" style="RotaryVerticalDrag" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <COMBOBOX name="sysexIn" id="3750642d8b5be11" memberName="sysexIn" virtualName=""
            explicitFocusOrder="0" pos="104 224 224 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <COMBOBOX name="sysexOut" id="44730115841c2214" memberName="sysexOut" virtualName=""
            explicitFocusOrder="0" pos="104 264 224 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <SLIDER name="sysexChl" id="7fdc8830f90a7c86" memberName="sysexChl" virtualName=""
          explicitFocusOrder="0" pos="264 304 72 24" min="1" max="16" int="1"
          style="RotaryVerticalDrag" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <COMBOBOX name="new combo box" id="4087ff978c3d9e8d" memberName="engineReso"
            virtualName="" explicitFocusOrder="0" pos="160 156 168 24" editable="0"
            layout="33" items="Modern (24-bit)&#10;Mark I&#10;OPL Series"
            textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <TOGGLEBUTTON name="showKeyboard" id="c963d2cb8e49ffd7" memberName="showKeyboard"
                virtualName="" explicitFocusOrder="0" pos="264 96 56 24" buttonText=""
                connectedEdges="0" needsCallback="0" radioGroupId="0" state="0"/>
  <SLIDER name="whlRange" id="3d6522f5f581e580" memberName="whlRange" virtualName=""
          explicitFocusOrder="0" pos="448 16 72 24" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="ftRange" id="cf553f74c3fb0d12" memberName="ftRange" virtualName=""
          explicitFocusOrder="0" pos="448 56 72 24" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="brRange" id="c4aa6814f75016a7" memberName="brRange" virtualName=""
          explicitFocusOrder="0" pos="448 96 72 24" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <SLIDER name="atRange" id="d0aa1ebb24284577" memberName="atRange" virtualName=""
          explicitFocusOrder="0" pos="448 136 72 24" min="0" max="99" int="1"
          style="RotaryVerticalDrag" textBoxPos="TextBoxLeft" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <TOGGLEBUTTON name="whlEg" id="d8242ae592c912a" memberName="whlEg" virtualName=""
                explicitFocusOrder="0" pos="640 16 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="ftEg" id="69d816607bd71cb0" memberName="ftEg" virtualName=""
                explicitFocusOrder="0" pos="640 56 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="brEg" id="ba89ae54d676983f" memberName="brEg" virtualName=""
                explicitFocusOrder="0" pos="640 96 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="atEg" id="371934a58ce5f1bc" memberName="atEg" virtualName=""
                explicitFocusOrder="0" pos="640 136 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="whlAmp" id="3d4e46e63c3ddd86" memberName="whlAmp" virtualName=""
                explicitFocusOrder="0" pos="584 16 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="ftAmp" id="f631892e209b094a" memberName="ftAmp" virtualName=""
                explicitFocusOrder="0" pos="584 56 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="brAmp" id="d0a68d37220638f1" memberName="brAmp" virtualName=""
                explicitFocusOrder="0" pos="584 96 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="atAmp" id="4220c7b22e7845ea" memberName="atAmp" virtualName=""
                explicitFocusOrder="0" pos="584 136 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="whlPitch" id="b7a626ec1e45af16" memberName="whlPitch" virtualName=""
                explicitFocusOrder="0" pos="528 16 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="ftPitch" id="1acedf6f16a5a3" memberName="ftPitch" virtualName=""
                explicitFocusOrder="0" pos="528 56 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="brPitch" id="23fa82533e004b96" memberName="brPitch" virtualName=""
                explicitFocusOrder="0" pos="528 96 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <TOGGLEBUTTON name="atPitch" id="43805c6a4673e291" memberName="atPitch" virtualName=""
                explicitFocusOrder="0" pos="528 136 56 24" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
