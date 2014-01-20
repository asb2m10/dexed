/**
 *
 * Copyright (c) 2013 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 */

#include <time.h>

#include "PluginParam.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"



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
}

void Ctrl::bind(Button *b) {
    button = b;
    updateComponent();
    b->addListener(this);
}

void Ctrl::bind(ComboBox *c) {
    comboBox = c;
    updateComponent();
    c->addListener(this);
}

void Ctrl::unbind() {
    if (slider != NULL) {
        slider->removeListener(this);
        slider = NULL;
    }

    if (button != NULL) {
        button->removeListener(this);
        button = NULL;
    }

    if (comboBox != NULL) {
        comboBox->removeListener(this);
        comboBox = NULL;
    }
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
    publishValue(clicked->getToggleStateValue() == 1 ? 1 : 0);
}

void Ctrl::comboBoxChanged(ComboBox* combo) {
    publishValue((combo->getSelectedId() - 1) / combo->getNumItems());
}

// ************************************************************************
// CtrlDX - control DX mapping
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
        slider->setValue(*vPointer, NotificationType::dontSendNotification);
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
    return dxValue / steps;
}

void CtrlDX::setValueHost(float f) {
    setValue((f * steps));
}

void CtrlDX::setValue(int v) {
    if (v >= steps) {
        TRACE("WARNING: value too big %s : %d", label.toRawUTF8(), v);
        v = steps - 1;
    }
    dxValue = v;
    if (dxOffset >= 0) {
        if (parent != NULL)
            parent->setDxValue(dxOffset, v);
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

void CtrlDX::publishValue(float value) {
    Ctrl::publishValue(value / steps);

    DexedAudioProcessorEditor *editor = (DexedAudioProcessorEditor *) parent->getActiveEditor();
    if ( editor == NULL )
        return;
    String msg;
    msg << label << " = " << getValueDisplay();
    editor->global.setParamMessage(msg);
}

void CtrlDX::sliderValueChanged(Slider* moved) {
    publishValue(((int) moved->getValue() - displayValue));
}

void CtrlDX::comboBoxChanged(ComboBox* combo) {
    publishValue(combo->getSelectedId() - 1);
}

void CtrlDX::updateComponent() {
    if (slider != NULL) {
        slider->setValue(getValue() + displayValue,
                NotificationType::dontSendNotification);
    }

    if (button != NULL) {
        if (getValue() == 0) {
            button->setToggleState(false,
                    NotificationType::dontSendNotification);
        } else {
            button->setToggleState(true,
                    NotificationType::dontSendNotification);
        }
    }

    if (comboBox != NULL) {
        int cvalue = getValue() + 1;
        if (comboBox->getNumItems() <= cvalue) {
            cvalue = comboBox->getNumItems();
        }
        comboBox->setSelectedId(cvalue, NotificationType::dontSendNotification);
    }
}

/***************************************************************
 *
 */
void DexedAudioProcessor::initCtrl() {
    MemoryInputStream *mis = new MemoryInputStream(BinaryData::builtin_pgm_zip, BinaryData::builtin_pgm_zipSize, false);
    builtin_pgm = new ZipFile(mis, true);
    builtin_pgm->sortEntriesByFilename();
    
    loadBuiltin(0);
    
    currentProgram = 0;
    
    fxCutoff = new CtrlFloat("Cutoff", &fx.uiCutoff);
    ctrl.add(fxCutoff);
    
    fxReso = new CtrlFloat("Resonance", &fx.uiReso);
    ctrl.add(fxReso);
    
    algo = new CtrlDX("ALGORITHM", 32, 134, 1);
    ctrl.add(algo);
    
    feedback = new CtrlDX("FEEDBACK", 8, 135);
    ctrl.add(feedback);
    
    oscSync = new CtrlDX("OSC KEY SYNC", 2, 136);
    ctrl.add(oscSync);
    
    lfoRate = new CtrlDX("LFO SPEED", 100, 137);
    ctrl.add(lfoRate);
    
    lfoDelay = new CtrlDX("LFO DELAY", 100, 138);
    ctrl.add(lfoDelay);
    
    lfoPitchDepth = new CtrlDX("LFO PM DEPTH", 100, 139);
    ctrl.add(lfoPitchDepth);
    
    lfoAmpDepth = new CtrlDX("LFO AM DEPTH", 100, 140);
    ctrl.add(lfoAmpDepth);
    
    lfoSync = new CtrlDX("LFO KEY SYNC", 2, 141);
    ctrl.add(lfoSync);
    
    lfoWaveform = new CtrlDX("LFO WAVE", 5, 142);
    ctrl.add(lfoWaveform);
    
    transpose = new CtrlDX("MIDDLE C", 49, 144);
    ctrl.add(transpose);
    
    pitchModSens = new CtrlDX("P MODE SENS.", 8, 143);
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
            opCtrl[opVal].egRate[j] = new CtrlDX(opRate, 100, opTarget + j);
            ctrl.add(opCtrl[opVal].egRate[j]);
        }
    
        for (int j = 0; j < 4; j++) {        
            String opLevel;
            opLevel << opName << " EG LEVEL " << (j + 1);
            opCtrl[opVal].egLevel[j] = new CtrlDX(opLevel, 100, opTarget + j + 4);
            ctrl.add(opCtrl[opVal].egLevel[j]);
        }
    
        String opVol;
        opVol << opName << " OUTPUT LEVEL";
        opCtrl[opVal].level = new CtrlDX(opVol, 100, opTarget + 16);
        ctrl.add(opCtrl[opVal].level);

        String opMode;
        opMode << opName << " MODE";
        opCtrl[opVal].opMode = new CtrlDX(opMode, 1, opTarget + 17);
        ctrl.add(opCtrl[opVal].opMode);

        String coarse;
        coarse << opName << " F COARSE";
        opCtrl[opVal].coarse = new CtrlDX(coarse, 32, opTarget + 18);
        ctrl.add(opCtrl[opVal].coarse);

        String fine;
        fine << opName << " F FINE";
        opCtrl[opVal].fine = new CtrlDX(fine, 100, opTarget + 19);
        ctrl.add(opCtrl[opVal].fine);

        String detune;
        detune << opName << " OSC DETUNE";
        opCtrl[opVal].detune = new CtrlDX(detune, 15, opTarget + 20, -7);
        ctrl.add(opCtrl[opVal].detune);

        String sclBrkPt;
        sclBrkPt << opName << " BREAK POINT";
        opCtrl[opVal].sclBrkPt = new CtrlDX(sclBrkPt, 100, opTarget + 8);
        ctrl.add(opCtrl[opVal].sclBrkPt);

        String sclLeftDepth;
        sclLeftDepth << opName << " L SCALE DEPTH";
        opCtrl[opVal].sclLeftDepth = new CtrlDX(sclLeftDepth, 100,
                opTarget + 9);
        ctrl.add(opCtrl[opVal].sclLeftDepth);

        String sclRightDepth;
        sclRightDepth << opName << " R SCALE DEPTH";
        opCtrl[opVal].sclRightDepth = new CtrlDX(sclRightDepth, 100,
                opTarget + 10);
        ctrl.add(opCtrl[opVal].sclRightDepth);

        String sclLeftCurve;
        sclLeftCurve << opName << " L KEY SCALE";
        opCtrl[opVal].sclLeftCurve = new CtrlDX(sclLeftCurve, 4, opTarget + 11);
        ctrl.add(opCtrl[opVal].sclLeftCurve);

        String sclRightCurve;
        sclRightCurve << opName << " R KEY SCALE";
        opCtrl[opVal].sclRightCurve = new CtrlDX(sclRightCurve, 4,
                opTarget + 12);
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
        opCtrl[opVal].velModSens = new CtrlDX(velModSens, 8, opTarget + 15);
        ctrl.add(opCtrl[opVal].velModSens);
    }
    
    for (int i=0; i < ctrl.size(); i++) {
        ctrl[i]->idx = i;
        ctrl[i]->parent = this;
    }
}

