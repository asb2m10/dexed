

#include "OperatorEditor.h"
#include "PluginEditor.h"
#include "component/OperatorDetails.h"
#include "util/ContextMenuAdapter.h"

#ifndef M_LN10
#define M_LN10 2.30258509299404568402
#endif

class OperatorSwitch : public ToggleButton {
    Image image;
public :
    OperatorSwitch(juce::String name) : ToggleButton(name) {
        SharedResourcePointer<DXLookNFeel> lookAndFeel;
        image = lookAndFeel->imageSwitchOperator;
        setSize(32, 32);
    }

    void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override {
        g.drawImage(image, 0, 0, 32, 32, 0, getToggleState() ? 0 : 64, 64, 64);
    }
};

//[/MiscUserDefs]


class OperatorContextMenu : public ContextMenuAdapter {
    int op;
public:
    OperatorContextMenu(DexedAudioProcessor &processor, int op) : ContextMenuAdapter(processor), op(op) {
    }

    void addSpecificItems(PopupMenu &menu, juce::Component *source) override {
        menu.addItem(1, "Copy Operator Values");
        menu.addItem(2, "Paste Envelope Values", DexedClipboard().isOperatorData());
        menu.addItem(3, "Paste Operator Values", DexedClipboard().isOperatorData());
        menu.addSeparator();
    }
};

