//
//  libMTSClient.cpp
//  MTS
//
//  Created by Dave on 03/06/2017.
//  Copyright © 2017 dmgaudio. All rights reserved.
//

#include "libMTSClient.h"
#include <math.h>
#include <string>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

const static double ratioToSemitones=12.0/log(2.0);
typedef void (*mts_pv)(void*);
typedef bool (*mts_bool)(void);
typedef bool (*mts_bcc)(char,char);
typedef const double *(*mts_cd)(void);
typedef const double *(*mts_cdc)(char);
typedef bool (*mts_bc)(char);
typedef const char *(*mts_pcc)(void);

struct mtsclientglobal
{
    mtsclientglobal() : RegisterClient(0), DeregisterClient(0), HasMaster(0), ShouldFilterNote(0), ShouldFilterNoteMultiChannel(0), GetTuning(0), GetMultiChannelTuning(0), UseMultiChannelTuning(0), GetScaleName(0), esp_retuning(0)
    {
        for (int i=0;i<128;i++) iet[i]=1.0/(440.0*pow(2.0,(i-69.0)/12.0));
        load_lib();
        if (GetTuning) esp_retuning=GetTuning();
        for (int i=0;i<16;i++) multi_channel_esp_retuning[i]=GetMultiChannelTuning?GetMultiChannelTuning(i):0;
    }
    virtual inline bool isOnline() const {return esp_retuning && HasMaster && HasMaster();}
    
    mts_pv RegisterClient,DeregisterClient;mts_bool HasMaster;mts_bcc ShouldFilterNote,ShouldFilterNoteMultiChannel;mts_cd GetTuning;mts_cdc GetMultiChannelTuning;mts_bc UseMultiChannelTuning;mts_pcc GetScaleName;    // Interface to lib
    double iet[128];const double *esp_retuning;const double *multi_channel_esp_retuning[16];    // tuning tables
    
#ifdef _WIN32
    virtual void load_lib()
	{
		char buffer[8192];HKEY hkey;DWORD type,l=8192;
		RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\OddSound\\MTS",0,KEY_QUERY_VALUE,&hkey);
		if (!hkey) return;
		DWORD ret=RegQueryValueEx(hkey,"Path",0,&type,(LPBYTE)buffer,&l);
		RegCloseKey(hkey);
		if (ret || (l<=1)) return;
        if (buffer[strlen(buffer)-1]!='\\') strcat(buffer,"\\");
		strcat(buffer,"LIBMTS.dll");
		if (!(handle=LoadLibraryA(buffer))) return;
        RegisterClient                  =(mts_pv)   GetProcAddress(handle,"MTS_RegisterClient");
        DeregisterClient                =(mts_pv)   GetProcAddress(handle,"MTS_DeregisterClient");
        HasMaster                       =(mts_bool) GetProcAddress(handle,"MTS_HasMaster");
        ShouldFilterNote                =(mts_bcc)  GetProcAddress(handle,"MTS_ShouldFilterNote");
        ShouldFilterNoteMultiChannel    =(mts_bcc)  GetProcAddress(handle,"MTS_ShouldFilterNoteMultiChannel");
        GetTuning                       =(mts_cd)   GetProcAddress(handle,"MTS_GetTuningTable");
        GetMultiChannelTuning           =(mts_cdc)  GetProcAddress(handle,"MTS_GetMultiChannelTuningTable");
        UseMultiChannelTuning           =(mts_bc)   GetProcAddress(handle,"MTS_UseMultiChannelTuning");
        GetScaleName                    =(mts_pcc)  GetProcAddress(handle,"MTS_GetScaleName");
    }
    virtual ~mtsclientglobal() {FreeLibrary(handle);}
    HINSTANCE handle;
#else
    virtual void load_lib(){
        if (!(handle=dlopen("/Library/Application Support/OddSound/MTS/libMTS.dylib",RTLD_NOW))) return;
        RegisterClient                  =(mts_pv)   dlsym(handle,"MTS_RegisterClient");
        DeregisterClient                =(mts_pv)   dlsym(handle,"MTS_DeregisterClient");
        HasMaster                       =(mts_bool) dlsym(handle,"MTS_HasMaster");
        ShouldFilterNote                =(mts_bcc)  dlsym(handle,"MTS_ShouldFilterNote");
        ShouldFilterNoteMultiChannel    =(mts_bcc)  dlsym(handle,"MTS_ShouldFilterNoteMultiChannel");
        GetTuning                       =(mts_cd)   dlsym(handle,"MTS_GetTuningTable");
        GetMultiChannelTuning           =(mts_cdc)  dlsym(handle,"MTS_GetMultiChannelTuningTable");
        UseMultiChannelTuning           =(mts_bc)   dlsym(handle,"MTS_UseMultiChannelTuning");
        GetScaleName                    =(mts_pcc)  dlsym(handle,"MTS_GetScaleName");
    }
    virtual ~mtsclientglobal() {dlclose(handle);}
    void *handle;
#endif
};

