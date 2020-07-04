/**
 *
 * Copyright (c) 2013-2017 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

#include <time.h>
#include <stdlib.h>

#include "PluginParam.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Dexed.h"

// Async updater
class CtrlUpdate : public CallbackMessage {
    Ctrl *ctrl;
    float value;
public:
    CtrlUpdate(Ctrl *ctrl, float value) {
        this->ctrl = ctrl;
        this->value = value;
    }
    void messageCallback() {
        ctrl->publishValue(value);
    }
};

// ************************************************************************
// Custom displays

class CtrlDXLabel : public CtrlDX {
   StringArray labels;
public:
    CtrlDXLabel(String name, int steps, int offset, StringArray &labels) : CtrlDX(name, steps, offset, 0) {
        this->labels = labels;
    };
    
    String getValueDisplay() {
        return labels[getValue()];
    }
};

class CtrlDXTranspose : public CtrlDX {
public:
    CtrlDXTranspose(String name, int steps, int offset) : CtrlDX(name, steps, offset, 0) {
    };
    
    String getValueDisplay() {
        String ret ;
        int value = getValue();
        return ret << (value - 24);
#if 0        
        switch(value % 12) {
            case 0: ret << "C"; break;
            case 1: ret << "C#"; break;
            case 2: ret << "D"; break;
            case 3: ret << "D#"; break;
            case 4: ret << "E"; break;
            case 5: ret << "F"; break;
            case 6: ret << "F#"; break;
            case 7: ret << "G"; break;
            case 8: ret << "G#"; break;
            case 9: ret << "A"; break;
            case 10: ret << "A#"; break;
            case 11: ret << "B"; break;
        }
        return ret << (value/12+1);
#endif        
    }
};

class CtrlDXSwitch : public CtrlDX {
public:
    CtrlDXSwitch(String name, int steps, int offset) : CtrlDX(name, steps, offset, 0) {
    };
    
    String getValueDisplay() {
        return getValue() ? String("ON") : String("OFF");
    }
};

class CtrlDXOpMode : public CtrlDX {
public:
    CtrlDXOpMode(String name, int steps, int offset) : CtrlDX(name, steps, offset, 0) {
    };
    
    String getValueDisplay() {
        return getValue() ? String("FIXED") : String("RATIO");
    }
};

class CtrlDXBreakpoint : public CtrlDX {
public:
    CtrlDXBreakpoint(String name, int steps, int offset) : CtrlDX(name, steps, offset, 0) {
    };
    
    String getValueDisplay() {
        const char *breakNames[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
        String ret;
        ret << breakNames[getValue()%12] << (getValue()+9) / 12 - 1;
        return ret;
    }
};

class CtrlTune : public Ctrl {
public:
    DexedAudioProcessor *processor;
    
    CtrlTune(String name, DexedAudioProcessor *owner) : Ctrl(name) {
        processor = owner;
    }
    
    float getValueHost() {
        // meh. good enough for now
        int32_t tune = processor->controllers.masterTune / (1.0/12);
        tune = (tune >> 11) + 0x2000;
        return (float)tune / 0x4000;
    }
    
    void setValueHost(float v) {
        int32_t tune = (v * 0x4000) - 0x2000;
        processor->controllers.masterTune = ((float) (tune << 11)) * (1.0/12);
    }
    
    String getValueDisplay() {
        String display;
        display << (getValueHost() * 2) -1;
        return display;
    }
    
    void updateComponent() {
        if (slider != NULL) {
            slider->setValue(getValueHost(), dontSendNotification);
        }
    }
};

class CtrlOpSwitch : public Ctrl {
    DexedAudioProcessor *processor;
    char *value;
public :
    CtrlOpSwitch(String name, char *switchValue, DexedAudioProcessor *owner) : Ctrl(name) {
        processor = owner;
        value = switchValue;
    }
    
    void setValueHost(float f) {
        if ( f == 0 )
            *value = '0';
        else
            *value = '1';
        updateDisplayName();
        
        // the value is based on the controller
        parent->setDxValue(155, -1);
    }
    
    float getValueHost() {
        if ( *value == '0' )
            return 0;
        else
            return 1;
    }
    
    String getValueDisplay() {
        String ret;
        ret << label << " " << (*value == '0' ? "OFF" : "ON");
        return ret;
    }
    
    void updateComponent() {
        if (button != NULL) {
            if (*value == '0') {
                button->setToggleState(false, dontSendNotification);
            } else {
                button->setToggleState(true, dontSendNotification);
            }
        }
    }
    
    void updateDisplayName() {
        DexedAudioProcessorEditor *editor = (DexedAudioProcessorEditor *) parent->getActiveEditor();
        if ( editor == NULL ) {
            return;
        }
        editor->global.setParamMessage(getValueDisplay());
    }
};

// ************************************************************************
//
Ctrl::Ctrl(String name) {
    label << name;
    slider = NULL;
    button = NULL;
    comboBox = NULL;
}

void Ctrl::bind(Slider *s) {
    slider = s;
    updateComponent();
    s->addListener(this);
    s->addMouseListener(this, true);
    s->setVelocityModeParameters (0.1, 1, 0.05, 1, ModifierKeys::shiftModifier);
}

void Ctrl::bind(Button *b) {
    button = b;
    updateComponent();
    b->addListener(this);
    b->addMouseListener(this, true);
}

void Ctrl::bind(ComboBox *c) {
    comboBox = c;
    updateComponent();
    c->addListener(this);
    c->addMouseListener(this, true);
}

void Ctrl::unbind() {
    if (slider != NULL) {
        slider->removeListener(this);
        slider->removeMouseListener(this);
        slider = NULL;
    }

    if (button != NULL) {
        button->removeListener(this);
        button->removeMouseListener(this);
        button = NULL;
    }

    if (comboBox != NULL) {
        comboBox->removeListener(this);
        comboBox->removeMouseListener(this);
        comboBox = NULL;
    }
}

void Ctrl::publishValueAsync(float value) {
    CtrlUpdate *update = new CtrlUpdate(this, value);
    update->post();
}

void Ctrl::publishValue(float value) {
    parent->beginParameterChangeGesture(idx);
    parent->setParameterNotifyingHost(idx, value);
    parent->endParameterChangeGesture(idx);
}

void Ctrl::sliderValueChanged(Slider* moved) {
    publishValue(moved->getValue());
}

void Ctrl::buttonClicked(Button* clicked) {
    publishValue(clicked->getToggleState());
}

void Ctrl::comboBoxChanged(ComboBox* combo) {
    publishValue((combo->getSelectedId() - 1) / combo->getNumItems());
}

void Ctrl::mouseEnter(const juce::MouseEvent &event) {
    updateDisplayName();
}

void Ctrl::mouseDown(const juce::MouseEvent &event) {
    if ( event.mods.isPopupMenu()) {
        PopupMenu popup;

        if ( parent->mappedMidiCC.containsValue(this) ) {
            popup.addItem(3, "Re-Map controller to midi CC for: " + String(label));
            popup.addSeparator();
            popup.addItem(1, "Remove midi CC mapping for this controller");
        } else {
            popup.addItem(3, "Map controller to midi CC for: " + String(label));
            popup.addSeparator();
        }
        popup.addItem(2, "Clear midi CC mapping");

        switch(popup.show()) {
            case 1:
                parent->mappedMidiCC.removeValue(this);
                parent->savePreference();
                break;
            case 2:
                if ( AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon, "Confirm", "Clear midi mapping for all controller change (CC) messages?", "YES", "NO", "CANCEL") ) {
                    parent->mappedMidiCC.clear();
                    parent->savePreference();
                }
                break;
            case 3:
                AudioProcessorEditor *editor = parent->getActiveEditor();
                if ( editor == NULL ) {
                    return;
                }
                DexedAudioProcessorEditor *dexedEditor = (DexedAudioProcessorEditor *) editor;
                dexedEditor->discoverMidiCC(this);
                break;
        }
    }
}

void Ctrl::updateDisplayName() {
}

// ************************************************************************
// CtrlFloat - control float values
CtrlFloat::CtrlFloat(String name, float *storageValue) : Ctrl(name) {
    vPointer = storageValue;
}

float CtrlFloat::getValueHost() {
    return *vPointer;
}

void CtrlFloat::setValueHost(float v) {
    *vPointer = v;
}

String CtrlFloat::getValueDisplay() {
    String display;
    display << *vPointer;
    return display;
}

void CtrlFloat::updateComponent() {
    if (slider != NULL) {
        slider->setValue(*vPointer, dontSendNotification);
    }
}

// ************************************************************************
// CtrlDX - control DX mapping
CtrlDX::CtrlDX(String name, int steps, int offset, int displayValue) : Ctrl(name) {
    this->displayValue = displayValue;
    this->steps = steps;
    dxValue = 0;
    dxOffset = offset;
}

float CtrlDX::getValueHost() {
    return getValue() / (float) steps;
}

void CtrlDX::setValueHost(float f) {
    setValue(roundToInt(f * steps));
}

void CtrlDX::setValue(int v) {
    TRACE("setting value %d %d", dxOffset, v);
    dxValue = v;
    if (dxOffset >= 0) {
        if (parent != NULL)
            parent->setDxValue(dxOffset, dxValue);
    }
}

int CtrlDX::getValue() {
    if (dxOffset >= 0)
        dxValue = parent->data[dxOffset];
    return dxValue;
}

int CtrlDX::getOffset() {
    return dxOffset;
}

String CtrlDX::getValueDisplay() {
    String ret;
    ret << ( getValue() + displayValue );
    return ret;
}

void CtrlDX::updateDisplayName() {
    DexedAudioProcessorEditor *editor = (DexedAudioProcessorEditor *) parent->getActiveEditor();
    if ( editor == NULL ) {
        return;
    }
    String msg;
    msg << label << " = " << getValueDisplay();
    editor->global.setParamMessage(msg);
}


void CtrlDX::publishValue(float value) {
    Ctrl::publishValue(value / steps);
    updateDisplayName();
}

void CtrlDX::sliderValueChanged(Slider* moved) {
    publishValue(((int) moved->getValue() - displayValue));
}

void CtrlDX::comboBoxChanged(ComboBox* combo) {
    publishValue(combo->getSelectedId() - 1);
}

void CtrlDX::buttonClicked(Button *button) {
    publishValue((int) button->getToggleState());
}

void CtrlDX::updateComponent() {
    if (slider != NULL) {
        slider->setValue(getValue() + displayValue,
                dontSendNotification);
    }

    if (button != NULL) {
        if (getValue() == 0) {
            button->setToggleState(false, dontSendNotification);
        } else {
            button->setToggleState(true, dontSendNotification);
        }
    }

    if (comboBox != NULL) {
        int cvalue = getValue() + 1;
        if (comboBox->getNumItems() <= cvalue) {
            cvalue = comboBox->getNumItems();
        }
        comboBox->setSelectedId(cvalue, dontSendNotification);
    }
}

/***************************************************************
 *
 */
