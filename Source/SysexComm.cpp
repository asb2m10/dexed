/*
  ==============================================================================

    MidiMgr.cpp
    Created: 24 Jun 2014 2:43:55am
    Author:  Pascal Gauthier

  ==============================================================================
*/

#include "SysexComm.h"

SysexComm::SysexComm() {
    sysexChl = 1;
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

void SysexComm::setInput(String target) {
    if ( input != NULL ) {
        input->stop();
        delete input;
        input = NULL;
    }
    
    if ( listener == NULL )
        return;
    
    StringArray devices = MidiInput::getDevices();
    int idx = devices.indexOf(target);

    if ( idx == -1 ) {
        // device not found
        inputName = "";
        return;
    }

    input = MidiInput::openDevice(idx, listener);
    if ( input != NULL ) {
        inputName = target;
        input->start();
    }
}

String SysexComm::getOutput() {
    return outputName;
}

void SysexComm::setOutput(String target) {
    if ( output != NULL ) {
        delete output;
        output = NULL;
    }
    
    StringArray devices = MidiOutput::getDevices();
    int idx = devices.indexOf(target);
    
    if ( idx == -1 ) {
        // device not found
        return;
    }
    
    output = MidiOutput::openDevice(idx);
    if ( output != NULL ) {
        outputName = target;
    }
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

void SysexComm::send(const juce::MidiMessage &message) {
    if ( output == NULL )
        return;

    output->sendMessageNow(message);
}