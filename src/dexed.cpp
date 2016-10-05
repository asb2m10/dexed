// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lv2synth.hpp>
#include "dexed.peg"
#include "EngineMkI.h"
#include "EngineOpl.h"

class DexedVoice : public LV2::Voice {
public:
      Controllers controllers;
    StringArray programNames;
    Cartridge currentCart;
    uint8_t data[161];

    SysexComm sysexComm;
    VoiceStatus voiceStatus;
    File activeFileCartridge;
    int getEngineType();
    void setEngineType(int rs);
    Array<Ctrl*> ctrl;

    OperatorCtrl opCtrl[6];
    ScopedPointer<CtrlDX> pitchEgRate[4];
    ScopedPointer<CtrlDX> pitchEgLevel[4];
    ScopedPointer<CtrlDX> pitchModSens;
    ScopedPointer<CtrlDX> algo;
    ScopedPointer<CtrlDX> oscSync;
    ScopedPointer<CtrlDX> feedback;
    ScopedPointer<CtrlDX> lfoRate;
    ScopedPointer<CtrlDX> lfoDelay;
    ScopedPointer<CtrlDX> lfoAmpDepth;
    ScopedPointer<CtrlDX> lfoPitchDepth;
    ScopedPointer<CtrlDX> lfoWaveform;
    ScopedPointer<CtrlDX> lfoSync;
    ScopedPointer<CtrlDX> transpose;

    ScopedPointer<CtrlFloat> fxCutoff;
    ScopedPointer<CtrlFloat> fxReso;
    ScopedPointer<CtrlFloat> output;
    ScopedPointer<Ctrl> tune;

  DexedVoice(double rate) 
    : m_key(LV2::INVALID_KEY), m_rate(rate) {
    
  }
  
  void on(unsigned char key, unsigned char velocity) { 
    m_key = key;
  }

  void off(unsigned char velocity) { 
    m_key = LV2::INVALID_KEY;
  }

  unsigned char get_key() const { 
    return m_key; 
  }

  void render(uint32_t from, uint32_t to) {
    int i=0;

    if (m_key == LV2::INVALID_KEY)
      return;
    float s=0.0;
    p(p_lv2_audio_out_1)[i] += s;
  }

protected:

  unsigned char m_key;
  double m_rate;

  PluginFx fx;
  bool refreshVoice;
  bool normalizeDxVelocity;
  bool sendSysexChange;
  static const int MAX_ACTIVE_NOTES = 16;
  ProcessorVoice voices[MAX_ACTIVE_NOTES];
  int currentNote;
      Lfo lfo;

    bool sustain;
    bool monoMode;
    float extra_buf[N];
    int extra_buf_size;

    int currentProgram;
    long lastStateSave;
    uint32_t engineType;

    FmCore engineMsfa;
    EngineMkI engineMkI;
    EngineOpl engineOpl;
    int feedback_bitdepth;
};


class Dexed : public LV2::Synth<DexedVoice, Dexed> {
public:
  
  Dexed(double rate)
    : LV2::Synth<DexedVoice, Dexed>(p_n_ports, p_lv2_events_in) {
    add_voices(new DexedVoice(rate), new DexedVoice(rate), new DexedVoice(rate));
    add_audio_outputs(p_lv2_audio_out_1);
  }

};


static int _ = Dexed::register_class(p_uri);