void DexedAudioProcessor::initCtrl() {
    setupStartupCart();
    currentProgram = 0;
    
    fxCutoff = new CtrlFloat("Cutoff", &fx.uiCutoff);
    ctrl.add(fxCutoff);
    
    fxReso = new CtrlFloat("Resonance", &fx.uiReso);
    ctrl.add(fxReso);
    
    output = new CtrlFloat("Output", &fx.uiGain);
    ctrl.add(output);
    
    tune = new CtrlTune("MASTER TUNE ADJ", this);
    ctrl.add(tune);
    
    algo = new CtrlDX("ALGORITHM", 31, 134, 1);
    ctrl.add(algo);
    
    feedback = new CtrlDX("FEEDBACK", 7, 135);
    ctrl.add(feedback);
    
    oscSync = new CtrlDXSwitch("OSC KEY SYNC", 1, 136);
    ctrl.add(oscSync);
    
    lfoRate = new CtrlDX("LFO SPEED", 99, 137);
    ctrl.add(lfoRate);
    
    lfoDelay = new CtrlDX("LFO DELAY", 99, 138);
    ctrl.add(lfoDelay);
    
    lfoPitchDepth = new CtrlDX("LFO PM DEPTH", 99, 139);
    ctrl.add(lfoPitchDepth);
    
    lfoAmpDepth = new CtrlDX("LFO AM DEPTH", 99, 140);
    ctrl.add(lfoAmpDepth);
    
    lfoSync = new CtrlDXSwitch("LFO KEY SYNC", 1, 141);
    ctrl.add(lfoSync);
    
    StringArray lbl;
    lbl.add("TRIANGE");
    lbl.add("SAW DOWN");
    lbl.add("SAW UP");
    lbl.add("SQUARE");
    lbl.add("SINE");
    lbl.add("S&HOLD");
    
    lfoWaveform = new CtrlDXLabel("LFO WAVE", 5, 142, lbl);
    ctrl.add(lfoWaveform);
    
    transpose = new CtrlDXTranspose("TRANSPOSE", 48, 144);
    ctrl.add(transpose);
    
    pitchModSens = new CtrlDX("P MODE SENS.", 7, 143);
    ctrl.add(pitchModSens);
    
    for (int i=0;i<4;i++) {
        String rate;
        rate << "PITCH EG RATE " << (i+1);
        pitchEgRate[i] = new CtrlDX(rate, 99, 126+i);
        ctrl.add(pitchEgRate[i]);
    }

    for (int i=0;i<4;i++) {
        String level;
        level << "PITCH EG LEVEL " << (i+1);
        pitchEgLevel[i] = new CtrlDX(level, 99, 130+i);
        ctrl.add(pitchEgLevel[i]);
    }
    
    StringArray keyScaleLabels;
    keyScaleLabels.add("-LN");
    keyScaleLabels.add("-EX");
    keyScaleLabels.add("+EX");
    keyScaleLabels.add("+LN");
    
    // fill operator values;
    for (int i = 0; i < 6; i++) {
        //// In the Sysex, OP6 comes first, then OP5...
        int opTarget = (5-i) * 21;
        int opVal = i;
        String opName;
        opName << "OP" << (opVal + 1);

        for (int j = 0; j < 4; j++) {     
            String opRate;
            opRate << opName << " EG RATE " << (j + 1);
            opCtrl[opVal].egRate[j] = new CtrlDX(opRate, 99, opTarget + j);
            ctrl.add(opCtrl[opVal].egRate[j]);
        }
    
        for (int j = 0; j < 4; j++) {        
            String opLevel;
            opLevel << opName << " EG LEVEL " << (j + 1);
            opCtrl[opVal].egLevel[j] = new CtrlDX(opLevel, 99, opTarget + j + 4);
            ctrl.add(opCtrl[opVal].egLevel[j]);
        }
    
        String opVol;
        opVol << opName << " OUTPUT LEVEL";
        opCtrl[opVal].level = new CtrlDX(opVol, 99, opTarget + 16);
        ctrl.add(opCtrl[opVal].level);

        String opMode;
        opMode << opName << " MODE";
        opCtrl[opVal].opMode = new CtrlDXOpMode(opMode, 1, opTarget + 17);
        ctrl.add(opCtrl[opVal].opMode);

        String coarse;
        coarse << opName << " F COARSE";
        opCtrl[opVal].coarse = new CtrlDX(coarse, 31, opTarget + 18);
        ctrl.add(opCtrl[opVal].coarse);

        String fine;
        fine << opName << " F FINE";
        opCtrl[opVal].fine = new CtrlDX(fine, 99, opTarget + 19);
        ctrl.add(opCtrl[opVal].fine);

        String detune;
        detune << opName << " OSC DETUNE";
        opCtrl[opVal].detune = new CtrlDX(detune, 14, opTarget + 20, -7);
        ctrl.add(opCtrl[opVal].detune);

        String sclBrkPt;
        sclBrkPt << opName << " BREAK POINT";
        opCtrl[opVal].sclBrkPt = new CtrlDXBreakpoint(sclBrkPt, 99, opTarget + 8);
        ctrl.add(opCtrl[opVal].sclBrkPt);

        String sclLeftDepth;
        sclLeftDepth << opName << " L SCALE DEPTH";
        opCtrl[opVal].sclLeftDepth = new CtrlDX(sclLeftDepth, 99, opTarget + 9);
        ctrl.add(opCtrl[opVal].sclLeftDepth);

        String sclRightDepth;
        sclRightDepth << opName << " R SCALE DEPTH";
        opCtrl[opVal].sclRightDepth = new CtrlDX(sclRightDepth, 99, opTarget + 10);
        ctrl.add(opCtrl[opVal].sclRightDepth);

        String sclLeftCurve;
        sclLeftCurve << opName << " L KEY SCALE";
        opCtrl[opVal].sclLeftCurve = new CtrlDXLabel(sclLeftCurve, 3, opTarget + 11, keyScaleLabels);
        ctrl.add(opCtrl[opVal].sclLeftCurve);

        String sclRightCurve;
        sclRightCurve << opName << " R KEY SCALE";
        opCtrl[opVal].sclRightCurve = new CtrlDXLabel(sclRightCurve, 3, opTarget + 12, keyScaleLabels);
        ctrl.add(opCtrl[opVal].sclRightCurve);

        String sclRate;
        sclRate << opName << " RATE SCALING";
        opCtrl[opVal].sclRate = new CtrlDX(sclRate, 7, opTarget + 13);
        ctrl.add(opCtrl[opVal].sclRate);

        String ampModSens;
        ampModSens << opName << " A MOD SENS.";
        opCtrl[opVal].ampModSens = new CtrlDX(ampModSens, 3, opTarget + 14);
        ctrl.add(opCtrl[opVal].ampModSens);

        String velModSens;
        velModSens << opName << " KEY VELOCITY";
        opCtrl[opVal].velModSens = new CtrlDX(velModSens, 7, opTarget + 15);
        ctrl.add(opCtrl[opVal].velModSens);
        
        String opSwitchLabel;
        opSwitchLabel << opName << " SWITCH";
        opCtrl[opVal].opSwitch = new CtrlOpSwitch(opSwitchLabel, (char *)&(controllers.opSwitch)+(5-i), this);
        ctrl.add(opCtrl[opVal].opSwitch);
    }
    
    for (int i=0; i < ctrl.size(); i++) {
        ctrl[i]->idx = i;
        ctrl[i]->parent = this;
    }
}

