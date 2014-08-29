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

#ifndef ENGINEMKI_H_INCLUDED
#define ENGINEMKI_H_INCLUDED

#include "synth.h"
#include "aligned_buf.h"
#include "fm_op_kernel.h"
#include "controllers.h"
#include "fm_core.h"


class EngineMkI : public FmCore {
public:
    virtual void render(int32_t *output, FmOpParams *params, int algorithm,
                        int32_t *fb_buf, int feedback_shift, const Controllers *controllers);
    
    void compute(int32_t *output, const int32_t *input, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2,
                    bool add, const Controllers *controllers);
    
	void compute_pure(int32_t *output, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2,
                         bool add, const Controllers *controllers);
    
    void compute_fb(int32_t *output, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2,
                       int32_t *fb_buf, int fb_gain, bool add, const Controllers *controllers);
    
    void compute_fb2(int32_t *output, FmOpParams *params, int32_t *fb_buf, int fb_shift, const Controllers *controllers);
    
    void compute_fb3(int32_t *output, FmOpParams *params, int32_t *fb_buf, int fb_shift, const Controllers *controllers);
};


#endif  // ENGINEMKI_H_INCLUDED
