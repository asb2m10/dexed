// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lvtk/synth.hpp>
#include <string.h>
#include "dexed-ttl.h"
#include "dexed.h"
#include "EngineMkI.h"
#include "EngineOpl.h"
#include "msfa/exp2.h"
#include "msfa/sin.h"
#include "msfa/freqlut.h"
#include "msfa/controllers.h"
#include "msfa/ringbuffer.h"

Dexed::Dexed(double rate) : lvtk::Synth<DexedVoice, Dexed>(p_n_ports, p_lv2_events_in)
{
  TRACE("Hi");

  bufsize_=256;
  outbuf16_=new int16_t[bufsize_];

  Exp2::init();
  Tanh::init();
  Sin::init();

  lastStateSave = 0;
  currentNote = -1;
  engineType = -1;
  monoMode = 0;
  normalizeDxVelocity = false;

  memset(&voiceStatus, 0, sizeof(VoiceStatus));
  setEngineType(DEXED_ENGINE_MARKI);

  Freqlut::init(rate);
  Lfo::init(rate);
  PitchEnv::init(rate);
  Env::init_sr(rate);
  fx.init(rate);

  for (int note = 0; note < MAX_ACTIVE_NOTES; ++note) {
    voices[note].dx7_note = new Dx7Note;
    voices[note].keydown = false;
    voices[note].sustained = false;
    voices[note].live = false;
  }

  currentNote = 0;
  controllers.values_[kControllerPitch] = 0x2000;
  controllers.modwheel_cc = 0;
  controllers.foot_cc = 0;
  controllers.breath_cc = 0;
  controllers.aftertouch_cc = 0;

  sustain = false;

  lfo.reset(data + 137);

  add_voices(new DexedVoice(rate));

  add_audio_outputs(p_lv2_audio_out_1);
}

Dexed::~Dexed()
{
  TRACE("Hi");

  delete [] outbuf16_;

  TRACE("Bye");
}

//==============================================================================

DexedVoice::DexedVoice(double rate) : m_key(lvtk::INVALID_KEY), m_rate(rate)
{
  TRACE("Hi");

  TRACE("Bye");
}

DexedVoice::~DexedVoice()
{
  TRACE("Hi");

  TRACE("Bye");
}

void DexedVoice::on(unsigned char key, unsigned char velocity)
{
  TRACE("Hi");

  m_key = key;

  TRACE("Bye");
}

void DexedVoice::off(unsigned char velocity)
{
  TRACE("Hi");

  m_key = lvtk::INVALID_KEY;

  TRACE("Bye");
}

unsigned char DexedVoice::get_key(void) const
{
  TRACE("Hi");

  return m_key;

  TRACE("Bye");
}

#ifdef DEBUG
void dexed_trace(const char *source, const char *fmt, ...) {
    char output[4096];
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(output, 4095, fmt, argptr);
    va_end(argptr);
    printf("%s: %s\n",source,output);
}
#endif

static int _ = Dexed::register_class(p_uri);
