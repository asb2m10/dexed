// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lv2synth.hpp>
#include "dexed.peg"
#include "dexed.h"
#include "EngineMkI.h"
#include "EngineOpl.h"
#include "msfa/exp2.h"
#include "msfa/sin.h"

DexedVoice::DexedVoice(double rate)
{
  Exp2::init();
  Tanh::init();
  Sin::init();
};

Dexed::Dexed(double rate) : LV2::Synth<DexedVoice, Dexed>(p_n_ports, p_lv2_events_in)
{
    add_voices(new DexedVoice(rate), new DexedVoice(rate), new DexedVoice(rate));
    add_audio_outputs(p_lv2_audio_out_1);
}

static int _ = Dexed::register_class(p_uri);
