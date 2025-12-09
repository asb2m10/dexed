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
#include "PluginEditor.h"
#include "DXLookNFeel.h"
#include "../debugger/value_tree_debugger.h"
//[/Headers]

#include "GlobalEditor.h"

#include "parameter/Model.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

/**
 * Ugly but useful midi monitor to know if you are really sending/receiving something from the DX7
 * If the midi is not configured this component wont show up
 *
 */
#ifdef IMPLEMENT_MidiMonitor
class MidiMonitor : public Component {
    SysexComm *midi;
    Image light;
    int imageHeight;
    int imageHeight2;
    int imageWidth;

    SharedResourcePointer<DXLookNFeel> lookAndFeel;
public:
    MidiMonitor(SysexComm *sysexComm) {
        midi = sysexComm;
        light = lookAndFeel->imageLight;
        imageHeight = light.getHeight();
        imageHeight2 = imageHeight / 2;
        imageWidth = light.getWidth();

        TRACE("WARNING! This functionality is a candidate for deprecation/obsolescence!");
    }

    void paint(Graphics &g) {
        if ( ! (midi->isInputActive() || midi->isOutputActive() ) )
            return;
        g.setColour(Colours::white);

        if ( midi->isInputActive() ) {
            g.drawSingleLineText("DX7 IN", 24, 18);
            g.drawImage(light, 0, 0, imageWidth, imageHeight2, 0, midi->inActivity ? imageHeight2 : 0, imageWidth, imageHeight2);
            midi->inActivity = false;
        }

        if ( midi->isOutputActive() ) {
            g.drawSingleLineText("DX7 OUT", 24, 36);
            g.drawImage(light, 0, 18, imageWidth, imageHeight2, 0, midi->outActivity ? imageHeight2 : 0, imageWidth, imageHeight2);
            midi->outActivity = false;
        }
    }
};
#endif //IMPLEMENT_MidiMonitor

class AboutBox : public DialogWindow {
public:
    Image logo_png;
    std::unique_ptr<juce::HyperlinkButton> dexed; // changed to std::unique_ptr from juce::ScopedPointer
    std::unique_ptr<juce::HyperlinkButton> surge; // changed to std::unique_ptr from juce::ScopedPointer

