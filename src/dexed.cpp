// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lvtk/synth.hpp>
#include "dexed.peg"
#include "dexed.h"
#include "EngineMkI.h"
#include "EngineOpl.h"
#include "msfa/exp2.h"
#include "msfa/sin.h"
#include "msfa/freqlut.h"
#include "msfa/controllers.h"

DexedVoice::DexedVoice(double rate) : m_key(lvtk::INVALID_KEY), m_rate(rate)
{
  voice.dx7_note=new Dx7Note;
}

void DexedVoice::on(unsigned char key, unsigned char velocity)
{
  m_key = key;
  voice.dx7_note->init(data, key, velocity, feedback_bitdepth);
  voice.keydown=true;
  printf("Dexed: key-down: %d %d\n",key,velocity);
}

void DexedVoice::off(unsigned char velocity)
{
  voice.dx7_note->keyup();
  voice.keydown=false;
  m_key = lvtk::INVALID_KEY;
  printf("Dexed: key-up: %d\n",velocity);
}

unsigned char DexedVoice::get_key(void) const
{
  return m_key;
}

void DexedVoice::render(uint32_t from, uint32_t to)
{
  if (m_key == lvtk::INVALID_KEY)
    return;

  for (uint32_t i = from; i < to; ++i)
  {
    int32_t s;
    //dx7_note->compute(&s, lfovalue, lfodelay, &controllers);
    voice.dx7_note->compute(&s, lfo.getsample(), lfo.getdelay(), &controllers);
    float fs=float(s)/(INT32_MAX);
    p(p_lv2_audio_out_1)[i]+=fs;
  } 
}

void DexedVoice::post_process(uint32_t from, uint32_t to)
{
  for (uint32_t i = from; i < to; ++i)
  {
    p(p_lv2_audio_out_1)[i] *= *p(p_output);
  }
}

//==============================================================================

Dexed::Dexed(double rate) : lvtk::Synth<DexedVoice, Dexed>(p_n_ports, p_lv2_events_in)
{
  Exp2::init();
  Tanh::init();
  Sin::init();
  Freqlut::init(rate);
  Lfo::init(rate);
  PitchEnv::init(rate);
  Env::init_sr(rate);

  engineType=DEXED_ENGINE_MARKI;
  feedback_bitdepth=11;

  controllers.core=&engineMkI;

  for(uint i=0;i<sizeof(init_voice);i++) {
    data[i] = init_voice[i];
  }

  add_voices(new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate));

  add_audio_outputs(p_lv2_audio_out_1);
}

static int _ = Dexed::register_class(p_uri);
