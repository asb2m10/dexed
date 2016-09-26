// from: http://ll-plugins.nongnu.org/lv2pftci/#A_synth

#include <lvtk/synth.hpp>
#include "beep.peg"

class BeepVoice : public lvtk::Voice {
public:
  
  BeepVoice(double rate) 
    : m_key(lvtk::INVALID_KEY), m_rate(rate), m_period(10), m_counter(0) {
    
  }
  
  void on(unsigned char key, unsigned char velocity) { 
    m_key = key;
    m_period = m_rate * 4.0 / lvtk::key2hz(m_key);
    m_pos = std::rand() / float(RAND_MAX);
    m_envelope = velocity / 128.0;
  }

  void off(unsigned char velocity) { 
    m_key = lvtk::INVALID_KEY;
  }

  unsigned char get_key() const { 
    return m_key; 
  }

  void render(uint32_t from, uint32_t to) {
    if (m_key == lvtk::INVALID_KEY)
      return;
    for (uint32_t i = from; i < to; ++i) {
      float pwm = *p(p_pwm) + (1 - *p(p_pwm)) * m_envelope;
      float s = -0.25 + 0.5 * (m_counter > m_period * (1 + pwm) / 2);
      m_counter = (m_counter + 1) % m_period;
      p(p_left)[i] += (1 - m_pos) * s;
      p(p_right)[i] += m_pos * s;
      if (m_envelope > 0)
	m_envelope -= 0.5 / m_rate;
    }
  }

protected:

  unsigned char m_key;
  double m_rate;
  uint32_t m_period;
  uint32_t m_counter;
  float m_pos;
  float m_envelope;
  
};


class Beep : public lvtk::Synth<BeepVoice, Beep> {
public:
  
  Beep(double rate)
    : lvtk::Synth<BeepVoice, Beep>(p_n_ports, p_midi),
      m_buf_pos(0), m_delay(rate / 3), 
      m_l_buffer(new float[m_delay]), m_r_buffer(new float[m_delay]) {
    add_voices(new BeepVoice(rate), new BeepVoice(rate), new BeepVoice(rate));
    add_audio_outputs(p_left, p_right);
    for (unsigned i = 0; i < m_delay; ++i) {
      m_l_buffer[i] = 0;
      m_r_buffer[i] = 0;
    }
  }

  void post_process(uint32_t from, uint32_t to) {
    for (uint32_t i = from; i < to; ++i) {
      float mono = (p(p_left)[i] + p(p_right)[i]) / 2;
      p(p_left)[i] += m_l_buffer[m_buf_pos];
      p(p_right)[i] += m_r_buffer[m_buf_pos];
      float tmp = m_l_buffer[m_buf_pos];
      m_l_buffer[m_buf_pos] = 0.6 * (mono + m_r_buffer[m_buf_pos]);
      m_r_buffer[m_buf_pos] = 0.6 * tmp;
      m_buf_pos = (m_buf_pos + 1) % m_delay;
      p(p_left)[i] *= *p(p_gain);
      p(p_right)[i] *= *p(p_gain);
    }
  }
  
protected:
  
  unsigned m_buf_pos;
  unsigned m_delay;
  float* m_l_buffer;
  float* m_r_buffer;

};


static int _ = Beep::register_class(p_uri);
