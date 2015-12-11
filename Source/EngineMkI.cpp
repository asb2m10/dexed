/*
 * Copyright 2014 Pascal Gauthier.
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

#include "EngineMkI.h"

#include <math.h>
#include <cstdlib>

#include "msfa/sin.h"
#include "msfa/exp2.h"

static const uint16_t MKI_BITDEPTH = 8;
static const uint16_t MKI_TABLESIZE = 1<<MKI_BITDEPTH;
static const uint16_t MKI_TABLEFILTER = MKI_TABLESIZE-1;
static const uint16_t NEGATIVE_BIT = 0x8000;

#ifdef _WIN32
    __declspec(align(16)) int zeros[N] = {0};
#else
    const int32_t __attribute__ ((aligned(16))) zeros[N] = {0};
#endif

static uint16_t sinLogTable[MKI_TABLESIZE];
static uint16_t sinExpTable[MKI_TABLESIZE];

EngineMkI::EngineMkI() {
    float bitReso = MKI_TABLESIZE;
    
    for(int i=0;i<MKI_TABLESIZE;i++) {
        float x1 = sin(((0.5+i)/bitReso) * M_PI/2.0);
        sinLogTable[i] = round(-bitReso * log2(x1));
    }
    
    bitReso = MKI_TABLESIZE;
    for(int i=0;i<MKI_TABLESIZE;i++) {
        float x1 = (pow(2, float(i)/bitReso)-1)*1024;
        sinExpTable[i] = round(x1);
    }
}

static inline uint16_t sinLog(uint16_t phi) {
    const uint16_t index = (phi & MKI_TABLEFILTER);
    
    switch( ( phi & (MKI_TABLESIZE*3) ) ) {
        case 0x0000:
            // rising quarter wave  Shape A
            return sinLogTable[index];
        case MKI_TABLESIZE:
            // falling quarter wave  Shape B
            return sinLogTable[index ^ MKI_TABLEFILTER];
        case (MKI_TABLESIZE*2):
            // rising quarter wave -ve  Shape C
            return sinLogTable[index] | NEGATIVE_BIT;
        default:
            // falling quarter wave -ve  Shape D
            return sinLogTable[index ^ MKI_TABLEFILTER] | NEGATIVE_BIT;
    }
}

inline int32_t mkiSin(int32_t phase, uint16_t env) {
    const uint16_t shift = 22 - MKI_BITDEPTH;
    uint16_t expVal = sinLog(phase >> shift) + (env << 3);
    const bool isSigned = expVal & NEGATIVE_BIT;
    
    expVal &= ~NEGATIVE_BIT;
    // expVal: 0..2137+511*8 = 0..6225
    // result: 0..1018+1024
    uint16_t result = 0x0400 + sinExpTable[( expVal & MKI_TABLEFILTER ) ^ MKI_TABLEFILTER];
    result <<= 1;
    result >>= ( expVal >> 8 ); // exp
    
    uint32_t ret;
    if( isSigned ) {
        // -1 for one's complement
        ret = -result - 1;
    } else {
        ret = result;
    }
    return ret << shift;
}

void EngineMkI::compute(int32_t *output, const int32_t *input,
                        int32_t phase0, int32_t freq,
                        int32_t gain1, int32_t gain2, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
    const int32_t *adder = add ? output : zeros;
    
    for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t y = mkiSin((phase+input[i]), gain);
        output[i] = y + adder[i];
        phase += freq;
    }
    
}

void EngineMkI::compute_pure(int32_t *output, int32_t phase0, int32_t freq,
                             int32_t gain1, int32_t gain2, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
    const int32_t *adder = add ? output : zeros;
    
    for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t y = mkiSin(phase , gain);
        output[i] = y + adder[i];
        phase += freq;
    }
}

void EngineMkI::compute_fb(int32_t *output, int32_t phase0, int32_t freq,
                           int32_t gain1, int32_t gain2,
                           int32_t *fb_buf, int fb_shift, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
    const int32_t *adder = add ? output : zeros;
    int32_t y0 = fb_buf[0];
    int32_t y = fb_buf[1];
    
    for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t scaled_fb = (y0 + y) >> (fb_shift + 1);
        y0 = y;
        y = mkiSin((phase+scaled_fb), gain);
        output[i] = y + adder[i];
        phase += freq;
    }
    
    fb_buf[0] = y0;
    fb_buf[1] = y;
}


// exclusively used for ALGO 6 with feedback
void EngineMkI::compute_fb2(int32_t *output, FmOpParams *parms, int32_t gain01, int32_t gain02, int32_t *fb_buf, int fb_shift) {
    int32_t dgain[2];
    int32_t gain[2];
    int32_t phase[2];
    int32_t y0 = fb_buf[0];
    int32_t y = fb_buf[1];
    
    phase[0] = parms[0].phase;
    phase[1] = parms[1].phase;
    
    gain[0] = gain01;
    gain[1] = parms[1].gain_out;
    
    dgain[0] = (gain02 - gain01 + (N >> 1)) >> LG_N;
    
    parms[1].gain_out = Exp2::lookup(parms[1].level_in - (14 * (1 << 24)));
    dgain[1] = (parms[1].gain_out - gain[1] + (N >> 1)) >> LG_N;
 
    for (int i = 0; i < N; i++) {
        // op 0
        gain[0] += dgain[0];
        int32_t scaled_fb = (y0 + y) >> (fb_shift + 2); // tsk tsk tsk: this needs some tuning
        y0 = y;
        y = Sin::lookup(phase[0] + scaled_fb);
        y = ((int64_t)y * (int64_t)gain[0]) >> 24;
        phase[0] += parms[0].freq;
        
        // op 1
        gain[1] += dgain[1];
        y = Sin::lookup(phase[1] + y);
        y = ((int64_t)y * (int64_t)gain[1]) >> 24;
        output[i] = y;
        phase[1] += parms[1].freq;
    }
    fb_buf[0] = y0;
    fb_buf[1] = y;
}

const FmAlgorithm EngineMkI::algo2[32] = {
    { { 0xc1, 0x11, 0x11, 0x14, 0x01, 0x14 } }, // 1
    { { 0x01, 0x11, 0x11, 0x14, 0xc1, 0x14 } }, // 2
    { { 0xc1, 0x11, 0x14, 0x01, 0x11, 0x14 } }, // 3
    { { 0xc4, 0x00, 0x00, 0x01, 0x11, 0x14 } }, // 4 ** EXCEPTION VIA CODE
    { { 0xc1, 0x14, 0x01, 0x14, 0x01, 0x14 } }, // 5
    { { 0xc4, 0x00, 0x01, 0x14, 0x01, 0x14 } }, // 6 ** EXCEPTION VIA CODE
    { { 0xc1, 0x11, 0x05, 0x14, 0x01, 0x14 } }, // 7
    { { 0x01, 0x11, 0xc5, 0x14, 0x01, 0x14 } }, // 8
    { { 0x01, 0x11, 0x05, 0x14, 0xc1, 0x14 } }, // 9
    { { 0x01, 0x05, 0x14, 0xc1, 0x11, 0x14 } }, // 10
    { { 0xc1, 0x05, 0x14, 0x01, 0x11, 0x14 } }, // 11
    { { 0x01, 0x05, 0x05, 0x14, 0xc1, 0x14 } }, // 12
    { { 0xc1, 0x05, 0x05, 0x14, 0x01, 0x14 } }, // 13
    { { 0xc1, 0x05, 0x11, 0x14, 0x01, 0x14 } }, // 14
    { { 0x01, 0x05, 0x11, 0x14, 0xc1, 0x14 } }, // 15
    { { 0xc1, 0x11, 0x02, 0x25, 0x05, 0x14 } }, // 16
    { { 0x01, 0x11, 0x02, 0x25, 0xc5, 0x14 } }, // 17
    { { 0x01, 0x11, 0x11, 0xc5, 0x05, 0x14 } }, // 18
    { { 0xc1, 0x14, 0x14, 0x01, 0x11, 0x14 } }, // 19
    { { 0x01, 0x05, 0x14, 0xc1, 0x14, 0x14 } }, // 20
    { { 0x01, 0x14, 0x14, 0xc1, 0x14, 0x14 } }, // 21
    { { 0xc1, 0x14, 0x14, 0x14, 0x01, 0x14 } }, // 22
    { { 0xc1, 0x14, 0x14, 0x01, 0x14, 0x04 } }, // 23
    { { 0xc1, 0x14, 0x14, 0x14, 0x04, 0x04 } }, // 24
    { { 0xc1, 0x14, 0x14, 0x04, 0x04, 0x04 } }, // 25
    { { 0xc1, 0x05, 0x14, 0x01, 0x14, 0x04 } }, // 26
    { { 0x01, 0x05, 0x14, 0xc1, 0x14, 0x04 } }, // 27
    { { 0x04, 0xc1, 0x11, 0x14, 0x01, 0x14 } }, // 28
    { { 0xc1, 0x14, 0x01, 0x14, 0x04, 0x04 } }, // 29
    { { 0x04, 0xc1, 0x11, 0x14, 0x04, 0x04 } }, // 30
    { { 0xc1, 0x14, 0x04, 0x04, 0x04, 0x04 } }, // 31
    { { 0xc4, 0x04, 0x04, 0x04, 0x04, 0x04 } }, // 32
};

// exclusively used for ALGO 4 with feedback
void EngineMkI::compute_fb3(int32_t *output, FmOpParams *parms, int32_t gain01, int32_t gain02, int32_t *fb_buf, int fb_shift) {
    int32_t dgain[3];
    int32_t gain[3];
    int32_t phase[3];
    int32_t y0 = fb_buf[0];
    int32_t y = fb_buf[1];
    
    phase[0] = parms[0].phase;
    phase[1] = parms[1].phase;
    phase[2] = parms[2].phase;
    
    gain[0] = gain01;
    gain[1] = parms[1].gain_out;
    gain[2] = parms[2].gain_out;
    
    dgain[0] = (gain02 - gain01 + (N >> 1)) >> LG_N;
    
    parms[1].gain_out = Exp2::lookup(parms[1].level_in - (14 * (1 << 24)));
    dgain[1] = (parms[1].gain_out - gain[1] + (N >> 1)) >> LG_N;
    parms[2].gain_out = Exp2::lookup(parms[2].level_in - (14 * (1 << 24)));
    dgain[2] = (parms[1].gain_out - gain[2] + (N >> 1)) >> LG_N;
    
    for (int i = 0; i < N; i++) {
        // op 0
        gain[0] += dgain[0];
        int32_t scaled_fb = (y0 + y) >> (fb_shift + 6);     // tsk tsk tsk: this needs some tuning
        y0 = y;
        y = Sin::lookup(phase[0] + scaled_fb);
        y = ((int64_t)y * (int64_t)gain[0]) >> 24;
        phase[0] += parms[0].freq;
        
        // op 1
        gain[1] += dgain[1];
        y = Sin::lookup(phase[1] + y);
        y = ((int64_t)y * (int64_t)gain[1]) >> 24;
        phase[1] += parms[1].freq;
        
        // op 2
        gain[2] += dgain[2];
        y = Sin::lookup(phase[2] + y);
        y = ((int64_t)y * (int64_t)gain[2]) >> 24;
        output[i] = y;
        phase[2] += parms[2].freq;
    }
    fb_buf[0] = y0;
    fb_buf[1] = y;
}

void EngineMkI::render(int32_t *output, FmOpParams *params, int algorithm,
                       int32_t *fb_buf, int feedback_shift) {
    const int kLevelThresh = 507;  // really ????
    const FmAlgorithm alg = algorithms[algorithm];
    bool has_contents[3] = { true, false, false };
    for (int op = 0; op < 6; op++) {
        int flags = alg.ops[op];
        bool add = (flags & OUT_BUS_ADD) != 0;
        FmOpParams &param = params[op];
        int inbus = (flags >> 4) & 3;
        int outbus = flags & 3;
        int32_t *outptr = (outbus == 0) ? output : buf_[outbus - 1].get();
        int32_t gain1 = param.gain_out == 0 ? 511 : param.gain_out;
        int32_t gain2 = 512-(param.level_in >> 19);
        param.gain_out = gain2;
        
        if (gain1 <= kLevelThresh || gain2 <= kLevelThresh) {
            if (!has_contents[outbus]) {
                add = false;
            }
            if (inbus == 0 || !has_contents[inbus]) {
                // todo: more than one op in a feedback loop
                if ((flags & 0xc0) == 0xc0 && feedback_shift < 16) {
                    // cout << op << " fb " << inbus << outbus << add << endl;
                    compute_fb(outptr, param.phase, param.freq,
                               gain1, gain2,
                               fb_buf, feedback_shift, add);
                } else {
                    // cout << op << " pure " << inbus << outbus << add << endl;
                    compute_pure(outptr, param.phase, param.freq,
                                 gain1, gain2, add);
                }
            } else {
                // cout << op << " normal " << inbus << outbus << " " << param.freq << add << endl;
                compute(outptr, buf_[inbus - 1].get(),
                        param.phase, param.freq, gain1, gain2, add);
            }
            has_contents[outbus] = true;
        } else if (!add) {
            has_contents[outbus] = false;
        }
        param.phase += param.freq << LG_N;
    }
}

/*
void EngineMkI::render(int32_t *output, FmOpParams *params, int algorithm, int32_t *fb_buf, int feedback_shift) {
    const int kLevelThresh = 507;
    const FmAlgorithm alg = algo2[algorithm];
    bool has_contents[3] = { true, false, false };
    
    for (int op = 0; op < 6; op++) {
        int flags = alg.ops[op];
        bool add = (flags & OUT_BUS_ADD) != 0;
        FmOpParams &param = params[op];
        int inbus = (flags >> 4) & 3;
        int outbus = flags & 3;
        int32_t *outptr = (outbus == 0) ? output : buf_[outbus - 1].get();
        int32_t gain1 = param.gain_out == 0 ? 511 : param.gain_out;
        int32_t gain2 = 512-(param.level_in >> 19);
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
                        // two operator feedback, process exception for ALGO 6
                        case 5 :
                            //compute_fb2(outptr, params, gain1, gain2, fb_buf, feedback_shift, controllers);
                            params[1].phase += params[1].freq << LG_N;  // yuk, hack, we already processed op-5
                            op++; // ignore next operator;
                            break;
                        // three operator feedback, process exception for ALGO 4
                        case 3 :
                            //compute_fb3(outptr, params, gain1, gain2, fb_buf, feedback_shift, controllers);
                            params[1].phase += params[1].freq << LG_N; // hack, we already processed op-5 - op-4
                            params[2].phase += params[2].freq << LG_N; // yuk yuk
                            op += 2; // ignore the 2 other operators
                            break;
                        default:
                            // one operator feedback, normal proces
                            //cout << "\t" << op << " fb " << inbus << outbus << add << endl;
                            compute_fb(outptr, param.phase, param.freq,gain1, gain2, fb_buf, feedback_shift, add);
                            break;
                    }
                } else {
                    // cout << op << " pure " << inbus << outbus << add << endl;
                    compute_pure(outptr, param.phase, param.freq, gain1, gain2, add);
                }
            } else {
                
                // cout << op << " normal " << inbus << outbus << " " << param.freq << add << endl;
                compute(outptr, buf_[inbus - 1].get(), param.phase, param.freq, gain1, gain2, add);
            }
            
            has_contents[outbus] = true;
            
        } else if (!add) {
            has_contents[outbus] = false;
        }
        param.phase += param.freq << LG_N;
    }
}
*/
