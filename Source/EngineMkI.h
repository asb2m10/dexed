/*
  ==============================================================================

    EngineMkI.h
    Created: 25 Aug 2014 12:08:00am
    Author:  Pascal Gauthier

  ==============================================================================
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
    virtual void compute(int32_t *output, FmOpParams *params, int algorithm,
                         int32_t *fb_buf, int feedback_shift, const Controllers *controllers);
    
    static void compute_fb2(int32_t *output, FmOpParams *params, int32_t *fb_buf, int fb_shift);
    
    static void compute_fb3(int32_t *output, FmOpParams *params, int32_t *fb_buf, int fb_shift);
};


#endif  // ENGINEMKI_H_INCLUDED
