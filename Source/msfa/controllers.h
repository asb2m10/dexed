/*
 * Copyright 2013 Google Inc.
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

#ifndef __CONTROLLERS_H
#define __CONTROLLERS_H

#include "synth.h"
#include "../Dexed.h"

// State of MIDI controllers
const int kControllerPitch = 128;
const int kControllerPitchRange = 129;
const int kControllerPitchStep = 130;

class FmCore;

struct FmMod {
    int range = 0;
    bool pitch = false;
    bool amp = false;
    bool eg = false;
};

class Controllers {
    void applyMod(int cc, FmMod &mod) {
        float range = 0.01 * mod.range;
        if ( mod.amp ) {
            int total = cc * range;
            if ( amp_mod + total > 127 )
                amp_mod = 127;
            else
                amp_mod += total;
        }
        if ( mod.pitch ) {
            int total = cc * range;
            if ( pitch_mod + total > 127 )
                pitch_mod = 127;
            else
                pitch_mod += total;
        }
        if ( mod.eg ) {
            int total = cc * range;
            if ( eg_mod + total > 127 )
                eg_mod = 127;
            else
                eg_mod += total;
        }
    }
    
public:
    int values_[131];
    
    int amp_mod = 0;
    int pitch_mod = 0;
    int eg_mod = 0;
    
    int aftertouch_cc;
    int breath_cc;
    int foot_cc;
    int modwheel_cc;
    
    FmMod wheel;
    FmMod foot;
    FmMod breath;
    FmMod at;
    
    void refresh() {
        amp_mod = 0;
        pitch_mod = 0;
        eg_mod = 0;
        
        applyMod(modwheel_cc, wheel);
        applyMod(breath_cc, breath);
        applyMod(foot_cc, foot);
        applyMod(aftertouch_cc, at);
        
        TRACE("amp_mod %d pitch_mod %d", amp_mod, pitch_mod);
    }
    
    FmCore *core;
};

#endif  // __CONTROLLERS_H

