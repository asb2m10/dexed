/*
  beep.cpp  -  LV2 Toolkit API Demonstration Plugin

  Copyright (C) 2004-2010  Lars Luthman
  Updated for LVTK by Michael Fisher <mfisher31@gmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
/** @file beep.cpp */


#include <lvtk/synth.hpp>
#include "beep.peg"

class BeepVoice : public lvtk::Voice
{
public:
  
    BeepVoice (double rate)
        : m_key(lvtk::INVALID_KEY), m_rate(rate), m_period(10), m_counter(0)
    { }
  
    void
    on (unsigned char key, unsigned char velocity)
    {
        m_key = key;
        m_period = m_rate * 4.0 / lvtk::key2hz(m_key);
    }

    void
    off (unsigned char velocity)
    {
        m_key = lvtk::INVALID_KEY;
    }

    unsigned char
    get_key() const
    {
        return m_key;
    }

    void
    render(uint32_t from, uint32_t to)
    {
        if (m_key == lvtk::INVALID_KEY) return;

        for (uint32_t i = from; i < to; ++i)
        {
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


class Beep : public lvtk::Synth<BeepVoice, Beep>
{
public:
  
    Beep (double rate) : lvtk::Synth<BeepVoice, Beep> (p_n_ports, p_midi)
    {
        add_voices (new BeepVoice(rate), new BeepVoice(rate), new BeepVoice(rate));
        add_audio_outputs (p_left, p_right);
    }
};


static int _ = Beep::register_class(p_uri);