//==============================================================================
OperatorEditor::OperatorEditor (DexedAudioProcessor &processor, int num) : processor(processor), internalOp(num) {
    SharedResourcePointer<DXLookNFeel> lookAndFeel;

    binder = std::make_unique<AudioComponentContainer>(*this, processor.parameters);
    opNum << num + 1;
    internalOp = 5-num;
    setTitle("Operator " + opNum);
    setFocusContainerType(FocusContainerType::focusContainer);

    std::unique_ptr<DXSlider> eglevel[4];
    std::unique_ptr<DXSlider> egrate[4];
    for (int i=0; i<4; i++) {
        eglevel[i] = std::make_unique<DXSlider>(IDs::egLevel.op(num).idx(i).name);
        egrate[i] = std::make_unique<DXSlider>(IDs::egRate.op(num).idx(i).name);
        eglevel[i]->setSliderStyle (juce::Slider::RotaryVerticalDrag);
        eglevel[i]->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
        egrate[i]->setSliderStyle (juce::Slider::RotaryVerticalDrag);
        egrate[i]->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    }

    eglevel[0]->setExplicitFocusOrder (4);
    eglevel[0]->setBounds (5, 128, 34, 34);
    binder->addAndAttach(std::move(eglevel[0]));
    eglevel[1]->setExplicitFocusOrder (6);
    eglevel[1]->setBounds (33, 128, 34, 34);
    binder->addAndAttach(std::move(eglevel[1]));
    eglevel[2]->setExplicitFocusOrder (8);
    eglevel[2]->setBounds (61, 128, 34, 34);
    binder->addAndAttach(std::move(eglevel[2]));
    eglevel[3]->setExplicitFocusOrder (10);
    eglevel[3]->setBounds (89, 128, 34, 34);
    binder->addAndAttach(std::move(eglevel[3]));
    egrate[0]->setExplicitFocusOrder (5);
    egrate[0]->setBounds (5, 169, 34, 34);
    binder->addAndAttach(std::move(egrate[0]));
    egrate[1]->setExplicitFocusOrder (7);
    egrate[1]->setBounds (33, 169, 34, 34);
    binder->addAndAttach(std::move(egrate[1]));
    egrate[2]->setExplicitFocusOrder (9);
    egrate[2]->setBounds (61, 169, 34, 34);
    binder->addAndAttach(std::move(egrate[2]));
    egrate[3]->setExplicitFocusOrder (11);
    egrate[3]->setBounds (89, 169, 34, 34);
    binder->addAndAttach(std::move(egrate[3]));

    auto opLevel = std::make_unique<DXSlider>(IDs::outputLevel.op(num).name);
    opLevel->setExplicitFocusOrder (15);
    opLevel->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    opLevel->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    opLevel->setBounds (245, 76, 34, 34);
    binder->addAndAttach(std::move(opLevel));

    auto opFine = std::make_unique<DXSlider>(IDs::frequencyFine.op(num).name);
    opFine->setExplicitFocusOrder (3);
    opFine->setSliderStyle  (juce::Slider::RotaryVerticalDrag);
    opFine->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    opFine->setBounds (78, 24, 34, 34);
    binder->addAndAttach(std::move(opFine));

    auto opCoarse = std::make_unique<DXSlider>(IDs::frequencyCoarse.op(num).name);
    opCoarse->setExplicitFocusOrder (2);
    opCoarse->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    opCoarse->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    opCoarse->setBounds (43, 24, 34, 34);
    binder->addAndAttach(std::move(opCoarse));

    auto detune = std::make_unique<DXSlider>(IDs::detune.op(num).name);
    detune->setExplicitFocusOrder (1);
    detune->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    detune->setTextBoxStyle (juce::Slider::NoTextBox, true, 80, 20);
    detune->setBounds (6, 24, 34, 34);
    binder->addAndAttach(std::move(detune));

    auto sclLeftLevel = std::make_unique<DXSlider>(IDs::lScaleDepth.op(num).name);
    sclLeftLevel->setTooltip (TRANS ("Keyboard Scale Level Left Depth "));
    sclLeftLevel->setExplicitFocusOrder (16);
    sclLeftLevel->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sclLeftLevel->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    sclLeftLevel->setBounds (131, 115, 34, 34);
    binder->addAndAttach(std::move(sclLeftLevel));

    auto sclRightLevel = std::make_unique<DXSlider>(IDs::rScaleDepth.op(num).name);
    sclRightLevel->setTooltip (TRANS ("Keyboard Scale Level Right Depth "));
    sclRightLevel->setExplicitFocusOrder (18);
    sclRightLevel->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sclRightLevel->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    sclRightLevel->setBounds (241, 115, 34, 34);
    binder->addAndAttach(std::move(sclRightLevel));

    auto sclLvlBrkPt = std::make_unique<DXSlider>(IDs::breakpoint.op(num).name);
    sclLvlBrkPt->setTooltip (TRANS ("Scale Level Breakpoint"));
    sclLvlBrkPt->setExplicitFocusOrder (17);
    sclLvlBrkPt->setBounds (178, 130, 54, 24);
    binder->addAndAttach(std::move(sclLvlBrkPt));

    auto sclRateScaling = std::make_unique<DXSlider>(IDs::rateScaling.op(num).name);
    sclRateScaling->setTooltip (TRANS ("Keyboard Rate Scaling"));
    sclRateScaling->setExplicitFocusOrder (20);
    sclRateScaling->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sclRateScaling->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    sclRateScaling->setBounds (186, 179, 34, 34);
    binder->addAndAttach(std::move(sclRateScaling));

    auto keyVelSens = std::make_unique<DXSlider>(IDs::keyVelocity.op(num).name);
    keyVelSens->setExplicitFocusOrder (14);
    keyVelSens->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    keyVelSens->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    keyVelSens->setBounds (204, 76, 34, 34);
    binder->addAndAttach(std::move(keyVelSens));

    auto ampModeSens = std::make_unique<DXSlider>(IDs::ampModeSens.op(num).name);
    ampModeSens->setExplicitFocusOrder (13);
    ampModeSens->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    ampModeSens->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    ampModeSens->setBounds (140, 76, 34, 34);
    binder->addAndAttach(std::move(ampModeSens));

    auto opMode = std::make_unique<juce::ToggleButton>(IDs::mode.op(num).name);
    opMode->setExplicitFocusOrder (12);
    opMode->setButtonText (juce::String());
    opMode->setBounds (146, 19, 48, 26);
    binder->addAndAttach(std::move(opMode));

    auto kbdLeftCurve = std::make_unique<ComboBoxImage>();
    kbdLeftCurve->setName(IDs::lKeyScale.op(internalOp).name);
    kbdLeftCurve->setExplicitFocusOrder (19);
    kbdLeftCurve->setBounds (128, 170, 36, 26);
    binder->attach(kbdLeftCurve.get());
    int posLeft[] = {0,5,4,3};
    kbdLeftCurve->setImage(lookAndFeel->imageScaling, posLeft);
    binder->add(std::move(kbdLeftCurve));

    auto kbdRightCurve = std::make_unique<ComboBoxImage>();
    kbdRightCurve->setName(IDs::rKeyScale.op(internalOp).name);
    kbdRightCurve->setExplicitFocusOrder (21);
    kbdRightCurve->setBounds (240, 170, 36, 26);
    binder->attach(kbdRightCurve.get());
    int posRight[] = {3,2,1,0};
    kbdRightCurve->setImage(lookAndFeel->imageScaling, posRight);
    binder->add(std::move(kbdRightCurve));

    auto operatorDetails = std::make_unique<OperatorDetails>(processor.parameters, num);
    operatorDetails->setBounds(15, 10, 95, 10);
    binder->add(std::move(operatorDetails));

    opSwitch = std::make_unique<OperatorSwitch>(IDs::on.op(5-internalOp).name);
    addAndMakeVisible(opSwitch.get());
    opSwitch->setTitle("Operator switch");
    opSwitch->setBounds(226, 13, 64, 32);
    binder->attach(opSwitch.get());

    envDisplay = std::make_unique<EnvDisplay>(processor.parameters, num);
    addAndMakeVisible (envDisplay.get());
    envDisplay->setBounds (16, 83, 94, 30);

    vu = std::make_unique<VuMeter>();
    addAndMakeVisible (vu.get());
    vu->setBounds (132, 52, 140, 8);

    light = lookAndFeel->imageLight;
    background = lookAndFeel->imageOperator;

    contextMenuAdapter = std::make_unique<OperatorContextMenu>(processor, internalOp);
    addMouseListener(contextMenuAdapter.get(), true);
    setWantsKeyboardFocus(true);
}

