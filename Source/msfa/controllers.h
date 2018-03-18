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
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define snprintf _snprintf
#endif

// State of MIDI controllers
const int kControllerPitch = 128;
const int kControllerPitchRange = 129;
const int kControllerPitchStep = 130;

class FmCore;

struct FmMod {
    int range;
    bool pitch;
    bool amp;
    bool eg;
    
    FmMod() {
        range = 0;
        pitch = false;
        amp = false;
        eg = false;
    }

    void parseConfig(const char *cfg) {
        int r = 0, p = 0, a = 0, e = 0;
        sscanf(cfg, "%d %d %d %d", &r, &p, &a, &e);
        
        range = r < 0 && r > 127 ? 0 : r;
        pitch = p != 0;
        amp = a != 0;
        eg = e != 0;
    }
    
    void setConfig(char *cfg) {
        snprintf(cfg, 13, "%d %d %d %d", range, pitch, amp, eg);
    }
};

class Controllers {
    void applyMod(int cc, FmMod &mod) {
        float range = 0.01 * mod.range;
        int total = cc * range;
        if ( mod.amp )
            amp_mod = max(amp_mod, total);
        
        if ( mod.pitch )
            pitch_mod = max(pitch_mod, total);
        
        if ( mod.eg )
            eg_mod = max(eg_mod, total);
    }
    
public:
    int values_[131];
    
    char opSwitch[7];
    
    int amp_mod;
    int pitch_mod;
    int eg_mod;
    
    int aftertouch_cc;
    int breath_cc;
    int foot_cc;
    int modwheel_cc;
    
    int masterTune;
    
    FmMod wheel;
    FmMod foot;
    FmMod breath;
    FmMod at;
    
    Controllers() {
        amp_mod = 0;
        pitch_mod = 0;
        eg_mod = 0;
        strcpy(opSwitch, "111111");        
    }

    void refresh() {
        amp_mod = 0;
        pitch_mod = 0;
        eg_mod = 0;
        
        applyMod(modwheel_cc, wheel);
        applyMod(breath_cc, breath);
        applyMod(foot_cc, foot);
        applyMod(aftertouch_cc, at);
        
        if ( ! ((wheel.eg || foot.eg) || (breath.eg || at.eg)) )
            eg_mod = 127;
        
        TRACE("controllers refresh>>> amp_mod %d pitch_mod %d", amp_mod, pitch_mod);
    }
    
    FmCore *core;
};

#endif  // __CONTROLLERS_H

