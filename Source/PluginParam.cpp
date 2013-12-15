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

#include "PluginParam.h"
#include "PluginProcessor.h"

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
    if ( slider != NULL ) {
        slider->removeListener(this);
        slider = NULL;
    }
    
    if ( button != NULL ) {
        button->removeListener(this);
        button = NULL;
    }
    
    if ( comboBox != NULL ) {
        comboBox->removeListener(this);
        comboBox = NULL;
    }
}


// ************************************************************************
// CtrlInt ================================================================
CtrlInt::CtrlInt(String name, int steps, int offset, bool starts1) :
Ctrl(name) {
    add1 = starts1 == 1;
    this->steps = steps;
    value = 0;
    dxOffset = offset;
}

float CtrlInt::getValuePlugin() {
    return value / steps;
}

void CtrlInt::setValuePlugin(float f) {
    setValue((f * steps));
}

void CtrlInt::setValue(int v) {
    if ( v >= steps ) {
        TRACE("WARNING: value too big %s : %d", label.toRawUTF8(), v);
        v = steps-1;
    }
    value = v;
    if ( dxOffset >= 0 ) {
        if ( parent != NULL )
            parent->setDxValue(dxOffset, v);
    }
}

int CtrlInt::getValue() {
    if ( dxOffset >= 0 )
        value = parent->data[dxOffset];
    return value;
}

String CtrlInt::getValueDisplay() {
    String ret;
    ret << getValue();
    return ret;
}

void CtrlInt::publishValue(int value) {
    parent->beginParameterChangeGesture(idx);
    parent->setParameterNotifyingHost(idx, (((float)value) / steps));
    parent->endParameterChangeGesture(idx);
}

void CtrlInt::sliderValueChanged(Slider* moved) {
    publishValue(((int)moved->getValue() - add1));
}

void CtrlInt::buttonClicked (Button* clicked) {
    publishValue(clicked->getToggleStateValue() == 1 ? 1 : 0);
}

void CtrlInt::comboBoxChanged (ComboBox* combo) {
    publishValue(combo->getSelectedId()-1);
}

void CtrlInt::updateComponent() {
    //TRACE("setting for %s %d", label.toRawUTF8(), getValue());
    if ( slider != NULL ) {
        slider->setValue(getValue() + add1, NotificationType::dontSendNotification);
    }
    
    if ( button != NULL ) {
        if ( getValue() == 0 ) {
            button->setToggleState(false, NotificationType::dontSendNotification);
        } else {
            button->setToggleState(true, NotificationType::dontSendNotification);
        }
    }
    
    if ( comboBox != NULL ) {
        int cvalue = getValue() + 1;
        if ( comboBox->getNumItems() <= cvalue ) {
            cvalue = comboBox->getNumItems();
        }
        comboBox->setSelectedId(cvalue, NotificationType::dontSendNotification);
    }
}

// ************************************************************************
// Patcher ================================================================
void DexedAudioProcessor::initCtrl() {
    importSysex(BinaryData::startup_syx);
    
    // fill operator values;
    for(int i=0;i<6;i++) {
        // In the Sysex, OP6 comes first, then OP5...
        //int opTarget = (5-i) * 21;
        // In the Sysex, OP6 comes first, then OP5...
        int opTarget = i * 21;

        for(int j=0;j<4;j++) {
            String opRate;
            opRate << "OP" << (i+1) << "-EGR" << (j+1);
            opCtrl[i].egRate[j] = new CtrlInt(opRate, 100, opTarget+j);
            ctrl.add(opCtrl[i].egRate[j]);
            
            String opLevel;
            opLevel << "OP" << (i+1) << "-EGL" << (j+1);
            opCtrl[i].egLevel[j] = new CtrlInt(opLevel, 100, opTarget+j+4);
            ctrl.add(opCtrl[i].egLevel[j]);
        }
        String opVol;
        opVol << "OP" << (i+1) << "-LEVEL";
        opCtrl[i].level = new CtrlInt(opVol, 100, opTarget+16);
        ctrl.add(opCtrl[i].level);
        
        String opMode;
        opMode << "OP" << (i+1) << "-MODE";
        opCtrl[i].opMode = new CtrlInt(opMode, 1, opTarget+17);
        ctrl.add(opCtrl[i].opMode);
        
        String coarse;
        coarse << "OP" << (i+1) << "-COARSE";
        opCtrl[i].coarse = new CtrlInt(coarse, 32, opTarget+18, true);
        ctrl.add(opCtrl[i].coarse);
        
        String fine;
        fine << "OP" << (i+1) << "-FINE";
        opCtrl[i].fine = new CtrlInt(fine, 100, opTarget+19);
        ctrl.add(opCtrl[i].fine);

        String detune;
        detune << "OP" << (i+1) << "-DETUNE";
        opCtrl[i].detune = new CtrlInt(detune, 15, opTarget+20);
        ctrl.add(opCtrl[i].detune);
    }
    
    algo = new CtrlInt("Algorithm", 32, 134, true);
    ctrl.add(algo);

    lfoRate  = new CtrlInt("LFO-Rate", 100, 137);
    ctrl.add(lfoRate);

    lfoDelay = new CtrlInt("LFO-Delay", 100, 138);
    ctrl.add(lfoDelay);

    lfoPitchDepth = new CtrlInt("LFO-PitchDepth", 100, 139);
    ctrl.add(lfoPitchDepth);

    lfoAmpDepth = new CtrlInt("LFO-AmpDepth", 100, 140);
    ctrl.add(lfoAmpDepth);

    lfoSync = new CtrlInt("LFO-Sync", 2, 141);
    ctrl.add(lfoSync);

    lfoWaveform = new CtrlInt("LFO-Waveform", 5, 142);
    ctrl.add(lfoWaveform);
    
    for(int i=0;i<ctrl.size();i++) {
        ctrl[i]->idx = i;
        ctrl[i]->parent = this;
    }
}

