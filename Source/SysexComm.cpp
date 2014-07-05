/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
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

#include "SysexComm.h"
#include "Dexed.h"

SysexComm::SysexComm() {
    sysexChl = 0;
    listener = NULL;    // this will get injected later
    
    inputName = "";
    outputName = "";
    
    input = NULL;
    output = NULL;
}

SysexComm::~SysexComm() {
    if ( input != NULL ) {
        input->stop();
        delete input;
    }
    
    if ( output != NULL )
        delete output;
}

String SysexComm::getInput() {
    return inputName;
}

bool SysexComm::setInput(String target) {
    if ( input != NULL ) {
        input->stop();
        delete input;
        input = NULL;
    }
    
    if ( listener == NULL )
        return true;
    
    StringArray devices = MidiInput::getDevices();
    int idx = devices.indexOf(target);

    if ( idx == -1 ) {
        TRACE("device %s not found", target.toRawUTF8());
        inputName = "";
        if ( target == "None" || target == "" )
            return true;
        return false;
    }

    input = MidiInput::openDevice(idx, listener);
    if ( input == NULL ) {
        TRACE("unable to open %s", target.toRawUTF8());
        return false;
    }

    inputName = target;
    TRACE("sysex %s opened", target.toRawUTF8());
    input->start();
    return true;
}

String SysexComm::getOutput() {
    return outputName;
}

bool SysexComm::setOutput(String target) {
    if ( output != NULL ) {
        delete output;
        output = NULL;
    }
    
    StringArray devices = MidiOutput::getDevices();
    int idx = devices.indexOf(target);
    
    if ( idx == -1 ) {
        TRACE("device %s not found", target.toRawUTF8());
        outputName = "";
        if ( target == "None" || target == "" )
            return true;
        return false;
    }
    
    output = MidiOutput::openDevice(idx);
    if ( output == NULL ) {
        TRACE("unable to open %s", target.toRawUTF8());
        return false;
    }

    outputName = target;
    TRACE("sysex %s opened", target.toRawUTF8());
    return true;
}

bool SysexComm::isInputActive() {
    return input != NULL;
}

bool SysexComm::isOutputActive() {
    return output != NULL;
}

int SysexComm::getChl() {
    return sysexChl;
}

void SysexComm::setChl(int chl) {
    sysexChl = chl;
}

int SysexComm::send(const MidiMessage &message) {
    if ( output == NULL )
        return 2;

    outActivity = true;
    output->sendMessageNow(message);
    return 0;
}

// This is called from the UI Keyboard...
void SysexComm::handleNoteOn(MidiKeyboardState*, int, int midiNoteNumber, float velocity) {
    if ( output == NULL )
        return;

    outActivity = true;
    char iVelo = velocity * 100;
    MidiMessage msg(0x90 + sysexChl, midiNoteNumber, iVelo);
    output->sendMessageNow(msg);
}

void SysexComm::handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber) {
    handleNoteOn(source, midiChannel, midiNoteNumber, 0);
}