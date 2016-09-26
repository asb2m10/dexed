/**
 *
 * Copyright (c) 2013-2015 Pascal Gauthier.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 */

#ifndef DEXED_H_INCLUDED
#define DEXED_H_INCLUDED

#include "msfa/controllers.h"
#include "msfa/dx7note.h"
#include "msfa/lfo.h"
#include "msfa/synth.h"
#include "msfa/fm_core.h"
#include "PluginFx.h"
#include "EngineMkI.h"
#include "EngineOpl.h"

struct ProcessorVoice {
    int midi_note;
    bool keydown;
    bool sustained;
    bool live;
    Dx7Note *dx7_note;
};

enum DexedEngineResolution {
    DEXED_ENGINE_MODERN,
    DEXED_ENGINE_MARKI,
    DEXED_ENGINE_OPL
};

//==============================================================================

class DexedVoice : public lvtk::Voice
{
  public:
    DexedVoice(double rate, uint8_t fb);
    void on(unsigned char key, unsigned char velocity);
    void off(unsigned char velocity);
    unsigned char get_key(void) const;
    void render(uint32_t from, uint32_t to);
    void post_process(uint32_t from, uint32_t to);

  protected:
    unsigned char m_key;
    double m_rate;
    uint8_t feedback_bitdepth;
    ProcessorVoice voice;
};

//==============================================================================

class Dexed : public lvtk::Synth<DexedVoice, Dexed>
{
  public:
    Dexed(double rate);
    FmCore *core;
    uint8_t get_feedback_bitdepth(void);
    uint32_t get_engineType(void);

  protected:
    // dexed internal
    FmCore engineMsfa;
    EngineMkI engineMkI;
    EngineOpl engineOpl;
    uint8_t feedback_bitdepth;
    uint32_t engineType;
};

const char init_voice[] =
      { 99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 7,
        99, 99, 99, 99, 99, 99, 99, 00, 39, 0, 0, 0, 0, 0, 0, 0, 99, 0, 1, 0, 7,
        99, 99, 99, 99, 50, 50, 50, 50, 0, 0, 1, 35, 0, 0, 0, 1, 0, 3, 24,
        73, 78, 73, 84, 32, 86, 79, 73, 67, 69 };

uint8_t data[161]; // program data

#endif  // PLUGINPROCESSOR_H_INCLUDED
