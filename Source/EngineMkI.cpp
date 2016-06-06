/*
 * Copyright (C) 2015-2016 Pascal Gauthier.
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
 * The code is based on ppplay https://github.com/stohrendorf/ppplay and opl3
 * math documentation :
 * https://github.com/gtaylormb/opl3_fpga/blob/master/docs/opl3math/opl3math.pdf
 *
 */

#include "EngineMkI.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

#include "msfa/sin.h"
#include "msfa/exp2.h"

#ifdef DEBUG
    #include "time.h"
    //#define MKIDEBUG
#endif

#ifdef _WIN32
    double log2(double n)  {  
        return log(n) / log(2.0);  
    }
    double round(double n) {
        return n < 0.0 ? ceil(n - 0.5) : floor(n + 0.5);
    }
    __declspec(align(16)) int zeros[N] = {0};
#else
    const int32_t __attribute__ ((aligned(16))) zeros[N] = {0};
#endif

static const uint16_t NEGATIVE_BIT = 0x8000;
static const uint16_t ENV_BITDEPTH = 14;

static const uint16_t SINLOG_BITDEPTH = 10;
static const uint16_t SINLOG_TABLESIZE = 1<<SINLOG_BITDEPTH;
static uint16_t sinLogTable[SINLOG_TABLESIZE];

static const uint16_t SINEXP_BITDEPTH = 10;
static const uint16_t SINEXP_TABLESIZE = 1<<SINEXP_BITDEPTH;
static uint16_t sinExpTable[SINEXP_TABLESIZE];

static inline uint16_t sinLog(uint16_t phi) {
    const uint16_t SINLOG_TABLEFILTER = SINLOG_TABLESIZE-1;
    const uint16_t index = (phi & SINLOG_TABLEFILTER);
    
    switch( ( phi & (SINLOG_TABLESIZE * 3) ) ) {
        case 0:
            return sinLogTable[index];
        case SINLOG_TABLESIZE:
            return sinLogTable[index ^ SINLOG_TABLEFILTER];
        case SINLOG_TABLESIZE * 2 :
            return sinLogTable[index] | NEGATIVE_BIT;
        default:
            return sinLogTable[index ^ SINLOG_TABLEFILTER] | NEGATIVE_BIT;
    }
}

EngineMkI::EngineMkI() {
    float bitReso = SINLOG_TABLESIZE;
    
    for(int i=0;i<SINLOG_TABLESIZE;i++) {
        float x1 = sin(((0.5+i)/bitReso) * M_PI/2.0);
        sinLogTable[i] = round(-1024 * log2(x1));
    }
    
    bitReso = SINEXP_TABLESIZE;
    for(int i=0;i<SINEXP_TABLESIZE;i++) {
        float x1 = (pow(2, float(i)/bitReso)-1) * 4096;
        sinExpTable[i] = round(x1);
    }
    
#ifdef MKIDEBUG
    char buffer[4096];
    int pos = 0;
    
    TRACE("****************************************");
    for(int i=0;i<SINLOG_TABLESIZE;i++) {
        pos += sprintf(buffer+pos, "%d ", sinLogTable[i]);
        if ( pos > 90 ) {
            TRACE("SINLOGTABLE: %s" ,buffer);
            buffer[0] = 0;
            pos = 0;
        }
    }
    TRACE("SINLOGTABLE: %s", buffer);
    buffer[0] = 0;
    pos = 0;
    TRACE("----------------------------------------");    
    for(int i=0;i<SINEXP_TABLESIZE;i++) {
        pos += sprintf(buffer+pos, "%d ", sinExpTable[i]);
        if ( pos > 90 ) {
            TRACE("SINEXTTABLE: %s" ,buffer);
            buffer[0] = 0;
            pos = 0;
        }
    }
    TRACE("SINEXTTABLE: %s", buffer);
    TRACE("****************************************");
#endif
}

