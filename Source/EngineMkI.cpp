/**
 *
 * Copyright (c) 2014 Pascal Gauthier.
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

#include "EngineMkI.h"

#include <math.h>
#include <cstdlib>

#include "msfa/sin.h"
#include "msfa/exp2.h"

void EngineMkI::compute(int32_t *output, const int32_t *input,
                        int32_t phase0, int32_t freq,
                        int32_t gain1, int32_t gain2, bool add, const Controllers *controllers) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
    if (add) {
        for (int i = 0; i < N; i++) {
            gain += dgain;
            int32_t y = Sin::lookup(phase + input[i]);
            y &= controllers->sinBitFilter;
            int32_t y1 = ((int64_t)y * (int64_t)gain) >> 24;
            output[i] += y1;
            phase += freq;
        }
    } else {
        for (int i = 0; i < N; i++) {
            gain += dgain;
            int32_t y = Sin::lookup(phase + input[i]);
            y &= controllers->sinBitFilter;
            int32_t y1 = ((int64_t)y * (int64_t)gain) >> 24;
            output[i] = y1;
            phase += freq;
        }
    }
    
}

void EngineMkI::compute_pure(int32_t *output, int32_t phase0, int32_t freq,
                             int32_t gain1, int32_t gain2, bool add, const Controllers *controllers) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
    if (add) {
        for (int i = 0; i < N; i++) {
            gain += dgain;
            int32_t y = Sin::lookup(phase);
            y &= controllers->sinBitFilter;
            int32_t y1 = ((int64_t)y * (int64_t)gain) >> 24;
            output[i] += y1;
            phase += freq;
        }
    } else {
        for (int i = 0; i < N; i++) {
            gain += dgain;
            int32_t y = Sin::lookup(phase);
            y &= controllers->sinBitFilter;
            int32_t y1 = ((int64_t)y * (int64_t)gain) >> 24;
            output[i] = y1;
            phase += freq;
        }
    }
}

void EngineMkI::compute_fb(int32_t *output, int32_t phase0, int32_t freq,
                           int32_t gain1, int32_t gain2,
                           int32_t *fb_buf, int fb_shift, bool add, const Controllers *controllers) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
    int32_t y0 = fb_buf[0];
    int32_t y = fb_buf[1];
    if (add) {
        for (int i = 0; i < N; i++) {
            gain += dgain;
            int32_t scaled_fb = (y0 + y) >> (fb_shift + 1);
            y0 = y;
            y = Sin::lookup(phase + scaled_fb);
            y &= controllers->sinBitFilter;
            y = ((int64_t)y * (int64_t)gain) >> 24;
            output[i] += y;
            phase += freq;
        }
    } else {
        for (int i = 0; i < N; i++) {
            gain += dgain;
            int32_t scaled_fb = (y0 + y) >> (fb_shift + 1);
            y0 = y;
            y = Sin::lookup(phase + scaled_fb);
            y &= controllers->sinBitFilter;
            y = ((int64_t)y * (int64_t)gain) >> 24;
            output[i] = y;
            phase += freq;
        }
    }
    fb_buf[0] = y0;
    fb_buf[1] = y;
}

/*
// exclusively used for ALGO 6 with feedback
void EngineMkI::compute_fb2(int32_t *output, FmOpParams *parms, int32_t *fb_buf, int fb_shift, const Controllers *cont) {
    int32_t dgain[2];
    int32_t gain[2];
    int32_t phase[2];
    int32_t y0 = fb_buf[0];
    int32_t y = fb_buf[1];
    
    phase[0] = parms[0].phase;
    phase[1] = parms[1].phase;
    
    dgain[0] = (parms[0].gain[1] - parms[0].gain[0] + (N >> 1)) >> LG_N;
    dgain[1] = (parms[1].gain[1] - parms[1].gain[0] + (N >> 1)) >> LG_N;
    
    gain[0] = parms[0].gain[0];
    gain[1] = parms[1].gain[1];
    
    for (int i = 0; i < N; i++) {
        // op 0
        gain[0] += dgain[0];
        int32_t scaled_fb = (y0 + y) >> (fb_shift + 1);
        y0 = y;
        y = Sin::lookup(phase[0] + scaled_fb);
        y = ((int64_t)y * (int64_t)gain) >> 24;
        phase[0] += parms[0].freq;
        
        // op 1
        gain[1] += dgain[1];
        scaled_fb = (y0 + y) >> (fb_shift + 1);
        y0 = y;
        y = Sin::lookup(phase[1] + scaled_fb + y);
        y = ((int64_t)y * (int64_t)gain) >> 24;
        output[i] = y;
        phase[1] += parms[1].freq;
    }
    fb_buf[0] = y0;
    fb_buf[1] = y;
}

// exclusively used for ALGO 4 with feedback
void EngineMkI::compute_fb3(int32_t *output, FmOpParams *parms, int32_t *fb_buf, int fb_shift, const Controllers *conts) {
    int32_t dgain[3];
    int32_t gain[3];
    int32_t phase[3];
    int32_t y0 = fb_buf[0];
    int32_t y = fb_buf[1];
    
    phase[0] = parms[0].phase;
    phase[1] = parms[1].phase;
    phase[2] = parms[2].phase;
    
    dgain[0] = (parms[0].gain[1] - parms[0].gain[0] + (N >> 1)) >> LG_N;
    dgain[1] = (parms[1].gain[1] - parms[1].gain[0] + (N >> 1)) >> LG_N;
    dgain[2] = (parms[2].gain[1] - parms[2].gain[0] + (N >> 1)) >> LG_N;
    
    gain[0] = parms[0].gain[0];
    gain[1] = parms[1].gain[0];
    gain[2] = parms[2].gain[0];
    
    for (int i = 0; i < N; i++) {
        // op 0
        gain[0] += dgain[0];
        int32_t scaled_fb = (y0 + y) >> (fb_shift + 1);
        y0 = y;
        y = Sin::lookup(phase[0] + scaled_fb);
        y = ((int64_t)y * (int64_t)gain) >> 24;
        phase[0] += parms[0].freq;
        
        // op 1
        gain[1] += dgain[1];
        scaled_fb = (y0 + y) >> (fb_shift + 1);
        y0 = y;
        y = Sin::lookup(phase[1] + scaled_fb + y);
        y = ((int64_t)y * (int64_t)gain) >> 24;
        phase[1] += parms[1].freq;
        
        // op 2
        gain[2] += dgain[2];
        scaled_fb = (y0 + y) >> (fb_shift + 1);
        y0 = y;
        y = Sin::lookup(phase[2] + scaled_fb + y);
        y = ((int64_t)y * (int64_t)gain) >> 24;
        output[i] = y;
        phase[2] += parms[2].freq;      
    }
    fb_buf[0] = y0;
    fb_buf[1] = y;
}
*/