static mtsclientglobal global;

struct MTSClient
{
    MTSClient() : supportsMultiChannelNoteFiltering(false), supportsMultiChannelTuning(false), freqRequestReceived(false)
    {
        for (int i=0;i<128;i++) retuning[i]=440.0*pow(2.0,(i-69.0)/12.0);
        if (global.RegisterClient) global.RegisterClient((void*)this);
    }
    ~MTSClient() {if (global.DeregisterClient) global.DeregisterClient((void*)this);}
    bool hasMaster() {return global.isOnline();}
    inline double freq(char midinote,char midichannel)
    {
        freqRequestReceived=true;
        supportsMultiChannelTuning=!(midichannel&~15);
        if (global.isOnline())
        {
            if (supportsMultiChannelNoteFiltering && supportsMultiChannelTuning && global.UseMultiChannelTuning && global.UseMultiChannelTuning(midichannel) && global.multi_channel_esp_retuning[midichannel])
            {
                return global.multi_channel_esp_retuning[midichannel][midinote&127];
            }
            else return global.esp_retuning[midinote&127];
        }
        return retuning[midinote&127];
    }
    inline bool shouldFilterNote(char midinote,char midichannel)
    {
        supportsMultiChannelNoteFiltering=!(midichannel&~15);
        if (!freqRequestReceived) supportsMultiChannelTuning=supportsMultiChannelNoteFiltering;    // assume it supports multi channel tuning until we receive a request for a frequency and can verify
        if (supportsMultiChannelNoteFiltering && supportsMultiChannelTuning && global.UseMultiChannelTuning && global.UseMultiChannelTuning(midichannel))
        {
            return global.ShouldFilterNoteMultiChannel?global.ShouldFilterNoteMultiChannel(midinote&127,midichannel):false;
        }
        return global.ShouldFilterNote?global.ShouldFilterNote(midinote&127,midichannel):false;
    }
    inline void parseMIDIData(const unsigned char *buffer,int len)
    {
        int sysex_ctr=0,sysex_value=0,note=0,numTunings=0;
        int bank=-1,prog=0,checksum=0;short int channelBitmap=0;   // maybe we'll want to use these at some point
        eSysexState state=eIgnoring;eMTSFormat format=eBulk;bool realtime=false;
        for (int i=0;i<len;i++)
        {
            unsigned char b=buffer[i];
            if (b==0xF7) {state=eIgnoring;continue;}
            switch (state)
            {
                case eIgnoring:
                    if (b==0xF0) state=eMatchingSysex;
                    break;
                case eMatchingSysex:
                    sysex_ctr=0;
                    if (b==0x7E) state=eSysexValid;
                    else if (b==0x7F) realtime=true,state=eSysexValid;
                    else state=eMatchingSysex;   // don't switch to ignoring...Scala adds two bytes here, we need to skip over them
                    break;
                case eSysexValid:
                    switch (sysex_ctr++)    // handle device ID
                    {
                        case 0: case 2: break;
                        case 1: case 3: if (b==0x08) state=eMatchingMTS; break; // no extended device IDs have byte 2 set to 0x08, so this is a safe check for MTS message
                        default: state=eIgnoring; break;    // it's not an MTS message
                    }
                    break;
                case eMatchingMTS:
                    sysex_ctr=0;
                    switch (b)
                    {
                        case 0: format=eRequest,state=eMatchingProg; break;
                        case 1: format=eBulk,state=eMatchingProg; break;
                        case 2: format=eSingle,state=eMatchingProg; break;
                        case 3: format=eRequest,state=eMatchingBank; break;
                        case 4: format=eBulk,state=eMatchingBank; break;
                        case 5: format=eScaleOctOneByte,state=eMatchingBank; break;
                        case 6: format=eScaleOctTwoByte,state=eMatchingBank; break;
                        case 7: format=eSingle,state=eMatchingBank; break;
                        case 8: format=eScaleOctOneByteExt,state=eMatchingChannel; break;
                        case 9: format=eScaleOctTwoByteExt,state=eMatchingChannel; break;
                        default: state=eIgnoring; break;    // it's not a valid MTS format
                    }
                    break;
                case eMatchingBank:
                    bank=b;
                    state=eMatchingProg;
                    break;
                case eMatchingProg:
                    prog=b;
                    if (format==eSingle) state=eNumTunings;else state=eTuningName,tuningName.clear();
                    break;
                case eTuningName:
                    tuningName.push_back(b);
                    if (++sysex_ctr>=16) sysex_ctr=0,state=eTuningData;
                    break;
                case eNumTunings:
                    numTunings=b,sysex_ctr=0,state=eTuningData;
                    break;
                case eMatchingChannel:
                    switch (sysex_ctr++)
                    {
                        case 0: for (int i=14;i<16;i++) channelBitmap|=(1<<i); break;
                        case 1: for (int i=7;i<14;i++) channelBitmap|=(1<<i); break;
                        case 2: for (int i=0;i<7;i++) channelBitmap|=(1<<i); sysex_ctr=0,state=eTuningData; break;
                    }
                    break;
                case eTuningData:
                    switch (format)
                    {
                        case eBulk:
                            sysex_value=(sysex_value<<7)|(b&127);
                            sysex_ctr++;
                            if ((sysex_ctr&3)==3)
                            {
                                if (!(note==0x7F && sysex_value==16383)) updateTuning(note,(sysex_value>>14)&127,(sysex_value&16383)/16383.);
                                sysex_value=0;sysex_ctr++;
                                if (++note>=128) state=eCheckSum;
                            }
                            break;
                        case eSingle:
                            sysex_value=(sysex_value<<7)|(b&127);
                            sysex_ctr++;
                            if (!(sysex_ctr&3))
                            {
                                if (!(note==0x7F && sysex_value==16383)) updateTuning((sysex_value>>21)&127,(sysex_value>>14)&127,(sysex_value&16383)/16383.);
                                sysex_value=0;
                                if (++note>=numTunings) state=eIgnoring;
                            }
                            break;
                        case eScaleOctOneByte: case eScaleOctOneByteExt:
                            for (int i=sysex_ctr;i<128;i+=12) updateTuning(i,i,((double)(b&127)-64.)*0.01);
                            if (++sysex_ctr>=12) state=format==eScaleOctOneByte?eCheckSum:eIgnoring;
                            break;
                        case eScaleOctTwoByte: case eScaleOctTwoByteExt:
                            sysex_value=(sysex_value<<7)|(b&127);
                            sysex_ctr++;
                            if (!(sysex_ctr&1))
                            {
                                double detune=((double)(sysex_value&16383)-8192.)/(sysex_value>8192?8191.:8192.);
                                for (int i=note;i<128;i+=12) updateTuning(i,i,detune);
                                if (++note>=12) state=format==eScaleOctTwoByte?eCheckSum:eIgnoring;
                            }
                            break;
                        default: state=eIgnoring; break;
                    }
                    break;
                case eCheckSum:
                    checksum=b;
                    state=eIgnoring;
                    break;
            }
        }
    }
    inline void updateTuning(int note,int retuneNote,double detune)
    {
        if (note<0 || note>127 || retuneNote<0 || retuneNote>127) return;
        retuning[note]=440.*pow(2.,((retuneNote+detune)-69.)/12.);
    }
    const char *getScaleName()
    {
        if (global.GetScaleName) return global.GetScaleName();
        return tuningName.c_str();
    }
    
