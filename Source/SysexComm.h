/*
  ==============================================================================

    MidiMgr.h
    Created: 24 Jun 2014 2:43:55am
    Author:  Pascal Gauthier

  ==============================================================================
*/

#ifndef SYSEXCOMM_H_INCLUDED
#define SYSEXCOMM_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SysexComm {
    MidiInput *input;
    MidiOutput *output;
    String inputName;
    String outputName;
    int sysexChl;
public :
    MidiInputCallback *listener;
    
    SysexComm();
    ~SysexComm();
    
    void setInput(String name);
    void setOutput(String name);
    void setChl(int chl);
    
    String getInput();
    String getOutput();
    int getChl();
    
    bool isInputActive();
    bool isOutputActive();
    
    void send(const MidiMessage& message);
};



#endif  // SYSEXCOMM_H_INCLUDED