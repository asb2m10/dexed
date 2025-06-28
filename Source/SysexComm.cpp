/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
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

#include "SysexComm.h"
#include "Dexed.h"

SysexComm::SysexComm() {
    sysexChl = 0;
    listener = NULL;    // this will get injected later
    
    inputName = "";
    outputName = "";
    
    input = NULL;
    output = NULL;
    inputOutput = false;

#ifdef IMPLEMENT_MidiMonitor
    inActivity = false;
    outActivity = false;
#endif //IMPLEMENT_MidiMonitor
}

String SysexComm::getInput() {
    return inputName;
}

bool SysexComm::setInput(String target) {
#ifndef IMPLEMENT_MidiMonitor
    if ( JUCEApplication::isStandaloneApp() )
        return true;
#endif

    if ( target == inputName )
        return true;

    if ( input != NULL ) {
        input->stop();
        input = NULL;
    }
    inputOutput = false;
    
    if ( listener == NULL )
        return true;

    if ( target == "None" || target == "" ) {
        return true;
    }

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
    
    if ( output )
        inputOutput = true;
    
    return true;
}

String SysexComm::getOutput() {
    return outputName;
}

bool SysexComm::setOutput(String target) {
    if ( output != NULL ) {
        output = NULL;
    }
    inputOutput = false;

    if ( target == "None" || target == "" ) {
        return true;
    }

    if ( target == outputName )
        return true;

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
    
    if ( input )
        inputOutput = true;
    
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

#ifdef IMPLEMENT_MidiMonitor
    outActivity = true; // indicate to MidiMonitor that a MIDI message is going to be sent
#endif // IMPLEMENT_MidiMonitor

    output->sendMessageNow(message);
    return 0;
}

void SysexComm::playBuffer(MidiBuffer &keyboardEvents, int numSamples ) {
    noteOutput.addEvents(keyboardEvents, 0, numSamples, 0);
}