void EngineMkI::render(int32_t *output, FmOpParams *params, int algorithm,
                       int32_t *fb_buf, int feedback_shift, const Controllers *controllers) {
    const int kLevelThresh = 1120;
    const FmAlgorithm alg = algorithms[algorithm];
    bool has_contents[3] = { true, false, false };
    
    for (int op = 0; op < 6; op++) {
        int flags = alg.ops[op];
        bool add = (flags & OUT_BUS_ADD) != 0;
        FmOpParams &param = params[op];
        int inbus = (flags >> 4) & 3;
        int outbus = flags & 3;
        int32_t *outptr = (outbus == 0) ? output : buf_[outbus - 1].get();
        int32_t gain1 = param.gain_out;
        int32_t gain2 = Exp2::lookup(param.level_in - (14 * (1 << 24)));
        param.gain_out = gain2;
        
        if (gain1 >= kLevelThresh || gain2 >= kLevelThresh) {
            
            if (!has_contents[outbus]) {
                add = false;
            }
            
            if (inbus == 0 || !has_contents[inbus]) {
                // PG: this is my 'dirty' implementation of FB for 2 and 3 operators...
                // still needs some tuning...
                if ((flags & 0xc0) == 0xc0 && feedback_shift < 16) {
                    switch ( algorithm ) {
                            /*        // two operator feedback, process exception for ALGO 6
                             case 5 :
                             EngineMkI::compute_fb2(outptr, params, fb_buf, feedback_shift);
                             param.phase += param.freq << LG_N;
                             params[1].phase += param.freq + params[1].freq << LG_N;  // yuk, hack, we already processed op-5
                             op++; // ignore next operator;
                             break;
                             // three operator feedback, process exception for ALGO 4
                             case 3 :
                             FmOpKernel::compute_fb3(outptr, params, fb_buf, feedback_shift);
                             param.phase += param.freq << LG_N;
                             params[1].phase += param.freq + params[1].freq << LG_N; // hack, we already processed op-5 - op-4
                             params[2].phase += param.freq + params[1].freq + params[2].freq << LG_N; // yuk yuk
                             op += 2; // ignore the 2 other operators
                             break;*/
                        default:
                            // one operator feedback, normal proces
                            //cout << "\t" << op << " fb " << inbus << outbus << add << endl;
                            compute_fb(outptr, param.phase, param.freq,gain1, gain2, fb_buf, feedback_shift, add, controllers);
                            param.phase += param.freq << LG_N;
                            break;
                    }
                    has_contents[outbus] = true;
                    continue;
                } else {
                    // cout << op << " pure " << inbus << outbus << add << endl;
                    compute_pure(outptr, param.phase, param.freq, gain1, gain2, add, controllers);
                }
                
            } else {
                
                // cout << op << " normal " << inbus << outbus << " " << param.freq << add << endl;
                compute(outptr, buf_[inbus - 1].get(),
                        param.phase, param.freq, gain1, gain2, add, controllers);
            }
            
            has_contents[outbus] = true;
            
        } else if (!add) {
            has_contents[outbus] = false;
            
        }
        param.phase += param.freq << LG_N;
    }
}