int DexedAudioProcessor::importSysex(const char *imported) {    
    memcpy(sysex, imported + 6, 4096);
    for (int i = 0; i < 32; i++) {
        memcpy(patchNames[i], sysex + ((i * 128) + 118), 11);

        for (int j = 0; j < 10; j++) {
            char c = (unsigned char) patchNames[i][j];
            switch (c) {
            case 92:
                c = 'Y';
                break; /* yen */
            case 126:
                c = '>';
                break; /* >> */
            case 127:
                c = '<';
                break; /* << */
            default:
                if (c < 32 || c > 127)
                    c = 32;
                break;
            }
            patchNames[i][j] = c;
        }
        patchNames[i][11] = 0;
    }
    return 0;
}

void DexedAudioProcessor::exportSysex(char *dest) {
    uint8_t header[] = { 0xF0, 0x43, 0x00, 0x09, 0x20, 0x00 };
    memcpy(dest, header, 6);

    // copy 32 voices
    memcpy(dest+6, sysex, 4096);

    // make checksum for dump
    uint8_t footer[] = { 0x00, 0xF7 };
    uint8_t sum = 0;
    for (int i=0; i<4096; i++)
        sum = (sum + sysex[i]) % (1 << 8);
    footer[0] = ((1 << 8) - sum);

    memcpy(dest+4102, footer, 2);
}

