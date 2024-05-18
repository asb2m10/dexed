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

#ifndef ENGINEOPL_H_INCLUDED
#define ENGINEOPL_H_INCLUDED

#include "msfa/synth.h"
#include "msfa/aligned_buf.h"
#include "msfa/fm_op_kernel.h"
#include "msfa/controllers.h"
#include "msfa/fm_core.h"


class EngineOpl : public FmCore {
public:
    virtual void render(int32_t *output, FmOpParams *params, int algorithm,
                        int32_t *fb_buf, int feedback_shift) override;
    void compute(int32_t *output, const int32_t *input, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2, bool add);
	void compute_pure(int32_t *output, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2, bool add);
    void compute_fb(int32_t *output, int32_t phase0, int32_t freq,
                    int32_t gain1, int32_t gain2,
                    int32_t *fb_buf, int fb_gain, bool add);
};

/// Maximum absolute value of output amplitude of an operator of OPL Engine.
/// TODO: Its value _should_ _be_ derived from other constants used in this engine by some exact calculation
/// giving the maximum absolute value assigned or added to ``output[i]`` in methods ``EngineOpl::compute()``,
/// ``EngineOplFmOpKernel::compute_pure()`` and ``EngineOpl::compute_fb()``.
/// NOTE: this current value is still determined experimentally!
#define MAX_ABS_AMP_OPL_OP 61495214

/// Value of DC bias of output amplitude of an operator of OPL Engine.
#define DC_BIAS_OPL_OP 1069

#endif  // ENGINEOPL_H_INCLUDED
