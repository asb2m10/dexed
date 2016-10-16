// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lvtk/synth.hpp>
#include <string.h>
#include "dexed.peg"
#include "dexed.h"
#include "EngineMkI.h"
#include "EngineOpl.h"
#include "msfa/exp2.h"
#include "msfa/sin.h"
#include "msfa/freqlut.h"
#include "msfa/controllers.h"

Dexed::Dexed(double rate) : lvtk::Synth<DexedVoice, Dexed>(p_n_ports, p_lv2_events_in)
{
  TRACE("Hi");

  Exp2::init();
  Tanh::init();
  Sin::init();

  feedback_bitdepth=11;

  for(uint i=0;i<sizeof(init_voice);i++) {
    data[i] = init_voice[i];
  }

  controllers.values_[kControllerPitchRange] = 3;
  controllers.values_[kControllerPitchStep] = 0;
  controllers.masterTune = 0;
  controllers.values_[kControllerPitch] = 0x2000;
  controllers.modwheel_cc = 0;
  controllers.foot_cc = 0;
  controllers.breath_cc = 0;
  controllers.aftertouch_cc = 0;

  controllers.core=&engineMkI;

  lfo.reset(data + 137);

  Freqlut::init(rate);
  Lfo::init(rate);
  PitchEnv::init(rate);
  Env::init_sr(rate);

  //add_voices(new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate),new DexedVoice(rate));

  add_voices(new DexedVoice(rate));

  add_audio_outputs(p_lv2_audio_out_1);
}

Dexed::~Dexed()
{
  TRACE("Bye");
}

//==============================================================================

DexedVoice::DexedVoice(double rate) : m_key(lvtk::INVALID_KEY), m_rate(rate)
{
  voice_number=voice_counter++;
  TRACE("Hi %d",voice_number);
  voice.dx7_note=new Dx7Note;
}

DexedVoice::~DexedVoice()
{
  TRACE("Bye");
}

void DexedVoice::on(unsigned char key, unsigned char velocity)
{
  m_key = key;
  voice.midi_note=data[144] - 24;
  voice.keydown=true;
  voice.sustained=false;
  lfo.keydown();
  if(voice.live==false)
  {
    voice.dx7_note->init(data, key, velocity, feedback_bitdepth);
    voice.live=true;
  }
  if(data[136])
    voice.dx7_note->oscSync();

  TRACE("%d key-down: %d %d",voice_number,key,velocity);
}

void DexedVoice::off(unsigned char velocity)
{
  voice.dx7_note->keyup();
  voice.keydown=false;
  voice.live=false;
  m_key = lvtk::INVALID_KEY;
  TRACE("%d key-up: %d",voice_number,velocity);
}

unsigned char DexedVoice::get_key(void) const
{
  return m_key;
}

void DexedVoice::render(uint32_t from, uint32_t to)
{
  int32_t s;

  TRACE("Hi");
  TRACE("%d from: %d to: %d",voice_number,from,to);

  if (m_key != lvtk::INVALID_KEY)
  {
    for (uint32_t i = from; i < to; ++i)
    {
      if(voice.live==true)
      {
        voice.dx7_note->compute(&s, lfo.getsample(), lfo.getdelay(), &controllers);
        float fs=(float)s/INT32_MAX;
        p(p_lv2_audio_out_1)[i]+=fs;
	TRACE("out: %2.5f",fs);
      }
    } 
  }
  TRACE("Bye");
}

void DexedVoice::post_process(uint32_t from, uint32_t to)
{
  TRACE("Hi");
  for (uint32_t i = from; i < to; ++i)
  {
    p(p_lv2_audio_out_1)[i] *= *p(p_output);
  }
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