void DexedAudioProcessor::setDxValue(int offset, int v) {
    if (offset < 0)
        return;

    if ( offset == 155 ) {
        // used on op switch that are not part of a Sysex packed cartridge, we render it
        // ourselves.
        packOpSwitch();
        v = data[155];
    } else if ( data[offset] != v ) {
        TRACE("setting dx %d %d", offset, v);
        data[offset] = v;
    } else {
        TRACE("ignoring dx7 same values %d %d", offset, v);
        return;
    }

    refreshVoice = true;

    // MIDDLE C (transpose)
    if (offset == 144)
        panic();
    
    if (!sendSysexChange)
        return;
    
    uint8 msg[7] = { 0xF0, 0x43, 0x10, offset > 127, 0, (uint8) v, 0xF7 };
    msg[2] = 0x10 | sysexComm.getChl();
    msg[4] = offset & 0x7F;
    
    if ( sysexComm.isOutputActive() ) {
        //TRACE("SENDING SYSEX: %.2X%.2X %.2X%.2X %.2X%.2X %.2X", msg[0], msg[1], msg[2], msg[3], msg[4], msg[5], msg[6]);
        sysexComm.send(MidiMessage(msg,7));
    }
}

void DexedAudioProcessor::unbindUI() {
    for (int i = 0; i < ctrl.size(); i++) {
        ctrl[i]->unbind();
    }
}

