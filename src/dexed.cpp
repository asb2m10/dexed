// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lv2synth.hpp>
#include "dexed.peg"
#include "dexed.h"
#include "EngineMkI.h"
#include "EngineOpl.h"
#include "msfa/exp2.h"
#include "msfa/sin.h"

DexedVoice::DexedVoice(double rate) : m_key(LV2::INVALID_KEY), m_rate(rate)
{
  Exp2::init();
  Tanh::init();
  Sin::init();
  
};

void DexedVoice::on(unsigned char key, unsigned char velocity)
{
}

void DexedVoice::off(unsigned char velocity)
{
}

unsigned char DexedVoice::get_key(void) const
{
  return m_key;
}

void DexedVoice::render(uint32_t from, uint32_t to)
{
  if (m_key == LV2::INVALID_KEY)
    return;
}


//==============================================================================

Dexed::Dexed(double rate) : LV2::Synth<DexedVoice, Dexed>(p_n_ports, p_lv2_events_in)
{
    feedback_bitdepth=11;
    engineType=DEXED_ENGINE_MARKI;
    core=&engineMkI;

    add_voices(new DexedVoice(rate));
    add_audio_outputs(p_lv2_audio_out_1);
}

static int _ = Dexed::register_class(p_uri);
