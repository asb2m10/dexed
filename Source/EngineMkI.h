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

#ifndef ENGINEMKI_H_INCLUDED
#define ENGINEMKI_H_INCLUDED

#include "msfa/synth.h"
#include "msfa/aligned_buf.h"
#include "msfa/fm_op_kernel.h"
#include "msfa/controllers.h"
#include "msfa/fm_core.h"


class EngineMkI : public FmCore {
public:
    EngineMkI();
    
    void render(int32_t *output, FmOpParams *params, int algorithm, int32_t *fb_buf, int32_t feedback_shift) override;
    
    void compute(int32_t *output, const int32_t *input, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2,
                    bool add);
    
	void compute_pure(int32_t *output, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2,
                         bool add);
    
    void compute_fb(int32_t *output, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2,
                       int32_t *fb_buf, int fb_gain, bool add);
    
    void compute_fb2(int32_t *output, FmOpParams *params, int32_t gain01, int32_t gain02, int32_t *fb_buf, int fb_shift);
    
    void compute_fb3(int32_t *output, FmOpParams *params, int32_t gain01, int32_t gain02, int32_t *fb_buf, int fb_shift);
};

/// Maximum absolute value of output amplitude of an operator of MkI Engine.
/// TODO: Its value _should_ _be_ derived from other constants used in this engine by some exact calculation
/// giving the maximum absolute value stored or added to ``output[i]`` in methods ``EngineMkI::compute()``,
/// ``EngineMkI::compute_pure()`` and ``EngineMkI::::compute_fb()``.
/// NOTE: this current value is still determined experimentally! 61613110 
#define MAX_ABS_AMP_MKI_OP 6113110


/// Value of DC bias of output amplitude of an operator of MkI Engine.
#define DC_BIAS_MKI_OP 1069

#endif  // ENGINEMKI_H_INCLUDED