    enum eSysexState {eIgnoring=0,eMatchingSysex,eSysexValid,eMatchingMTS,eMatchingBank,eMatchingProg,eMatchingChannel,eTuningName,eNumTunings,eTuningData,eCheckSum};
    enum eMTSFormat {eRequest=0,eBulk,eSingle,eScaleOctOneByte,eScaleOctTwoByte,eScaleOctOneByteExt,eScaleOctTwoByteExt};

    double retuning[128];
    std::string tuningName;
    bool supportsMultiChannelNoteFiltering,supportsMultiChannelTuning,freqRequestReceived;
};

// Exported functions:
MTSClient* MTS_RegisterClient()                                             {return new MTSClient;}
void MTS_DeregisterClient(MTSClient* c)                                     {delete c;}
bool MTS_HasMaster(MTSClient* c)                                            {return c?c->hasMaster():false;}
bool MTS_ShouldFilterNote(MTSClient* c,char midinote,char midichannel)      {return c?c->shouldFilterNote(midinote&127,midichannel):false;}
double MTS_NoteToFrequency(MTSClient* c,char midinote,char midichannel)     {return c?c->freq(midinote,midichannel):(1.0/global.iet[midinote&127]);}
double MTS_RetuningAsRatio(MTSClient* c,char midinote,char midichannel)     {return c?c->freq(midinote,midichannel)*global.iet[midinote&127]:1.0;}
double MTS_RetuningInSemitones(MTSClient* c,char midinote,char midichannel) {return ratioToSemitones*log(MTS_RetuningAsRatio(c,midinote,midichannel));}
const char *MTS_GetScaleName(MTSClient *c)                                  {return c?c->getScaleName():"";}
void MTS_ParseMIDIDataU(MTSClient *c,const unsigned char *buffer,int len)   {if (c) c->parseMIDIData(buffer,len);}
void MTS_ParseMIDIData(MTSClient *c,const char *buffer,int len)             {if (c) c->parseMIDIData((const unsigned char*)buffer,len);}