OperatorEditor::~OperatorEditor() {
    envDisplay = nullptr;
    vu = nullptr;
    contextMenuAdapter = nullptr;
}

//==============================================================================
void OperatorEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    g.drawImage(background, 0, 0, 287, 218, 0, 0, 574, 436);
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..

    if ( opSwitch->getToggleState() )
        g.setColour(Colours::white);
    else
        g.setColour(DXLookNFeel::roundBackground);

    g.setFont(juce::Font(juce::FontOptions().withHeight(30.00f).withStyle("Regular")));
    g.drawText(opNum, 250, 14, 30, 30, Justification::centred, true);

    //bool state = opMode->getToggleState();

    // 129 x 24
    //    g.drawImage(light, 127, 24, 14, 14, 0, state ? 0 : 28, 28, 28);
    // 199 x 24
    // g.drawImage(light, 198, 24, 14, 14, 0, !state ? 0 : 28, 28, 28);
}

void OperatorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OperatorEditor::bind(DexedAudioProcessor *parent, int op) {
    opNum << op + 1;
    internalOp = 5-op;
    setTitle("Operator " + opNum);
    setFocusContainerType(FocusContainerType::focusContainer);
}

void OperatorEditor::updateGain(float v) {
    vu->v = v;
    vu->repaint();
}

void OperatorEditor::updateDisplay() {
}


void OperatorEditor::updateEnvPos(char pos) {
    envDisplay->vPos = pos;
    envDisplay->repaint();
}

void OperatorEditor::mouseDown(const MouseEvent &event) {

}

