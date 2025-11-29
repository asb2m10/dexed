/*
 * Copyright 2016-2017 Pascal Gauthier.
 * Copyright 2012 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYNTH_DX7NOTE_H_
#define SYNTH_DX7NOTE_H_

// This is the logic to put together a note from the MIDI description
// and run the low-level modules.

// It will continue to evolve a bit, as note-stealing logic, scaling,
// and real-time control of parameters live here.

#include "env.h"
#include "pitchenv.h"
#include "fm_core.h"
#include "tuning.h"
#include "porta.h"
#include "libMTSClient.h"
#include <memory>

struct VoiceStatus {
    uint32_t amp[6];
    char ampStep[6];
    char pitchStep;
};

class Dx7Note {
public:
    Dx7Note(std::shared_ptr<TuningState> ts, MTSClient *mtsc);
    void init(const uint8_t patch[156], int midinote, int velocity, int channel, const Controllers *ctrls);
    void initPortamento(const Dx7Note &srcNote);

    // Note: this _adds_ to the buffer. Interesting question whether it's
    // worth it...
    void compute(int32_t *buf, int32_t lfo_val, int32_t lfo_delay,
                 const Controllers *ctrls);
    
    void keyup();
    
    bool isPlaying();
    
    // PG:add the update
    void update(const uint8_t patch[156], int midinote, int velocity, int channel);
    void updateBasePitches();
    void peekVoiceStatus(VoiceStatus &status);
    void transferState(Dx7Note& src);
    void transferSignal(Dx7Note &src);
    void transferPhase(Dx7Note &src);
    void oscSync();

    // We should put this as a function and not a DX7Note method
    //int32_t osc_freq(int midinote, int mode, int coarse, int fine, int detune);

    std::shared_ptr<TuningState> tuning_state_;

    int mpePitchBend = 8192;
    int mpePressure = 0;
    int mpeTimbre = 0;
    
private:
    bool initialised_;
    Env env_[6];
    FmOpParams params_[6];
    PitchEnv pitchenv_;
    int32_t basepitch_[6];
    int32_t fb_buf_[2];
    int32_t fb_shift_;
    int32_t ampmodsens_[6];
    int32_t opMode[6];

    uint8_t playingMidiNote; // We need this for scale aware pitch bend
    uint8_t midiChannel;
    
    int ampmoddepth_;
    int algorithm_;
    int pitchmoddepth_;
    int pitchmodsens_;
    
    const uint8_t *currentPatch;
    
    int32_t porta_curpitch_[6];

    //int32_t noteLogFreq;
    double mtsFreq;
    static const int32_t mtsLogFreqToNoteLogFreq;
    MTSClient *mtsClient;
    int32_t osc_freq(int midinote, int mode, int coarse, int fine, int detune, int channel);
};

#endif  // SYNTH_DX7NOTE_H_
