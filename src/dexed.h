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
#include "msfa/ringbuffer.h"
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

#ifdef DEBUG
    #define TRACE(fmt, ...) _trace(__PRETTY_FUNCTION__,fmt,##__VA_ARGS__)
#else
    #define TRACE(fmt, ...)
#endif

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
};

//==============================================================================

class Dexed : public lvtk::Synth<DexedVoice, Dexed>
{
  public:
    Dexed(double rate);
    ~Dexed();
    int getEngineType();
    void setEngineType(int rs);
    bool isMonoMode();
    void setMonoMode(bool mode);
    void setParameter (int index, int newValue);

    uint8_t data[161];
    Controllers controllers;
    VoiceStatus voiceStatus;

  protected:
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
    int16_t* outbuf16_;
    uint32_t bufsize_;
};

// GLOBALS
RingBuffer ring_buffer_;
//static const float scaler = 0.00003051757813;
static const float scaler = 0.00003;

#endif  // PLUGINPROCESSOR_H_INCLUDED
