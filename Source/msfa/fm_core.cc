/*
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

#ifdef VERBOSE
#include <iostream>
#endif

#include "synth.h"
#include "exp2.h"
#include "fm_op_kernel.h"
#include "fm_core.h"


//using namespace std;

const FmAlgorithm FmCore::algorithms[32] = {
  { { 0xc1, 0x11, 0x11, 0x14, 0x01, 0x14 } }, // 1
  { { 0x01, 0x11, 0x11, 0x14, 0xc1, 0x14 } }, // 2
  { { 0xc1, 0x11, 0x14, 0x01, 0x11, 0x14 } }, // 3
  { { 0xc1, 0x11, 0x94, 0x01, 0x11, 0x14 } }, // 4
  { { 0xc1, 0x14, 0x01, 0x14, 0x01, 0x14 } }, // 5
  { { 0xc1, 0x94, 0x01, 0x14, 0x01, 0x14 } }, // 6
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

int n_out(const FmAlgorithm &alg) {
  int count = 0;
  for (int i = 0; i < 6; i++) {
    if ((alg.ops[i] & 7) == OUT_BUS_ADD) count++;
  }
  return count;
}

void FmCore::dump() {
#ifdef VERBOSE
  for (int i = 0; i < 32; i++) {
    cout << (i + 1) << ":";
    const FmAlgorithm &alg = algorithms[i];
    for (int j = 0; j < 6; j++) {
      int flags = alg.ops[j];
      cout << " ";
      if (flags & FB_IN) cout << "[";
      cout << (flags & IN_BUS_ONE ? "1" : flags & IN_BUS_TWO ? "2" : "0") << "->";
      cout << (flags & OUT_BUS_ONE ? "1" : flags & OUT_BUS_TWO ? "2" : "0");
      if (flags & OUT_BUS_ADD) cout << "+";
      //cout << alg.ops[j].in << "->" << alg.ops[j].out;
      if (flags & FB_OUT) cout << "]";
    }
    cout << " " << n_out(alg);
    cout << endl;
  }
#endif
}

void FmCore::render(int32_t *output, FmOpParams *params, int algorithm, int32_t *fb_buf, int feedback_shift) {
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
                // todo: more than one op in a feedback loop
                if ((flags & 0xc0) == 0xc0 && feedback_shift < 16) {
                    // cout << op << " fb " << inbus << outbus << add << endl;
                    FmOpKernel::compute_fb(outptr, param.phase, param.freq,
                                           gain1, gain2,
                                           fb_buf, feedback_shift, add);
                } else {
                    // cout << op << " pure " << inbus << outbus << add << endl;
                    FmOpKernel::compute_pure(outptr, param.phase, param.freq,
                                             gain1, gain2, add);
                }
            } else {
                // cout << op << " normal " << inbus << outbus << " " << param.freq << add << endl;
                FmOpKernel::compute(outptr, buf_[inbus - 1].get(),
                                    param.phase, param.freq, gain1, gain2, add);
            }
            has_contents[outbus] = true;
        } else if (!add) {
            has_contents[outbus] = false;
        }
        param.phase += param.freq << LG_N;
    }
}