void DexedAudioProcessor::unpackProgram(int idx) {
    char *bulk = sysex + (idx * 128);

    for (int op = 0; op < 6; op++) {
        // eg rate and level, brk pt, depth, scaling
        memcpy(data + op * 21, bulk + op * 17, 11);
        char leftrightcurves = bulk[op * 17 + 11];
        data[op * 21 + 11] = leftrightcurves & 3;
        data[op * 21 + 12] = (leftrightcurves >> 2) & 3;
        char detune_rs = bulk[op * 17 + 12];
        data[op * 21 + 13] = detune_rs & 7;
        char kvs_ams = bulk[op * 17 + 13];
        data[op * 21 + 14] = kvs_ams & 3;
        data[op * 21 + 15] = kvs_ams >> 2;
        data[op * 21 + 16] = bulk[op * 17 + 14];  // output level
        char fcoarse_mode = bulk[op * 17 + 15];
        data[op * 21 + 17] = fcoarse_mode & 1;
        data[op * 21 + 18] = fcoarse_mode >> 1;
        data[op * 21 + 19] = bulk[op * 17 + 16];  // fine freq
        data[op * 21 + 20] = detune_rs >> 3;
    }
    memcpy(data + 126, bulk + 102, 9);  // pitch env, algo
    char oks_fb = bulk[111];
    data[135] = oks_fb & 7;
    data[136] = oks_fb >> 3;
    memcpy(data + 137, bulk + 112, 4);  // lfo
    char lpms_lfw_lks = bulk[116];
    data[141] = lpms_lfw_lks & 1;
    data[142] = (lpms_lfw_lks >> 1) & 7;
    data[143] = lpms_lfw_lks >> 4;
    memcpy(data + 144, bulk + 117, 11);  // transpose, name
    data[155] = 1;  // operator on/off
    data[156] = 1;
    data[157] = 1;
    data[158] = 1;
    data[159] = 1;
    data[160] = 1;    
}

void DexedAudioProcessor::packProgram(int idx, const char *name) {
    char *bulk = sysex + (idx * 128);
    
    for(int op = 0; op < 6; op++) {
        // eg rate and level, brk pt, depth, scaling        
        memcpy(bulk + op * 17, data + op * 21, 11);
        int pp = op*17;
        int up = op*21;
        
        // left curves
        bulk[pp+11] = (data[up+11]&0x03) | ((data[up+12]&0x03) << 2);
        bulk[pp+12] = (data[up+13]&0x07) | ((data[up+20]&0x0f) << 3);
        // kvs_ams
        bulk[pp+13] = (data[up+14]&0x03) | ((data[up+15]&0x07) << 2);
        // output lvl
        bulk[pp+14] = data[up+16];
        // fcoarse_mode
        bulk[pp+15] = (data[up+17]&0x01) | ((data[up+18]&0x1f) << 1);
        // fine freq
        bulk[pp+16] = data[up+19];      
    }
    memcpy(bulk + 102, data + 126, 9);      // pitch env, algo
    bulk[111] = (data[135]&0x07) | ((data[136]&0x01) << 3);
    memcpy(bulk + 112, data + 137, 4);      // lfo
    bulk[116] = (data[141]&0x01) | (((data[142]&0x07) << 1) | ((data[143]&0x07) << 4));
    bulk[117] = data[144];
    int eos = 0;
    for(int i=0; i < 10; i++) {
        char c = name[i];
        if ( c == 0 )
            eos = 1;
        if ( eos ) {
            bulk[117+i] = ' ';
            continue;
        }
        c = c < 32 ? ' ' : c;
        c = c > 127 ? ' ' : c;
        bulk[117+i] = c;
    }
    
    memcpy(patchNames[idx], bulk+117, 10);
    patchNames[idx][10] = 0;
}

