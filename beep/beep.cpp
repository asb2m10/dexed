#include <lv2synth.hpp>
#include "beep.peg"


class BeepVoice : public LV2::Voice {
public:
  
  BeepVoice(double rate) 
    : m_key(LV2::INVALID_KEY), m_rate(rate), m_period(10), m_counter(0) {
    
  }
  
  void on(unsigned char key, unsigned char velocity) { 
    m_key = key;
    m_period = m_rate * 4.0 / LV2::key2hz(m_key);
  }

  void off(unsigned char velocity) { 
    m_key = LV2::INVALID_KEY;
  }

  unsigned char get_key() const { 
    return m_key; 
  }

  void render(uint32_t from, uint32_t to) {
    if (m_key == LV2::INVALID_KEY)
      return;
    for (uint32_t i = from; i < to; ++i) {
      float s = -0.25 + 0.5 * (m_counter > m_period / 2);
      m_counter = (m_counter + 1) % m_period;
      p(p_left)[i] += s;
      p(p_right)[i] += s;
    }
  }

protected:

  unsigned char m_key;
  double m_rate;
  uint32_t m_period;
  uint32_t m_counter;
  
};


class Beep : public LV2::Synth<BeepVoice, Beep> {
public:
  
  Beep(double rate)
    : LV2::Synth<BeepVoice, Beep>(p_n_ports, p_midi) {
    add_voices(new BeepVoice(rate), new BeepVoice(rate), new BeepVoice(rate));
    add_audio_outputs(p_left, p_right);
  }

};


static int _ = Beep::register_class(p_uri);
