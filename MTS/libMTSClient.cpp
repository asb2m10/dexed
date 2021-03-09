#include "libMTSClient.h"
#include <math.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__TOS_WIN__) || defined(_MSC_VER)
#define MTS_ESP_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#else
#include <dlfcn.h>
#endif

const static double ln2=0.693147180559945309417;
const static double ratioToSemitones=17.31234049066756088832;   // 12./log(2.)
typedef void (*mts_void)(void);
typedef bool (*mts_bool)(void);
typedef bool (*mts_bcc)(char,char);
typedef const double *(*mts_cd)(void);
typedef const double *(*mts_cdc)(char);
typedef bool (*mts_bc)(char);
typedef const char *(*mts_pcc)(void);

struct mtsclientglobal
{
    mtsclientglobal() : RegisterClient(0), DeregisterClient(0), HasMaster(0), ShouldFilterNote(0), ShouldFilterNoteMultiChannel(0), GetTuning(0), GetMultiChannelTuning(0), UseMultiChannelTuning(0), GetScaleName(0), esp_retuning(0), handle(0)
    {
        for (int i=0;i<128;i++) iet[i]=1./(440.*pow(2.,(i-69.)/12.));
        load_lib();
        if (GetTuning) esp_retuning=GetTuning();
        for (int i=0;i<16;i++) multi_channel_esp_retuning[i]=GetMultiChannelTuning?GetMultiChannelTuning(i):0;
    }
    virtual inline bool isOnline() const {return esp_retuning && HasMaster && HasMaster();}
    
    mts_void RegisterClient,DeregisterClient;mts_bool HasMaster;mts_bcc ShouldFilterNote,ShouldFilterNoteMultiChannel;mts_cd GetTuning;mts_cdc GetMultiChannelTuning;mts_bc UseMultiChannelTuning;mts_pcc GetScaleName;    // Interface to lib
    double iet[128];const double *esp_retuning;const double *multi_channel_esp_retuning[16];    // tuning tables
    
#ifdef MTS_ESP_WIN
    virtual void load_lib()
	{
		TCHAR buffer[MAX_PATH];
		const TCHAR *libpath=TEXT("\\LIBMTS.dll");
		GetSystemDirectory(buffer,MAX_PATH);
        _tcscat(buffer,libpath);
		if (!(handle=LoadLibrary(buffer))) return;
        RegisterClient                  =(mts_void) GetProcAddress(handle,"MTS_RegisterClient");
        DeregisterClient                =(mts_void) GetProcAddress(handle,"MTS_DeregisterClient");
        HasMaster                       =(mts_bool) GetProcAddress(handle,"MTS_HasMaster");
        ShouldFilterNote                =(mts_bcc)  GetProcAddress(handle,"MTS_ShouldFilterNote");
        ShouldFilterNoteMultiChannel    =(mts_bcc)  GetProcAddress(handle,"MTS_ShouldFilterNoteMultiChannel");
        GetTuning                       =(mts_cd)   GetProcAddress(handle,"MTS_GetTuningTable");
        GetMultiChannelTuning           =(mts_cdc)  GetProcAddress(handle,"MTS_GetMultiChannelTuningTable");
        UseMultiChannelTuning           =(mts_bc)   GetProcAddress(handle,"MTS_UseMultiChannelTuning");
        GetScaleName                    =(mts_pcc)  GetProcAddress(handle,"MTS_GetScaleName");
    }
    virtual ~mtsclientglobal() {if (handle) FreeLibrary(handle);}
    HINSTANCE handle;
#else
    virtual void load_lib()
    {
        if (!(handle=dlopen("/Library/Application Support/MTS-ESP/libMTS.dylib",RTLD_NOW)) &&
            !(handle=dlopen("/usr/local/lib/libMTS.so",RTLD_NOW))) return;
        RegisterClient                  =(mts_void) dlsym(handle,"MTS_RegisterClient");
        DeregisterClient                =(mts_void) dlsym(handle,"MTS_DeregisterClient");
        HasMaster                       =(mts_bool) dlsym(handle,"MTS_HasMaster");
        ShouldFilterNote                =(mts_bcc)  dlsym(handle,"MTS_ShouldFilterNote");
        ShouldFilterNoteMultiChannel    =(mts_bcc)  dlsym(handle,"MTS_ShouldFilterNoteMultiChannel");
        GetTuning                       =(mts_cd)   dlsym(handle,"MTS_GetTuningTable");
        GetMultiChannelTuning           =(mts_cdc)  dlsym(handle,"MTS_GetMultiChannelTuningTable");
        UseMultiChannelTuning           =(mts_bc)   dlsym(handle,"MTS_UseMultiChannelTuning");
        GetScaleName                    =(mts_pcc)  dlsym(handle,"MTS_GetScaleName");
    }
    virtual ~mtsclientglobal() {if (handle) dlclose(handle);}
    void *handle;
#endif
};

