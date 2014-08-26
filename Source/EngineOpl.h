/*
  ==============================================================================

    ExtraKernels.h
    Created: 20 Aug 2014 8:21:43pm
    Author:  Pascal Gauthier

  ==============================================================================
*/

#ifndef EXTRAKERNELS_H_INCLUDED
#define EXTRAKERNELS_H_INCLUDED

#include "synth.h"
#include "aligned_buf.h"
#include "fm_op_kernel.h"
#include "controllers.h"
#include "fm_core.h"


class EngineOpl : public FmCore {
public:
    virtual void compute(int32_t *output, FmOpParams *params, int algorithm,
                         int32_t *fb_buf, int feedback_shift, const Controllers *controllers);
    void computeOpl(int32_t *output, const int32_t *input, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2, bool add);
	void computeOpl_pure(int32_t *output, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2, bool add);
    void computeOpl_fb(int32_t *output, int32_t phase0, int32_t freq,
                    int32_t gain1, int32_t gain2,
                    int32_t *fb_buf, int fb_gain, bool add);
};



#endif  // EXTRAKERNELS_H_INCLUDED