int DexedAudioProcessor::importSysex(const char *imported) {
    memcpy(sysex, imported+6, 4104);
    for(int i=0;i<32;i++) {
        memcpy(patchNames[i], sysex + ((i*128)+118), 11);
        
        for(int j=0;j<10;j++) {
            char c = (unsigned char)patchNames[i][j];
            switch (c) {
            case  92:  c = 'Y';  break;  /* yen */
            case 126:  c = '>';  break;  /* >> */
            case 127:  c = '<';  break;  /* << */
            default:
               if (c < 32 || c > 127) c = 32;
               break;
            }
            patchNames[i][j] = c;
        }
        patchNames[i][11] = 0;
    }
    return 0;
}

void DexedAudioProcessor::unpackSysex(int idx) {
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


void DexedAudioProcessor::setDxValue(int offset, int v) {
    TRACE("setting dx %d %d", offset, v);
    isDirty = true;
    if ( offset >= 0 )
        data[offset] = v;
}

void DexedAudioProcessor::unbindUI() {
    for(int i=0;i<ctrl.size();i++) {
        ctrl[i]->unbind();
    }
}

//==============================================================================
int DexedAudioProcessor::getNumParameters() {
    return ctrl.size();
}

float DexedAudioProcessor::getParameter (int index) {
    return ctrl[index]->getValuePlugin();
}

void DexedAudioProcessor::setParameter (int index, float newValue) {
    ctrl[index]->setValuePlugin(newValue);
}

int DexedAudioProcessor::getNumPrograms() {
    // there is 32 program, the 33th one is a ghost for saving "unsaved" preset.
    return 32;
}

int DexedAudioProcessor::getCurrentProgram() {
    return currentProgram;
}

void DexedAudioProcessor::setCurrentProgram (int index) {
    /*// VST has a naughty problem of calling setCurrentProgram after a host has loaded
    // an edited preset. We ignore the 16th value, since we want to keep the user values
    if ( index == 32 ) {
        return;
    }*/

    for(int i=0;i<MAX_ACTIVE_NOTES;i++) {
        if ( voices[i].keydown == false && voices[i].live == true ) {
            voices[i].live = false;
        }
    }
    index = index > 31 ? 31 : index;
    unpackSysex(index);
    currentProgram = index;
    lfo.reset(data + 137);
    updateUI();
}

const String DexedAudioProcessor::getProgramName (int index) {
    if ( index >= 32 )
        index = 31;
    return String(patchNames[index]);
}

void DexedAudioProcessor::changeProgramName (int index, const String& newName) {
}


const String DexedAudioProcessor::getParameterName (int index) {
    return ctrl[index]->label;
}

const String DexedAudioProcessor::getParameterText (int index) {
    return ctrl[index]->getValueDisplay();
}

//==============================================================================
void DexedAudioProcessor::getStateInformation (MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.*/
    destData.insert(data, 161, 0);
}

void DexedAudioProcessor::setStateInformation (const void* source, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    memcpy((void *)data, source, sizeInBytes);
    updateUI();
}


//==============================================================================
void DexedAudioProcessor::getCurrentProgramStateInformation (MemoryBlock& destData) {
    destData.insert(data, 161, 0);
}

void DexedAudioProcessor::setCurrentProgramStateInformation (const void* source, int sizeInBytes) {
    memcpy((void *)data, source, sizeInBytes);
    updateUI();
}




