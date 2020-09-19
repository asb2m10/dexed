#ifndef libMTSClient_h
#define libMTSClient_h

extern "C" {
    
    /*
     How to use libMTSClient:

     On startup in the constructor:

        MTSClient *client = MTS_RegisterClient();

     On shutdown in the destructor:

        MTS_DeregisterClient(client);

     When given a note:

        double f=MTS_NoteToFrequency(client, midinote, midichannel);
     OR
        double retune_semitones=MTS_RetuningInSemitones(client, midinote, midichannel);
     OR
        double retune_ratio=MTS_RetuningAsRatio(client, midinote, midichannel);

     If you don’t have the midi channel, use -1, but supplying the channel allows support for microtonal
     MIDI controllers with more than 128 keys that use multi-channel keyboard mappings.

     Querying the retune while a note is playing will allow tuning to change along the flight of the note,
     which is cool, so do if you can! Ideally at the same time as processing any other pitch modulation
     sources (envelopes, MIDI controllers, LFOs etc.).

     The Scala .kbm keyboard mapping file format allows for MIDI keys to be unmapped i.e. no frequency
     is specified for them. The MTS-ESP library supports this. You can query whether the note should
     is unmapped and should be ignored with:

        bool should_ignore_note = MTS_ShouldFilterNote(client, midinote, midichannel);

     If this returns true, ignore the noteOn and don’t play anything. Calling this function is encouraged but
     optional and a valid value for the frequency/retuning will still be returned for an unmapped note. Once again
     if you don’t have the midi channel, use -1, however supplying it allows a master to dedicate keys on specific
     channels for e.g. switching tunings.

     To add support for MIDI Tuning System to your plugin (from the MIDI Spec, using sysex), both dump and realtime,
     implement the above and, when given sysex, call:

        MTS_ParseMIDIData(client, buffer, len); // if buffer is signed char *
     OR
        MTS_ParseMIDIDataU(client, buffer, len); // if buffer is unsigned char *

     If you want to tell the user whether you can see a Master in the session, call:

        bool has_master = MTS_HasMaster(client);

     If you want to tell the user what scale the Master is using, call:

        const char *name = MTS_GetScaleName(client);
     
     */
    
    // Opaque datatype for MTSClient.
    typedef struct MTSClient MTSClient;

    // Register/deregister as a client.  Call from the plugin constuctor and destructor.
    extern MTSClient* MTS_RegisterClient();
    extern void MTS_DeregisterClient(MTSClient*);

    // Check if the client is currently connected to a master plugin.
    extern bool MTS_HasMaster(MTSClient*);

    // Returns true if note should not be played. MIDI channel argument is optional but should be included if possible (0-15).
    extern bool MTS_ShouldFilterNote(MTSClient* c,char midinote,char midichannel=-1);

    // Retuning a midi note. Pick the version that makes your life easiest! MIDI channel argument is optional but should be included if possible (0-15).
    extern double MTS_NoteToFrequency(MTSClient *client,char midinote,char midichannel=-1);
    extern double MTS_RetuningInSemitones(MTSClient *client,char midinote,char midichannel=-1);
    extern double MTS_RetuningAsRatio(MTSClient *client,char midinote,char midichannel=-1);
    
    // Returns the name of the current scale.
    extern const char *MTS_GetScaleName(MTSClient *client);

    // Parse incoming MIDI data to update local retuning.  All formats of MTS sysex message accepted.
    extern void MTS_ParseMIDIDataU(MTSClient *client,const unsigned char *buffer,int len);
    extern void MTS_ParseMIDIData(MTSClient *client,const char *buffer,int len);

};

#endif