    AboutBox() : DialogWindow("About", Colour(0xFF000000), true),
        dexed(std::make_unique<juce::HyperlinkButton>("https://asb2m10.github.io/dexed/", URL("https://asb2m10.github.io/dexed/"))),
        surge(std::make_unique<juce::HyperlinkButton>("https://surge-synthesizer.github.io/", URL("https://surge-synthesizer.github.io/")))
    {
        setUsingNativeTitleBar(false);
        setAlwaysOnTop(true);
        logo_png = ImageCache::getFromMemory(BinaryData::dexedlogo_png, BinaryData::dexedlogo_pngSize);
        setSize(logo_png.getWidth() + 8, 500);
        //centreAroundComponent(parent, getWidth(), getHeight());

        dexed->setColour(HyperlinkButton::ColourIds::textColourId, Colour(0xFF4ea097));
        dexed->setJustificationType(Justification::left);
        dexed->setBounds(18, 433, getWidth() - 36, 30);

        surge->setColour(HyperlinkButton::ColourIds::textColourId, Colour(0xFF4ea097));
        surge->setJustificationType(Justification::left);
        surge->setBounds(18, 458, getWidth() - 36, 30);

        // create a new Component to hold ''dexed'' and ''surge'' as subcomponents
        // and set this holder Component as the content component of the DialogWindow
        Component* holder = new Component();
        holder->setSize(getWidth(), getHeight());
        holder->addAndMakeVisible((juce::Component*)dexed.get());
        holder->addAndMakeVisible((juce::Component*)surge.get());
        setContentOwned(holder, true);  // TODO: ''setContentComponent(holder, true, true);'' also worked; which is the better?
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
                            "This software is released under the GPL V3\n\n"
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
GlobalEditor::GlobalEditor (DexedAudioProcessor &processor) : processor(processor) {
    binder = std::make_unique<AudioComponentContainer>(*this, processor.parameters);

    algoDisplay.reset (new AlgoDisplay());
    algoDisplay->algo = (char *) &(processor.data[134]);
    addAndMakeVisible(algoDisplay.get());
    algoDisplay->setName("algoDisplay");
    algoDisplay->setBounds(335, 30, 152, 91);
    algoDisplay->opStatus = processor.controllers.opSwitch;

    auto algo = std::make_unique<DXSlider>(IDs::algorithm.name);
    algo->setExplicitFocusOrder (11);
    algo->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    algo->setTextBoxStyle(juce::Slider::NoTextBox, true, 80, 20);
    algo->setBounds(501, 22, 34, 34);
    algo->onValueChange = [this]() {
        algoDisplay->repaint();
    };
    binder->addAndAttach(std::move(algo));

    auto feedback = std::make_unique<DXSlider>(IDs::feedback.name);
    feedback->setExplicitFocusOrder (12);
    feedback->setSliderStyle(juce::Slider::RotaryVerticalDrag);
    feedback->setTextBoxStyle(juce::Slider::NoTextBox, true, 80, 20);
    feedback->setBounds(501, 81, 34, 34);
    binder->addAndAttach(std::move(feedback));

    auto lfoSpeed = std::make_unique<DXSlider>(IDs::lfoRate.name);
    lfoSpeed->setExplicitFocusOrder (14);
    lfoSpeed->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lfoSpeed->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    lfoSpeed->setBounds (564, 50, 34, 34);
    binder->addAndAttach(std::move(lfoSpeed));

    auto lfoAmDepth = std::make_unique<DXSlider>(IDs::lfoAmpDepth.name);
    lfoAmDepth->setExplicitFocusOrder (19);
    lfoAmDepth->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lfoAmDepth->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    lfoAmDepth->setBounds (686, 50, 34, 34);
    binder->addAndAttach(std::move(lfoAmDepth));

    auto lfoPitchDepth = std::make_unique<DXSlider>(IDs::lfoPmDepth.name);
    lfoPitchDepth->setExplicitFocusOrder (18);
    lfoPitchDepth->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lfoPitchDepth->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    lfoPitchDepth->setBounds (646, 50, 34, 34);
    binder->addAndAttach(std::move(lfoPitchDepth));

    auto lfoDelay = std::make_unique<DXSlider>(IDs::lfoDelay.name);
    lfoDelay->setExplicitFocusOrder (15);
    lfoDelay->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    lfoDelay->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    lfoDelay->setBounds (603, 50, 34, 34);
    binder->addAndAttach(std::move(lfoDelay));

    auto pitchRate1 = std::make_unique<DXSlider>(IDs::pitchEgRate.idx(0).name);
    pitchRate1->setExplicitFocusOrder (23);
    pitchRate1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRate1->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchRate1->setBounds (739, 96, 34, 34);
    binder->addAndAttach(std::move(pitchRate1));

    auto pitchRate2 = std::make_unique<DXSlider>(IDs::pitchEgRate.idx(1).name);
    pitchRate2->setExplicitFocusOrder (24);
    pitchRate2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRate2->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchRate2->setBounds (767, 96, 34, 34);
    binder->addAndAttach(std::move(pitchRate2));

    auto pitchRate3 = std::make_unique<DXSlider>(IDs::pitchEgRate.idx(2).name);
    pitchRate3->setExplicitFocusOrder (26);
    pitchRate3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRate3->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchRate3->setBounds (795, 96, 35, 34);
    binder->addAndAttach(std::move(pitchRate3));

    auto pitchRate4 = std::make_unique<DXSlider>(IDs::pitchEgRate.idx(3).name);
    pitchRate4->setExplicitFocusOrder (28);
    pitchRate4->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchRate4->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchRate4->setBounds (823, 96, 34, 34);
    binder->addAndAttach(std::move(pitchRate4));

    auto pitchLevel1 = std::make_unique<DXSlider>(IDs::pitchEgLevel.idx(0).name);
    pitchLevel1->setExplicitFocusOrder (21);
    pitchLevel1->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchLevel1->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchLevel1->setBounds (739, 57, 34, 34);
    binder->addAndAttach(std::move(pitchLevel1));

    auto pitchLevel2 = std::make_unique<DXSlider>(IDs::pitchEgLevel.idx(1).name);
    pitchLevel2->setExplicitFocusOrder (23);
    pitchLevel2->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchLevel2->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchLevel2->setBounds (767, 57, 34, 34);
    binder->addAndAttach(std::move(pitchLevel2));

    auto pitchLevel3 = std::make_unique<DXSlider>(IDs::pitchEgLevel.idx(2).name);
    pitchLevel3->setExplicitFocusOrder (25);
    pitchLevel3->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchLevel3->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchLevel3->setBounds (795, 56, 34, 34);
    binder->addAndAttach(std::move(pitchLevel3));

    auto pitchLevel4 = std::make_unique<DXSlider>(IDs::pitchEgLevel.idx(3).name);
    pitchLevel4->setExplicitFocusOrder (27);
    pitchLevel4->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchLevel4->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchLevel4->setBounds (823, 56, 34, 34);
    binder->addAndAttach(std::move(pitchLevel4));

    auto cutoff = std::make_unique<DXSlider>(IDs::cutoff.name);
    cutoff->setExplicitFocusOrder(6);
    cutoff->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    cutoff->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    cutoff->setBounds (234, 9, 34, 34);
    binder->addAndAttach(std::move(cutoff));

    auto reso = std::make_unique<DXSlider>(IDs::resonance.name);
    reso->setExplicitFocusOrder(7);
    reso->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    reso->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    reso->setBounds (278, 9, 34, 34);
    binder->addAndAttach(std::move(reso));

    auto transpose = std::make_unique<DXSlider>(IDs::transpose.name);
    transpose->setExplicitFocusOrder (9);
    transpose->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    transpose->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    transpose->setBounds (202, 60, 34, 34);
    binder->addAndAttach(std::move(transpose));

    pitchModSens = std::make_unique<DXSlider>(IDs::pitchModSens.name);
    pitchModSens->setExplicitFocusOrder(17);
    pitchModSens->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    pitchModSens->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pitchModSens->setBounds (666, 5, 34, 34);
    binder->addAndAttach(std::move(pitchModSens));

    auto output = std::make_unique<DXSlider>(IDs::output.name);
    output->setExplicitFocusOrder (8);
    output->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    output->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    output->setBounds (157, 60, 34, 34);
    binder->addAndAttach(std::move(output));

    auto tune = std::make_unique<DXSlider>(IDs::tune.name);
    tune->setExplicitFocusOrder (5);
    tune->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    tune->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    tune->setBounds (190, 9, 34, 34);
    binder->addAndAttach(std::move(tune));

    lfoType.reset (new ComboBoxImage());
    addAndMakeVisible (lfoType.get());
    lfoType->setExplicitFocusOrder (13);
    lfoType->setName ("lfoType");
    lfoType->setBounds (583, 8, 36, 26);

    // EVENT DRIVEN COMPONENTS

    monoMode = std::make_unique<juce::ToggleButton>(IDs::monoMode.name);
    monoMode->setExplicitFocusOrder(10);
    monoMode->setButtonText (juce::String());
    monoMode->setBounds (249, 65, 48, 26);
    monoMode->onStateChange = [this]() {
        repaint();
    };
    binder->attach(monoMode.get());
    addAndMakeVisible(monoMode.get());

    lfoSync = std::make_unique<juce::ToggleButton>(IDs::lfoKeySync.name);
    lfoSync->setExplicitFocusOrder(16);
    lfoSync->setButtonText (juce::String());
    lfoSync->setBounds (565, 96, 48, 26);
    lfoSync->onStateChange = [this]() {
        repaint();
    };
    binder->attach(lfoSync.get());
    addAndMakeVisible(lfoSync.get());

    oscSync = std::make_unique<juce::ToggleButton>(IDs::oscKeySync.name);
    oscSync->setExplicitFocusOrder(20);
    oscSync->setButtonText (juce::String());
    oscSync->setBounds (650, 96, 48, 26);
    lfoSync->onStateChange = [this]() {
        repaint();
    };
    binder->attach(oscSync.get());
    addAndMakeVisible(oscSync.get());

    // EXTRA COMPONENTS

    pitchEnvDisplay.reset (new PitchEnvDisplay());
    addAndMakeVisible (pitchEnvDisplay.get());
    pitchEnvDisplay->setName ("pitchEnvDisplay");
    pitchEnvDisplay->setBounds (751, 10, 93, 30);

    lcdDisplay.reset (new LcdDisplay());
    addAndMakeVisible (lcdDisplay.get());
    lcdDisplay->setName ("lcdDisplay");
    lcdDisplay->setBounds (6, 87, 140, 13);

    vuOutput.reset (new VuMeterOutput());
    addAndMakeVisible (vuOutput.get());
    vuOutput->setName ("vuOutput");
    vuOutput->setBounds (6, 103, 140, 8);

    initButton.reset (new juce::TextButton ("initButton"));
    addAndMakeVisible (initButton.get());
    initButton->setExplicitFocusOrder (3);
    initButton->setButtonText (TRANS ("INIT"));
    initButton->setBounds (100, 111, 50, 30);

    parmButton.reset (new juce::TextButton ("parmButton"));
    addAndMakeVisible (parmButton.get());
    parmButton->setExplicitFocusOrder (2);
    parmButton->setButtonText (TRANS ("PARM"));
    parmButton->setBounds (52, 111, 50, 30);

    cartButton.reset (new juce::TextButton ("cartButton"));
    addAndMakeVisible (cartButton.get());
    cartButton->setExplicitFocusOrder (1);
    cartButton->setButtonText (TRANS ("CART"));
    cartButton->setBounds (3, 111, 50, 30);

    storeButton.reset (new juce::TextButton ("storeButton"));
    addAndMakeVisible (storeButton.get());
    storeButton->setExplicitFocusOrder (4);
    storeButton->setButtonText (TRANS ("STORE"));
    storeButton->setBounds (270, 109, 50, 30);

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
    setTitle("Global Parameters");
    setFocusContainerType(FocusContainerType::focusContainer);
    setWantsKeyboardFocus(true);
    aboutButton->setTitle("About DEXED");

    pitchEnvDisplay->pvalues = &(processor.data[126]);

#ifdef IMPLEMENT_MidiMonitor
    midiMonitor = std::make_unique<MidiMonitor>(&(processor->sysexComm));
    addAndMakeVisible(*midiMonitor);
    midiMonitor->setBounds(110, 10, 80, 45);    //midiMonitor->setBounds(155, 21, 80, 45);
#endif //IMPLEMENT_MidiMonitor


    //[/Constructor]
}

GlobalEditor::~GlobalEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    binder->freeAttachments();

    //[/Destructor_pre]

    transpose = nullptr;
    oscSync = nullptr;
    pitchModSens = nullptr;
    pitchEnvDisplay = nullptr;
    algoDisplay = nullptr;
    lcdDisplay = nullptr;
    vuOutput = nullptr;
    initButton = nullptr;
    parmButton = nullptr;
    cartButton = nullptr;
    storeButton = nullptr;
    monoMode = nullptr;
    lfoType = nullptr;
    programSelector = nullptr;
    aboutButton = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GlobalEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.drawImage(background, 0, 0, 864, 144, 0, 0, 1728, 288);
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    g.drawImage(imageLight, 300, 70, 14, 14, 0, monoMode->getToggleState() ? 28 : 0, 28, 28);
    g.drawImage(imageLight, 619, 102, 14, 14, 0, lfoSync->getToggleState() ? 28 : 0, 28, 28);
    g.drawImage(imageLight, 705, 102, 14, 14, 0, oscSync->getToggleState() ? 28 : 0, 28, 28);
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
}

void GlobalEditor::buttonClicked (juce::Button* buttonThatWasClicked)
{

    if (buttonThatWasClicked == aboutButton.get())
    {
        //[UserButtonCode_aboutButton] -- add your button handler code here..
        juce::ModifierKeys modifiers = juce::ModifierKeys::getCurrentModifiers();

        if ( modifiers.isCtrlDown() ) {
            ValueTreeDebugger *vtd = new ValueTreeDebugger(processor.rootVt);
            debugger.reset(vtd);
            return;
        }

        aboutBox = std::make_unique<AboutBox>();
        getParentComponent()->addAndMakeVisible(aboutBox.get());
        aboutBox->centreWithSize(aboutBox->getWidth(), aboutBox->getHeight());
        aboutBox->enterModalState(true,{});
        //[/UserButtonCode_aboutButton]
    }
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

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
#ifdef IMPLEMENT_MidiMonitor
    midiMonitor->repaint();
#endif //IMPLEMENT_MidiMonitor
}

void GlobalEditor::mouseDown(const MouseEvent &e) {
    if ( e.mods.isPopupMenu()) {
        PopupMenu popup;
        popup.addItem(1, "Send current program to DX7");

        auto p = popup.show();
        switch( p )
        {
        case 1:
           processor.sendCurrentSysexProgram();
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
          explicitFocusOrder="14" pos="564 50 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="lfoAmDepth" id="3e95a6206fa4a891" memberName="lfoAmDepth"
          virtualName="" explicitFocusOrder="19" pos="686 50 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="lfoPitchDepth" id="6ead769ca786c813" memberName="lfoPitchDepth"
          virtualName="" explicitFocusOrder="18" pos="646 50 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="lfoDelay" id="1fce68dc81619ef5" memberName="lfoDelay" virtualName=""
          explicitFocusOrder="15" pos="603 50 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="cutoff" id="40531f16bb0bd225" memberName="cutoff" virtualName=""
          explicitFocusOrder="6" pos="234 9 34 34" min="0.0" max="1.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="reso" id="c8c13464e81a8d83" memberName="reso" virtualName=""
          explicitFocusOrder="7" pos="278 9 34 34" min="0.0" max="1.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="pitchRate2" id="73f386b3c91d3de4" memberName="pitchRate2"
          virtualName="" explicitFocusOrder="24" pos="767 96 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchRate3" id="187c25b24413fccf" memberName="pitchRate3"
          virtualName="" explicitFocusOrder="26" pos="795 96 35 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchRate4" id="1aeb2a8fbbcbeaab" memberName="pitchRate4"
          virtualName="" explicitFocusOrder="28" pos="823 96 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchRate1" id="419d613b3fb9604e" memberName="pitchRate1"
          virtualName="" explicitFocusOrder="22" pos="739 96 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchLevel2" id="3a355ad2221887d9" memberName="pitchLevel2"
          virtualName="" explicitFocusOrder="23" pos="767 57 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchLevel3" id="5c5d782e8a7f3ad7" memberName="pitchLevel3"
          virtualName="" explicitFocusOrder="25" pos="795 56 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchLevel4" id="eb900b141fbad8ff" memberName="pitchLevel4"
          virtualName="" explicitFocusOrder="27" pos="823 56 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="pitchLevel1" id="344cbe26ec9fa128" memberName="pitchLevel1"
          virtualName="" explicitFocusOrder="21" pos="739 57 34 34" min="0.0"
          max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="transpose" id="7d1266b1c1534947" memberName="transpose"
          virtualName="" explicitFocusOrder="9" pos="202 60 34 34" min="0.0"
          max="48.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="oscSync" id="8f3fe641537cd00" memberName="oscSync" virtualName=""
                explicitFocusOrder="20" pos="650 96 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="pitchModSens" id="904f73df85a9f886" memberName="pitchModSens"
          virtualName="" explicitFocusOrder="17" pos="666 5 34 34" min="0.0"
          max="7.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="lfoSync" id="ff92bb0a5a4f7187" memberName="lfoSync" virtualName=""
                explicitFocusOrder="16" pos="565 96 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="pitchEnvDisplay" id="9ddaae8ef924a038" memberName="pitchEnvDisplay"
                    virtualName="" explicitFocusOrder="0" pos="751 10 93 30" class="PitchEnvDisplay"
                    params=""/>
  <GENERICCOMPONENT name="algoDisplay" id="b26fb9e3b5f0bc37" memberName="algoDisplay"
                    virtualName="" explicitFocusOrder="0" pos="335 30 152 91" class="AlgoDisplay"
                    params=""/>
  <SLIDER name="feedback" id="4fac1940c29ab8c" memberName="feedback" virtualName=""
          explicitFocusOrder="12" pos="501 81 34 34" min="0.0" max="7.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="algo" id="8a226ddf9bbff752" memberName="algo" virtualName=""
          explicitFocusOrder="11" pos="501 22 34 34" min="1.0" max="32.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="lcdDisplay" id="30c7bb8f114cbbe3" memberName="lcdDisplay"
                    virtualName="" explicitFocusOrder="0" pos="6 87 140 13" class="LcdDisplay"
                    params=""/>
  <SLIDER name="output" id="7697fdd54fd1593e" memberName="output" virtualName=""
          explicitFocusOrder="8" pos="157 60 34 34" min="0.0" max="1.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="vuOutput" id="dac75af912267f51" memberName="vuOutput" virtualName=""
                    explicitFocusOrder="0" pos="6 103 140 8" class="VuMeterOutput"
                    params=""/>
  <TEXTBUTTON name="initButton" id="92b278163c42e21d" memberName="initButton"
              virtualName="" explicitFocusOrder="3" pos="100 111 50 30" buttonText="INIT"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="parmButton" id="d5cd6260b34be817" memberName="parmButton"
              virtualName="" explicitFocusOrder="2" pos="52 111 50 30" buttonText="PARM"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="cartButton" id="465a101c3a0e744" memberName="cartButton"
              virtualName="" explicitFocusOrder="1" pos="3 111 50 30" buttonText="CART"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="storeButton" id="a94bec26832eb58b" memberName="storeButton"
              virtualName="" explicitFocusOrder="4" pos="270 109 50 30" buttonText="STORE"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TOGGLEBUTTON name="monoMode" id="e683d8cbe286367b" memberName="monoMode" virtualName=""
                explicitFocusOrder="10" pos="249 65 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="lfoType" id="62f61dda9f76c8f" memberName="lfoType" virtualName=""
                    explicitFocusOrder="13" pos="583 8 36 26" class="ComboBoxImage"
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
          explicitFocusOrder="5" pos="190 9 34 34" min="0.0" max="1.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