//==============================================================================
int DexedAudioProcessor::getNumParameters() {
    return ctrl.size();
}

float DexedAudioProcessor::getParameter(int index) {
    return ctrl[index]->getValueHost();
}

void DexedAudioProcessor::setParameter(int index, float newValue) {
    ctrl[index]->setValueHost(newValue);
}

int DexedAudioProcessor::getNumPrograms() {
    return 32;
}

int DexedAudioProcessor::getCurrentProgram() {
    return currentProgram;
}

void DexedAudioProcessor::setCurrentProgram(int index) {
    TRACE("setting program %d state", index);

    if ( lastStateSave + 2 > time(NULL) ) {
        TRACE("skipping save, storage recall to close");
        return;
    }
    
    panic();
    
    index = index > 31 ? 31 : index;
    currentCart.unpackProgram(data, index);
    unpackOpSwitch(0x3F);
    lfo.reset(data + 137);
    currentProgram = index;
    triggerAsyncUpdate();
    
    // reset parameter display
    DexedAudioProcessorEditor *editor = (DexedAudioProcessorEditor *) getActiveEditor();
    if ( editor == NULL ) {
        return;
    }
    editor->global.setParamMessage("");
    
    panic();
}

const String DexedAudioProcessor::getProgramName(int index) {
    if (index >= 32)
        index = 31;
    return programNames[index];
}

