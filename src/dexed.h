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
#include "dexed.peg"
#ifdef DEBUG
#include "trace.h"
#endif

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

// GLOBALS

//==============================================================================

class DexedVoice : public lvtk::Voice
{
  public:
    DexedVoice(double rate);
    ~DexedVoice();
    void on(unsigned char key, unsigned char velocity);
    void off(unsigned char velocity);
    unsigned char get_key(void) const;

  protected:
    unsigned char m_key;
    double m_rate;

  private:
};

//==============================================================================

class Dexed : public lvtk::Synth<DexedVoice, Dexed>
{
  public:
    Dexed(double rate);
    ~Dexed();
    void run(uint32_t sample_count);
    int getEngineType();
    void setEngineType(int rs);
    bool isMonoMode(void);
    void setMonoMode(bool mode);
    void set_params(void);
    void GetSamples(int n_samples, float *buffer);

    uint8_t data[161];
    Controllers controllers;
    VoiceStatus voiceStatus;

  protected:
    bool ProcessMidiMessage(const uint8_t *buf, int buf_size);
    void onParam(int param_num,int param_val);
    void keyup(uint8_t pitch);
    void keydown(uint8_t pitch, uint8_t velo);
    void panic(void);

    static const int MAX_ACTIVE_NOTES = 16;
    ProcessorVoice voices[MAX_ACTIVE_NOTES];
    int currentNote;
    bool sustain;
    bool monoMode;
    bool refreshVoice;
    bool normalizeDxVelocity;
    uint32_t engineType;
    int feedback_bitdepth;
    long lastStateSave;
    PluginFx fx;
    Lfo lfo;
    FmCore engineMsfa;
    EngineMkI engineMkI;
    EngineOpl engineOpl;
    float* outbuf_;
    uint32_t bufsize_;
    int16_t extra_buf_[N];
    int extra_buf_size_;

  private:
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