static mtsclientglobal global;

struct MTSClient
{
    MTSClient() : tuningName("12-TET"), supportsMultiChannelNoteFiltering(false), supportsMultiChannelTuning(false), freqRequestReceived(false)
    {
        for (int i=0;i<128;i++) retuning[i]=440.*pow(2.,(i-69.)/12.);
        if (global.RegisterClient) global.RegisterClient();
    }
    ~MTSClient() {if (global.DeregisterClient) global.DeregisterClient();}
    bool hasMaster() {return global.isOnline();}
    inline double freq(char midinote,char midichannel)
    {
        freqRequestReceived=true;
        supportsMultiChannelTuning=!(midichannel&~15);
        if (global.isOnline())
        {
            if (supportsMultiChannelNoteFiltering && supportsMultiChannelTuning && global.UseMultiChannelTuning && global.UseMultiChannelTuning(midichannel) && global.multi_channel_esp_retuning[midichannel&15])
            {
                return global.multi_channel_esp_retuning[midichannel&15][midinote&127];
            }
            else return global.esp_retuning[midinote&127];
        }
        return retuning[midinote&127];
    }
    inline bool shouldFilterNote(char midinote,char midichannel)
    {
        supportsMultiChannelNoteFiltering=!(midichannel&~15);
        if (!freqRequestReceived) supportsMultiChannelTuning=supportsMultiChannelNoteFiltering;    // assume it supports multi channel tuning until we receive a request for a frequency and can verify
        if (!global.isOnline()) return false;
        if (supportsMultiChannelNoteFiltering && supportsMultiChannelTuning && global.UseMultiChannelTuning && global.UseMultiChannelTuning(midichannel))
        {
            return global.ShouldFilterNoteMultiChannel?global.ShouldFilterNoteMultiChannel(midinote&127,midichannel):false;
        }
        return global.ShouldFilterNote?global.ShouldFilterNote(midinote&127,midichannel):false;
    }
    inline char freqToNote(double freq,char midichannel)
    {
        bool online=global.isOnline();
        const double *freqs=online?global.esp_retuning:retuning;
        unsigned char iLower,iUpper;iLower=0;iUpper=0;
        double dLower,dUpper;dLower=0;dUpper=0;
        for (int i=0;i<128;i++)
        {
            if (online && global.ShouldFilterNote && global.ShouldFilterNote(i,midichannel)) continue;
            double d=freqs[i]-freq;
            if (!d) return i;
            if (d<0) {if (!dLower || d>dLower) {dLower=d;iLower=i;}}
            else if (!dUpper || d<dUpper) {dUpper=d;iUpper=i;}
        }
        if (!dLower) return iUpper;
        if (!dUpper || iLower==iUpper) return iLower;
        double fmid=freqs[iLower]*pow(2.,0.5*(log(freqs[iUpper]/freqs[iLower])/ln2));
        return freq<fmid?iLower:iUpper;
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
                    else if (b==0x7F) {realtime=true;state=eSysexValid;}
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
                        case 0: format=eRequest;state=eMatchingProg; break;
                        case 1: format=eBulk;state=eMatchingProg; break;
                        case 2: format=eSingle;state=eMatchingProg; break;
                        case 3: format=eRequest;state=eMatchingBank; break;
                        case 4: format=eBulk;state=eMatchingBank; break;
                        case 5: format=eScaleOctOneByte;state=eMatchingBank; break;
                        case 6: format=eScaleOctTwoByte;state=eMatchingBank; break;
                        case 7: format=eSingle;state=eMatchingBank; break;
                        case 8: format=eScaleOctOneByteExt;state=eMatchingChannel; break;
                        case 9: format=eScaleOctTwoByteExt;state=eMatchingChannel; break;
                        default: state=eIgnoring; break;    // it's not a valid MTS format
                    }
                    break;
                case eMatchingBank:
                    bank=b;
                    state=eMatchingProg;
                    break;
                case eMatchingProg:
                    prog=b;
                    if (format==eSingle) state=eNumTunings;else {state=eTuningName;tuningName[0]='\0';}
                    break;
                case eTuningName:
                    tuningName[sysex_ctr]=b;
                    if (++sysex_ctr>=16) {tuningName[16]='\0';sysex_ctr=0;state=eTuningData;}
                    break;
                case eNumTunings:
                    numTunings=b;sysex_ctr=0;state=eTuningData;
                    break;
                case eMatchingChannel:
                    switch (sysex_ctr++)
                    {
                        case 0: for (int i=14;i<16;i++) channelBitmap|=(1<<i); break;
                        case 1: for (int i=7;i<14;i++) channelBitmap|=(1<<i); break;
                        case 2: for (int i=0;i<7;i++) channelBitmap|=(1<<i);sysex_ctr=0;state=eTuningData; break;
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
    const char *getScaleName() {return global.isOnline() && global.GetScaleName?global.GetScaleName():tuningName;}
    
    enum eSysexState {eIgnoring=0,eMatchingSysex,eSysexValid,eMatchingMTS,eMatchingBank,eMatchingProg,eMatchingChannel,eTuningName,eNumTunings,eTuningData,eCheckSum};
    enum eMTSFormat {eRequest=0,eBulk,eSingle,eScaleOctOneByte,eScaleOctTwoByte,eScaleOctOneByteExt,eScaleOctTwoByteExt};

    double retuning[128];
    char tuningName[17];
    bool supportsMultiChannelNoteFiltering,supportsMultiChannelTuning,freqRequestReceived;
};

static char freqToNoteET(double freq)
{
    static double freqs[128];static bool init=false;
    if (!init) {for (int i=0;i<128;i++) freqs[i]=440.*pow(2.,(i-69.)/12.);init=true;}
    if (freq<=freqs[0]) return 0;
    if (freq>=freqs[127]) return 127;
    int mid=0;int n=-1;int n2=-1;
    for (int first=0,last=127;freq!=freqs[(mid=first+(last-first)/2)];(freq<freqs[mid])?last=mid-1:first=mid+1) if (first>last)
    {
        if (!mid) {n=mid;break;}
        if (mid>127) mid=127;
        n=mid-((freq-freqs[mid-1])<(freqs[mid]-freq));
        break;
    }
    if (n==-1) {if (freq==freqs[mid]) n=mid;else return 60;}
    if (!n) n2=1;
    else if (n==127) n2=126;
    else n2=n+1*(fabs(freqs[n-1]-freq)<fabs(freqs[n+1]-freq)?-1:1);
    if (n2<n) {int t=n;n=n2;n2=t;}
    double fmid=freqs[n]*pow(2.,0.5*(log(freqs[n2]/freqs[n])/ln2));
    return freq<fmid?n:n2;
}

// Exported functions:
MTSClient* MTS_RegisterClient()                                             {return new MTSClient;}
void MTS_DeregisterClient(MTSClient* c)                                     {delete c;}
bool MTS_HasMaster(MTSClient* c)                                            {return c?c->hasMaster():false;}
bool MTS_ShouldFilterNote(MTSClient* c,char midinote,char midichannel)      {return c?c->shouldFilterNote(midinote&127,midichannel):false;}
double MTS_NoteToFrequency(MTSClient* c,char midinote,char midichannel)     {return c?c->freq(midinote,midichannel):(1./global.iet[midinote&127]);}
double MTS_RetuningAsRatio(MTSClient* c,char midinote,char midichannel)     {return c?c->freq(midinote,midichannel)*global.iet[midinote&127]:1.;}
double MTS_RetuningInSemitones(MTSClient* c,char midinote,char midichannel) {return ratioToSemitones*log(MTS_RetuningAsRatio(c,midinote,midichannel));}
char MTS_FrequencyToNote(MTSClient *c,double freq,char midichannel)         {return c?c->freqToNote(freq,midichannel):freqToNoteET(freq);}
const char *MTS_GetScaleName(MTSClient *c)                                  {return c?c->getScaleName():"";}
void MTS_ParseMIDIDataU(MTSClient *c,const unsigned char *buffer,int len)   {if (c) c->parseMIDIData(buffer,len);}
void MTS_ParseMIDIData(MTSClient *c,const char *buffer,int len)             {if (c) c->parseMIDIData((const unsigned char*)buffer,len);}