void DexedAudioProcessor::changeProgramName(int index, const String& newName) {
}

const String DexedAudioProcessor::getParameterName(int index) {
    return ctrl[index]->label;
}

const String DexedAudioProcessor::getParameterText(int index) {
    return ctrl[index]->getValueDisplay();
}

void DexedAudioProcessor::loadPreference() {
    File propFile = DexedAudioProcessor::dexedAppDir.getChildFile("Dexed.xml");
    PropertiesFile::Options prefOptions;
    PropertiesFile prop(propFile, prefOptions);
    
    if ( ! prop.isValidFile() ) {
        return;
    }
    
    if ( prop.containsKey( String("normalizeDxVelocity") ) ) {
        normalizeDxVelocity = prop.getIntValue( String("normalizeDxVelocity") );
    }
    
    if ( prop.containsKey( String("pitchRange") ) ) {
        controllers.values_[kControllerPitchRangeUp] = prop.getIntValue( String("pitchRange") );
    }
    
    if ( prop.containsKey( String("pitchRangeDn") ) ) {
        controllers.values_[kControllerPitchRangeDn] = prop.getIntValue( String("pitchRangeDn") );
    } else {
        controllers.values_[kControllerPitchRangeDn] = controllers.values_[kControllerPitchRangeUp];
    }
    
    if ( prop.containsKey( String("pitchStep") ) ) {
        controllers.values_[kControllerPitchStep] = prop.getIntValue( String("pitchStep") );
    }
    
    if ( prop.containsKey( String("sysexIn") ) ) {
        sysexComm.setInput( prop.getValue("sysexIn") );
    }
    
    if ( prop.containsKey( String("sysexOut") ) ) {
        sysexComm.setOutput( prop.getValue("sysexOut") );
    }
    
    if ( prop.containsKey( String("sysexChl") ) ) {
        sysexComm.setChl( prop.getIntValue( String("sysexChl") ) );
    }
    
    if ( prop.containsKey( String("engineType") ) ) {
        setEngineType(prop.getIntValue(String("engineType")));
    }

    if ( prop.containsKey( String("showKeyboard") ) ) {
        showKeyboard = prop.getIntValue( String("showKeyboard") );
    }

    if ( prop.containsKey( String("wheelMod") ) ) {
        controllers.wheel.parseConfig(prop.getValue(String("wheelMod")).toRawUTF8());
    }
    
    if ( prop.containsKey( String("footMod") ) ) {
        controllers.foot.parseConfig(prop.getValue(String("footMod")).toRawUTF8());
    }
    
    if ( prop.containsKey( String("breathMod") ) ) {
        controllers.breath.parseConfig(prop.getValue(String("breathMod")).toRawUTF8());
    }
    
    if ( prop.containsKey( String("aftertouchMod") ) ) {
        controllers.at.parseConfig(prop.getValue(String("aftertouchMod")).toRawUTF8());
    }
    
    if ( prop.containsKey( String("dpiScaleFactor") ) ) {
        dpiScaleFactor = prop.getDoubleValue(String("dpiScaleFactor"));
    }
    
    controllers.refresh();
}