inline int32_t mkiSin(int32_t phase, uint16_t env) {
    uint16_t expVal = sinLog(phase >> (22 - SINLOG_BITDEPTH)) + (env);
    //int16_t expValShow = expVal;
    
    const bool isSigned = expVal & NEGATIVE_BIT;
    expVal &= ~NEGATIVE_BIT;
    
    const uint16_t SINEXP_FILTER = 0x3FF;
    uint16_t result = 4096 + sinExpTable[( expVal & SINEXP_FILTER ) ^ SINEXP_FILTER];
    
    //uint16_t resultB4 = result;
    result >>= ( expVal >> 10 ); // exp
    
#ifdef MKIDEBUG
    if ( ( time(NULL) % 5 ) == 0 ) {
        if ( expValShow < 0 ) {
            expValShow = (expValShow + 0x7FFF) * -1;
        }
        //TRACE(",%d,%d,%d,%d,%d,%d", phase >> (22 - SINLOG_BITDEPTH), env, expValShow, ( expVal & SINEXP_FILTER ) ^ SINEXP_FILTER, resultB4, result);
    }
#endif
    
    if( isSigned )
        return (-result - 1) << 13;
    else
        return result << 13;
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

/*
void EngineMkI::render(int32_t *output, FmOpParams *params, int algorithm,
                       int32_t *fb_buf, int feedback_shift) {
    const uint16_t ENV_MAX = 1<<ENV_BITDEPTH;
    const uint16_t kLevelThresh = ENV_MAX-100;  // really ???? uhuhuh
    const FmAlgorithm alg = algorithms[algorithm];
    bool has_contents[3] = { true, false, false };
    for (int op = 0; op < 6; op++) {
        int flags = alg.ops[op];
        bool add = (flags & OUT_BUS_ADD) != 0;
        FmOpParams &param = params[op];
        int inbus = (flags >> 4) & 3;
        int outbus = flags & 3;
        int32_t *outptr = (outbus == 0) ? output : buf_[outbus - 1].get();
        int32_t gain1 = param.gain_out == 0 ? (ENV_MAX-1) : param.gain_out;
        int32_t gain2 = ENV_MAX-(param.level_in >> (28-ENV_BITDEPTH));
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
*/

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
        int32_t scaled_fb = (y0 + y) >> (fb_shift + 1);     // tsk tsk tsk: this needs some tuning
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
        int32_t scaled_fb = (y0 + y) >> (fb_shift + 1); 
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


void EngineMkI::render(int32_t *output, FmOpParams *params, int algorithm, int32_t *fb_buf, int feedback_shift) {
    const uint16_t ENV_MAX = 1<<ENV_BITDEPTH;
    const int kLevelThresh = ENV_MAX-100;
    const FmAlgorithm alg = algo2[algorithm];
    bool has_contents[3] = { true, false, false };
    
    for (int op = 0; op < 6; op++) {
        int flags = alg.ops[op];
        bool add = (flags & OUT_BUS_ADD) != 0;
        FmOpParams &param = params[op];
        int inbus = (flags >> 4) & 3;
        int outbus = flags & 3;
        int32_t *outptr = (outbus == 0) ? output : buf_[outbus - 1].get();
        int32_t gain1 = param.gain_out == 0 ? (ENV_MAX-1) : param.gain_out;
        int32_t gain2 = ENV_MAX-(param.level_in >> (28-ENV_BITDEPTH));
        param.gain_out = gain2;
        
        if (gain1 <= kLevelThresh || gain2 <= kLevelThresh) {
            
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
                            compute_fb2(outptr, params, gain1, gain2, fb_buf, feedback_shift);
                            params[1].phase += params[1].freq << LG_N;  // yuk, hack, we already processed op-5
                            op++; // ignore next operator;
                            break;
                        // three operator feedback, process exception for ALGO 4
                        case 3 :
                            compute_fb3(outptr, params, gain1, gain2, fb_buf, feedback_shift);
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