void DexedAudioProcessor::updateProgramFromSysex(const uint8 *rawdata) {
    memcpy(data, rawdata, 160);
    refreshUI = true;
}

void DexedAudioProcessor::setDxValue(int offset, int v) {
    TRACE("setting dx %d %d", offset, v);
    refreshVoice = true;
    if (offset >= 0)
        data[offset] = v;

    if (!sendSysexChange)
        return;
    uint8 msg[7] = { 0xF0, 0x43, 0x10, offset > 127, 0, (uint8) v, 0xF7 };
    msg[4] = offset & 0x7F;
    midiOut.addEvent(msg, 7, 0);
}

void DexedAudioProcessor::loadBuiltin(int idx) {
    InputStream *is = builtin_pgm->createStreamForEntry(idx);
    
    if ( is == NULL ) {
        TRACE("ENTRY IN ZIP NOT FOUND");
        return;
    }
    
    uint8_t syx_data[4104];
    is->read(&syx_data, 4104);
    delete is;
    
    importSysex((char *) &syx_data);
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
    if ( lastStateSave + 2 < time(NULL) ) {
        for (int i = 0; i < MAX_ACTIVE_NOTES; i++) {
            if (voices[i].keydown == false && voices[i].live == true) {
                voices[i].live = false;
            }
        }
        index = index > 31 ? 31 : index;
        unpackProgram(index);
        lfo.reset(data + 137);
    }
    currentProgram = index;
    updateUI();
}

const String DexedAudioProcessor::getProgramName(int index) {
    if (index >= 32)
        index = 31;
    return String(patchNames[index]);
}

void DexedAudioProcessor::changeProgramName(int index, const String& newName) {
}

const String DexedAudioProcessor::getParameterName(int index) {
    return ctrl[index]->label;
}

const String DexedAudioProcessor::getParameterText(int index) {
    return ctrl[index]->getValueDisplay();
}


struct PluginState {
    uint8_t sysex[4011];
    uint8_t program[161];
    float cutoff;
    float reso;
};

//==============================================================================
void DexedAudioProcessor::getStateInformation(MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // used to SAVE plugin state
    
    PluginState state;
    
    exportSysex((char *)(&state.sysex));
    memcpy(state.program, data, 161);
    state.cutoff = fx.uiCutoff;
    state.reso = fx.uiReso;
    
    destData.insert(&state, sizeof(PluginState), 0);
}

void DexedAudioProcessor::setStateInformation(const void* source, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // used to LOAD plugin state
    
    PluginState state;
    
    if ( sizeInBytes < sizeof(PluginState) ) {
        TRACE("too small plugin state size %d", sizeInBytes);
        return;
    }
    
    if ( sizeInBytes > sizeof(PluginState) ) {
        TRACE("hmmm, too big plugin state size %d", sizeInBytes);
        sizeInBytes = sizeof(PluginState);
    }
    
    memcpy((void *) &state, source, sizeInBytes);

    importSysex((char *) state.sysex);
    memcpy(data, state.program, 161);
    
    fx.uiCutoff = state.cutoff;
    fx.uiReso = state.reso;

    lastStateSave = (long) time(NULL);
    TRACE("setting VST STATE");
    updateUI();
}

//==============================================================================
/*void DexedAudioProcessor::getCurrentProgramStateInformation(
        MemoryBlock& destData) {
    destData.insert(data, 161, 0);
}

void DexedAudioProcessor::setCurrentProgramStateInformation(const void* source,
        int sizeInBytes) {
    memcpy((void *) data, source, sizeInBytes);
    updateUI();
}*/