void DexedAudioProcessor::savePreference() {
    File propFile = DexedAudioProcessor::dexedAppDir.getChildFile("Dexed.xml");
    PropertiesFile::Options prefOptions;
    PropertiesFile prop(propFile, prefOptions);
    
    prop.setValue(String("normalizeDxVelocity"), normalizeDxVelocity);
    prop.setValue(String("pitchRange"), controllers.values_[kControllerPitchRangeUp]); // for backwards compat
    prop.setValue(String("pitchRangeUp"), controllers.values_[kControllerPitchRangeUp]);
    prop.setValue(String("pitchRangeDn"), controllers.values_[kControllerPitchRangeDn]);
    prop.setValue(String("pitchStep"), controllers.values_[kControllerPitchStep]);
    
    prop.setValue(String("sysexIn"), sysexComm.getInput());
    prop.setValue(String("sysexOut"), sysexComm.getOutput());
    prop.setValue(String("sysexChl"), sysexComm.getChl());
    
    prop.setValue(String("showKeyboard"), showKeyboard);

    char mod_cfg[15];
    controllers.wheel.setConfig(mod_cfg);
    prop.setValue(String("wheelMod"), mod_cfg);
    controllers.foot.setConfig(mod_cfg);
    prop.setValue(String("footMod"), mod_cfg);
    controllers.breath.setConfig(mod_cfg);
    prop.setValue(String("breathMod"), mod_cfg);
    controllers.at.setConfig(mod_cfg);
    prop.setValue(String("aftertouchMod"), mod_cfg);
    
    prop.setValue(String("engineType"), (int) engineType);
    prop.setValue(String("dpiScaleFactor"), dpiScaleFactor);
    
    prop.save();
}

