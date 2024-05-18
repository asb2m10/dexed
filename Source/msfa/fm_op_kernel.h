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

#ifndef __FM_OP_KERNEL_H
#define __FM_OP_KERNEL_H

struct FmOpParams {
    int32_t level_in;      // value to be computed (from level to gain[0])
    int32_t gain_out;      // computed value (gain[1] to gain[0])
    int32_t freq;
    int32_t phase;
};

class FmOpKernel {
 public:
  // gain1 and gain2 represent linear step: gain for sample i is
  // gain1 + (1 + i) / 64 * (gain2 - gain1)

  // This is the basic FM operator. No feedback.
  static void compute(int32_t *output, const int32_t *input,
                      int32_t phase0, int32_t freq,
                      int32_t gain1, int32_t gain2, bool add);
  
  // This is a sine generator, no feedback.
  static void compute_pure(int32_t *output, int32_t phase0, int32_t freq,
                           int32_t gain1, int32_t gain2, bool add);

  // One op with feedback, no add.
  static void compute_fb(int32_t *output, int32_t phase0, int32_t freq,
                         int32_t gain1, int32_t gain2,
                         int32_t *fb_buf, int fb_gain, bool add);
};

/// Maximum absolute value of output amplitude of an operator of MSFA Engine.
/// TODO: Its value _should_ _be_ derived from other constants used in this engine by some exact calculation
/// giving the maximum absolute value stored or added to ``output[i]`` in methods ``FmOpKernel::compute()``,
/// ``FmOpKernel::compute_pure()`` and ``FmOpKernel::compute_fb()``.
/// NOTE: this current value is still determined experimentally!
#define MAX_ABS_AMP_FM_CORE_OP 61562217

/// Value of DC bias of output amplitude of an operator of MSFA Engine.
#define DC_BIAS_FM_CORE_OP 1069

#endif
