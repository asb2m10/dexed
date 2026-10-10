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
#include "../PluginProcessor.h"

SysexComm::SysexComm() {
    sysexChl = 0;
    listener = NULL;    // this will get injected later
    
    inputName = "";
    outputName = "";
    
    input = NULL;
    output = NULL;
    inputOutput = false;
    commandFifo = NULL;

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


    // TODO: refactor this since JUCE 8
    // StringArray devices = MidiInput::getDevices();
    // int idx = devices.indexOf(target);
    //
    // if ( idx == -1 ) {
    //     TRACE("device %s not found", target.toRawUTF8());
    //     inputName = "";
    //     if ( target == "None" || target == "" )
    //         return true;
    //     return false;
    // }
    //
    // input = MidiInput::openDevice(idx, listener);
    // if ( input == NULL ) {
    //     TRACE("unable to open %s", target.toRawUTF8());
    //     return false;
    // }
    //
    // inputName = target;
    // TRACE("sysex %s opened", target.toRawUTF8());
    // input->start();
    //
    // if ( output )
    //     inputOutput = true;
    //
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

    // TODO: refactor this since JUCE 8
    // StringArray devices = MidiOutput::getDevices();
    // int idx = devices.indexOf(target);
    //
    // if ( idx == -1 ) {
    //     TRACE("device %s not found", target.toRawUTF8());
    //     outputName = "";
    //     if ( target == "None" || target == "" )
    //         return true;
    //     return false;
    // }
    //
    // output = MidiOutput::openDevice(idx);
    // if ( output == NULL ) {
    //     TRACE("unable to open %s", target.toRawUTF8());
    //     return false;
    // }
    //
    // outputName = target;
    //
    // if ( input )
    //     inputOutput = true;
    //
    // TRACE("sysex %s opened", target.toRawUTF8());
    return true;
}

bool SysexComm::isInputActive() {
    return true;
}

bool SysexComm::isOutputActive() {
    return true;
}

int SysexComm::getChl() {
    return sysexChl;
}

void SysexComm::setChl(int chl) {
    sysexChl = chl;
}

void SysexComm::setCommandFifo(CommandFifo<DexedAudioProcessor> *fifo) {
    commandFifo = fifo;
}

// Textual description of the DX7 sysex header, as documented in doc/sysex-format.txt:
//   F0 43 <0sssnnnn> <format/group> ...
// where s is the sub-status and n the channel number.
static const char *sysexSubstatusName(int substatus) {
    switch ( substatus ) {
        case 0  : return "BULK DATA";
        case 1  : return "PARAMETER CHANGE";
        default : return "UNKNOWN";
    }
}

// For a bulk dump (sub-status 0) the 4th byte is the format number; for a
// parameter change (sub-status 1) it carries the parameter group instead.
static String sysexFormatName(int substatus, int formatByte) {
    if ( substatus == 0 ) {
        switch ( formatByte ) {
            case 0 : return "1 VOICE (155 bytes)";
            case 9 : return "32 VOICES (4096 bytes)";
        }
        return String("UNKNOWN FORMAT ") + String(formatByte);
    }

    if ( substatus == 1 ) {
        // 0gggggpp: g = parameter group, pp = high bits of the parameter number
        int group = formatByte >> 2;
        switch ( group ) {
            case 0 : return "VOICE PARAMETER";
            case 2 : return "FUNCTION PARAMETER";
        }
        return String("UNKNOWN GROUP ") + String(group);
    }

    return String("UNKNOWN ") + String(formatByte);
}

int SysexComm::send(const MidiMessage &message) {
#ifdef IMPLEMENT_MidiMonitor
    outActivity = true; // indicate to MidiMonitor that a MIDI message is going to be sent
#endif // IMPLEMENT_MidiMonitor

    if ( message.isSysEx() ) {
        const uint8 *buf = message.getRawData();

        if ( message.getRawDataSize() > 3 && buf[1] == 0x43 ) {
            const int substatus = buf[2] >> 4;
            const int chl = (buf[2] & 0x0f) + 1;
            const int formatByte = buf[3];

            DXLOG("Sending sysex message: CHL=%d SUBSTATUS=%d (%s) FORMAT=%d (%s)", chl,
                  substatus, sysexSubstatusName(substatus),
                  formatByte, sysexFormatName(substatus, formatByte).toRawUTF8());
        } else {
            DXLOG("Sending non-DX7 sysex message: %d bytes", message.getRawDataSize());
        }
    } else {
        DXLOG("Sending midi message: CHL=%d", message.getChannel());
    }

    commandFifo->push([message](DexedAudioProcessor &proc) {
        proc.queueOutputMidiMessage(message);
    });
    return 0;
}

void SysexComm::playBuffer(MidiBuffer &keyboardEvents, int numSamples ) {
    noteOutput.addEvents(keyboardEvents, 0, numSamples, 0);
}
