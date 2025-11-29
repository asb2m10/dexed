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

#ifndef SYSEXCOMM_H_INCLUDED
#define SYSEXCOMM_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

 // If the macro ''IMPLEMENT_MidiMonitor'' is defined,
 // then the class ''MidiMonitor'' and its related variables 
 // and invocations from other parts are implemented.
 // If the macro is not defined the related source snippets are excluded.
 // WARNING: this class and related variables and invocations 
 // are very likely candidates for deprecation / obsolescence,
 // so it is NOT RECOMMENDED to define this macro!
 //#define IMPLEMENT_MidiMonitor

class SysexComm {
    std::unique_ptr<MidiInput> input;
    std::unique_ptr<MidiOutput> output;
    String inputName;
    String outputName;
    int sysexChl;
    
    bool inputOutput;
    
    MidiBuffer noteOutput;
public :
    MidiInputCallback *listener;
#ifdef IMPLEMENT_MidiMonitor
    bool inActivity;
    bool outActivity;
#endif //IMPLEMENT_MidiMonitor    
    SysexComm();
    
    bool setInput(String name);
    bool setOutput(String name);
    void setChl(int chl);
    
    String getInput();
    String getOutput();
    int getChl();
    
    bool isInputActive();
    bool isOutputActive();
    inline bool isInputOutputActive() {
        return inputOutput;
    }
    
    int send(const MidiMessage& message);
    void playBuffer(MidiBuffer &keyboardEvents, int numSamples);
};

#endif  // SYSEXCOMM_H_INCLUDED