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

#ifndef SYSEXCOMM_H_INCLUDED
#define SYSEXCOMM_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SysexComm : public MidiKeyboardStateListener {
    MidiInput *input;
    MidiOutput *output;
    String inputName;
    String outputName;
    int sysexChl;

public :
    MidiInputCallback *listener;
    
    SysexComm();
    ~SysexComm();
    
    bool setInput(String name);
    bool setOutput(String name);
    void setChl(int chl);
    
    String getInput();
    String getOutput();
    int getChl();
    
    bool isInputActive();
    bool isOutputActive();
    
    int send(const MidiMessage& message);

    void handleNoteOn(MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity);
    void handleNoteOff(MidiKeyboardState* source, int midiChannel, int midiNoteNumber);

    bool inActivity;
    bool outActivity;
};

#endif  // SYSEXCOMM_H_INCLUDED