void OperatorEditor::toggleOpSwitch() {
    opSwitch->setToggleState(!opSwitch->getToggleState(), dontSendNotification);
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
          explicitFocusOrder="4" pos="5 128 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egl2" id="66f5195e9c374029" memberName="s_egl2" virtualName=""
          explicitFocusOrder="6" pos="33 128 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egl3" id="9d57bd53203dcdb4" memberName="s_egl3" virtualName=""
          explicitFocusOrder="8" pos="61 128 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egl4" id="4f7c3ece3ea2cf9c" memberName="s_egl4" virtualName=""
          explicitFocusOrder="10" pos="89 128 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egr1" id="2ca8137d80da46fb" memberName="s_egv1" virtualName=""
          explicitFocusOrder="5" pos="5 169 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egr3" id="4ad6d0c532d15973" memberName="s_egv2" virtualName=""
          explicitFocusOrder="7" pos="33 169 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egr3" id="8a2027f9ede16b4f" memberName="s_egv3" virtualName=""
          explicitFocusOrder="9" pos="61 169 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="egr4" id="8c04f1c943d837e8" memberName="s_egv4" virtualName=""
          explicitFocusOrder="11" pos="89 169 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="opLevel" id="f8521c8214fb8993" memberName="opLevel" virtualName=""
          explicitFocusOrder="15" pos="245 76 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="opFine" id="e445aa61bd6cddcb" memberName="opFine" virtualName=""
          explicitFocusOrder="3" pos="78 24 34 34" min="0.0" max="99.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <SLIDER name="opCoarse" id="4eec63d30d7488d2" memberName="opCoarse" virtualName=""
          explicitFocusOrder="2" pos="43 24 34 34" min="0.0" max="31.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="khz" id="eb961eed8902a6fc" memberName="khzDisplay" virtualName=""
         explicitFocusOrder="0" pos="15 10 95 10" bkgCol="6a000000" textCol="ffffffff"
         outlineCol="0" edTextCol="ff000000" edBkgCol="0" labelText="1,000 kHz"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="12.6" kerning="0.0" bold="0"
         italic="0" justification="36"/>
  <SLIDER name="detune" id="f093ec8defca2fc2" memberName="detune" virtualName=""
          explicitFocusOrder="1" pos="6 24 34 34" min="-7.0" max="7.0"
          int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="0"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <GENERICCOMPONENT name="envDisplay" id="b18856de924c6340" memberName="envDisplay"
                    virtualName="" explicitFocusOrder="0" pos="16 83 94 30" class="EnvDisplay"
                    params=""/>
  <SLIDER name="sclLeftLevel" id="bd6f338ae68e454f" memberName="sclLeftLevel"
          virtualName="" explicitFocusOrder="16" pos="131 115 34 34" tooltip="Keyboard Scale Level Left Depth "
          min="0.0" max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="sclRightLevel" id="b9e23ed5187fc7e5" memberName="sclRightLevel"
          virtualName="" explicitFocusOrder="18" pos="241 115 34 34" tooltip="Keyboard Scale Level Right Depth "
          min="0.0" max="99.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="sclLvlBrkPt" id="c563ac3116923bbc" memberName="sclLvlBrkPt"
          virtualName="" explicitFocusOrder="17" pos="178 130 54 24" tooltip="Scale Level Breakpoint"
          min="0.0" max="99.0" int="1.0" style="LinearHorizontal" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="sclRateScaling" id="f0d17c8e09aa4c49" memberName="sclRateScaling"
          virtualName="" explicitFocusOrder="20" pos="186 179 34 34" tooltip="Keyboard Rate Scaling"
          min="0.0" max="7.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="keyVelSens" id="21795d045d07602b" memberName="keyVelSens"
          virtualName="" explicitFocusOrder="14" pos="204 76 34 34" min="0.0"
          max="7.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="ampModSens" id="634ceaa7b0f81a6c" memberName="ampModSens"
          virtualName="" explicitFocusOrder="13" pos="140 76 34 34" min="0.0"
          max="3.0" int="1.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <GENERICCOMPONENT name="vu" id="6f952594ea99dc1e" memberName="vu" virtualName=""
                    explicitFocusOrder="0" pos="132 52 140 8" class="VuMeter" params=""/>
  <TOGGLEBUTTON name="opMode" id="15db8e5c9dd13966" memberName="opMode" virtualName=""
                explicitFocusOrder="12" pos="146 19 48 26" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="kbdLeftCurve" id="ee4e60dfcd6cb790" memberName="kbdLeftCurve"
                    virtualName="" explicitFocusOrder="19" pos="128 170 36 26" class="ComboBoxImage"
                    params=""/>
  <GENERICCOMPONENT name="kbdRightCurve" id="f5ca44d67593488a" memberName="kbdRightCurve"
                    virtualName="" explicitFocusOrder="21" pos="240 170 36 26" class="ComboBoxImage"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

