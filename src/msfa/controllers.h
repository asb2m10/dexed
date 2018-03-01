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
#include <stdio.h>
#include <string.h>
#include "trace.h"

#ifdef _WIN32
#define snprintf _snprintf
#endif

// State of MIDI controllers
const int kControllerPitch = 0;
const int kControllerPitchRange = 1;
const int kControllerPitchStep = 2;

class FmCore;

struct FmMod {
    uint8_t range;
    bool pitch;
    bool amp;
    bool eg;
    
    FmMod() {
        range = 0;
        pitch = false;
        amp = false;
        eg = false;
    }

    void setRange(uint8_t r) {
        range = r < 0 && r > 127 ? 0 : r;
	TRACE("range=%d",range);
    }

    void setTarget(uint8_t assign) {
	TRACE("Target: %d", assign);
        assign=assign < 0 && assign > 7 ? 0 : assign;
        pitch=assign&1; // AMP
	amp=assign&2; // PITCH
	eg=assign&4; // EG

	TRACE("pitch[%d] amp[%d] eg[%d]", pitch,amp,eg);
    }
};

class Controllers {
    void applyMod(int cc, FmMod &mod) {
        float range = 0.01 * mod.range;
        uint8_t total = (float)cc * range;
        TRACE("amp[%d]|pitch[%d]|eg[%d]",mod.amp,mod.pitch,mod.eg);
        TRACE("range=%f mod.range=%d total=%d cc=%d",range,mod.range,total,cc);
        if(mod.amp)
          amp_mod = max(amp_mod, total);
        
        if(mod.pitch)
          pitch_mod = max(pitch_mod, total);
        
        if(mod.eg)
	  eg_mod = max(eg_mod, total);
    }
    
public:
    int32_t values_[3];
    
    uint8_t amp_mod;
    uint8_t pitch_mod;
    uint8_t eg_mod;
    
    uint8_t aftertouch_cc;
    uint8_t breath_cc;
    uint8_t foot_cc;
    uint8_t modwheel_cc;
    
    int masterTune;

    uint8_t opSwitch;

    FmMod wheel;
    FmMod foot;
    FmMod breath;
    FmMod at;
    
    Controllers() {
        amp_mod = 0;
        pitch_mod = 0;
        eg_mod = 0;
    }

    void refresh() {
        amp_mod=pitch_mod=eg_mod=0;

        applyMod(modwheel_cc, wheel);
        applyMod(breath_cc, breath);
        applyMod(foot_cc, foot);
        applyMod(aftertouch_cc, at);
        
        if ( ! ((wheel.eg || foot.eg) || (breath.eg || at.eg)) )
            eg_mod = 127;
    }
    
    FmCore *core;
};

#endif  // __CONTROLLERS_H
