//
//  libMTSClient.h
//  MTS
//
//  Created by Dave on 03/06/2017.
//  Copyright © 2017 dmgaudio. All rights reserved.
//
#ifndef libMTSClient_h
#define libMTSClient_h

extern "C" {

// Opaque datatype for MTSClient.
typedef struct MTSClient MTSClient;

// Retuning a midi note. Pick the version that makes your life easiest!
extern double MTS_NoteToFrequency(MTSClient *client,char midinote);
extern double MTS_RetuningInSemitones(MTSClient *client,char midinote);
extern double MTS_RetuningAsRatio(MTSClient *client,char midinote);
    
// Returns the name of the current scale
extern const char *MTS_GetScaleName(MTSClient *client);

// Register/deregister as a client
extern MTSClient* MTS_RegisterClient();
extern void MTS_DeregisterClient(MTSClient*);
extern bool MTS_HasMaster(MTSClient*);
    
// Parse incoming MIDI data to update local retuning
extern void MTS_ParseMIDIDataU(MTSClient *client,const unsigned char *buffer,int len);
extern void MTS_ParseMIDIData(MTSClient *client,const char *buffer,int len);

};


#endif
