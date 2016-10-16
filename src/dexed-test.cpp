#include "EngineMkI.h"
#include "EngineOpl.h"
#include "msfa/exp2.h"
#include "msfa/sin.h"
#include "msfa/freqlut.h"
#include "msfa/controllers.h"
#include "msfa/dx7note.h"
#include "msfa/lfo.h"
#include "msfa/synth.h"
#include "msfa/fm_core.h"

#include <cstdint>
#include <limits>

template<class T> float normalize(const T x) {
    const float valMin = std::numeric_limits<T>::min();
    const float valMax = std::numeric_limits<T>::max();
    return 2 * (x - valMin) / (valMax - valMin) - 1; // note: 0 does not become 0.
}

struct ProcessorVoice {
    int midi_note;
    bool keydown;
    bool sustained;
    bool live;
    Dx7Note *dx7_note;
};

// GLOBALS
FmCore engineMsfa;
EngineMkI engineMkI;
EngineOpl engineOpl;

Lfo lfo;
Controllers controllers;
const int rate=44100;
const uint8_t init_voice[] =
      { 99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 99, 0, 1, 0, 7,
        99, 99, 99, 99, 50, 50, 50, 50, 0, 0, 1, 35, 0, 0, 0, 1, 0, 3, 24,
        73, 78, 73, 84, 32, 86, 79, 73, 67, 69 };
uint8_t data[156]; // program data
ProcessorVoice voice;
uint8_t feedback_bitdepth=11;

int main(int argc, char **argv)
{
  Exp2::init();
  Tanh::init();
  Sin::init();
  Freqlut::init(rate);
  Lfo::init(rate);
  PitchEnv::init(rate);
  Env::init_sr(rate);

  uint32_t i;

  for(i=0;i<sizeof(init_voice);i++) {
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
  controllers.core=&engineMsfa;

  lfo.reset(data + 137);

  voice.dx7_note=new Dx7Note;

  unsigned char key=64;
  unsigned char velocity=100;

  voice.midi_note=key+data[144] - 24;
  voice.keydown=true;
  voice.live=false;
  voice.sustained=false;
  lfo.keydown();

  if(voice.live==false)
  {
    voice.dx7_note->init(data, key, velocity, feedback_bitdepth);
    voice.live=true;
  }
  if(data[136])
    voice.dx7_note->oscSync();

  int32_t s;
  for(i=0;i<256;++i)
  {
    if(voice.live==true)
    {
      voice.dx7_note->compute(&s, lfo.getsample(), lfo.getdelay(), &controllers);
      float fs=normalize(s);
      printf("%f %d %d\n",fs, lfo.getsample(), lfo.getdelay());
    }
  } 
}

