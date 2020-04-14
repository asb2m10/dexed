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
    
// Returns true if note should not be played, either because it is not mapped to a scale step or it is assigned to control a parameter in the Master.
// MIDI channel argument is optional but should be included if possible, as some note parameter assignments in the Master may be specific to a single channel.
extern bool MTS_ShouldFilterNote(MTSClient* c,char midinote,char midichannel=-1);

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
