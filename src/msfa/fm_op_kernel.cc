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

#include <math.h>

#include <cstdlib>

#ifdef HAVE_NEON
#include <cpu-features.h>
#endif

#include "synth.h"
#include "sin.h"
#include "fm_op_kernel.h"

#ifdef HAVE_NEONx
static bool hasNeon() {
  return true;
  return (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
}

extern "C"
void neon_fm_kernel(const int *in, const int *busin, int *out, int count,
  int32_t phase0, int32_t freq, int32_t gain1, int32_t dgain);

const int32_t __attribute__ ((aligned(16))) zeros[N] = {0};

#else
static bool hasNeon() {
  return false;
}
#endif

void FmOpKernel::compute(int32_t *output, const int32_t *input,
                         int32_t phase0, int32_t freq,
                         int32_t gain1, int32_t gain2, bool add) {
  int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
  int32_t gain = gain1;
  int32_t phase = phase0;
  if (hasNeon()) {
#ifdef HAVE_NEON
    neon_fm_kernel(input, add ? output : zeros, output, N,
      phase0, freq, gain, dgain);
#endif
  } else {
    if (add) {
      for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t y = Sin::lookup(phase + input[i]);
        int32_t y1 = ((int64_t)y * (int64_t)gain) >> 24;
        output[i] += y1;
        phase += freq;
      }
    } else {
      for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t y = Sin::lookup(phase + input[i]);
        int32_t y1 = ((int64_t)y * (int64_t)gain) >> 24;
        output[i] = y1;
        phase += freq;
      }
    }
  }
}

void FmOpKernel::compute_pure(int32_t *output, int32_t phase0, int32_t freq,
                              int32_t gain1, int32_t gain2, bool add) {
  int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
  int32_t gain = gain1;
  int32_t phase = phase0;
  if (hasNeon()) {
#ifdef HAVE_NEON
    neon_fm_kernel(zeros, add ? output : zeros, output, N,
      phase0, freq, gain, dgain);
#endif
  } else {
    if (add) {
      for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t y = Sin::lookup(phase);
        int32_t y1 = ((int64_t)y * (int64_t)gain) >> 24;
        output[i] += y1;
        phase += freq;
      }
    } else {
      for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t y = Sin::lookup(phase);
        int32_t y1 = ((int64_t)y * (int64_t)gain) >> 24;          
        output[i] = y1;
        phase += freq;
      }
    }
  }
}

#define noDOUBLE_ACCURACY
#define HIGH_ACCURACY

void FmOpKernel::compute_fb(int32_t *output, int32_t phase0, int32_t freq,
                            int32_t gain1, int32_t gain2,
                            int32_t *fb_buf, int fb_shift, bool add) {
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
      y = ((int64_t)y * (int64_t)gain) >> 24;
      output[i] = y;
      phase += freq;
    }
  }
  fb_buf[0] = y0;
  fb_buf[1] = y;
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

// Experimental sine wave generators below
#if 0
// Results: accuracy 64.3 mean, 170 worst case
// high accuracy: 5.0 mean, 49 worst case
void FmOpKernel::compute_pure(int32_t *output, int32_t phase0, int32_t freq,
                              int32_t gain1, int32_t gain2, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
#ifdef HIGH_ACCURACY
    int32_t u = Sin::compute10(phase << 6);
    u = ((int64_t)u * gain) >> 30;
    int32_t v = Sin::compute10((phase << 6) + (1 << 28));  // quarter cycle
    v = ((int64_t)v * gain) >> 30;
    int32_t s = Sin::compute10(freq << 6);
    int32_t c = Sin::compute10((freq << 6) + (1 << 28));
#else
    int32_t u = Sin::compute(phase);
    u = ((int64_t)u * gain) >> 24;
    int32_t v = Sin::compute(phase + (1 << 22));  // quarter cycle
    v = ((int64_t)v * gain) >> 24;
    int32_t s = Sin::compute(freq) << 6;
    int32_t c = Sin::compute(freq + (1 << 22)) << 6;
#endif
    for (int i = 0; i < N; i++) {
        output[i] = u;
        int32_t t = ((int64_t)v * (int64_t)c - (int64_t)u * (int64_t)s) >> 30;
        u = ((int64_t)u * (int64_t)c + (int64_t)v * (int64_t)s) >> 30;
        v = t;
    }
}
#endif

#if 0
// Results: accuracy 392.3 mean, 15190 worst case (near freq = 0.5)
// for freq < 0.25, 275.2 mean, 716 worst
// high accuracy: 57.4 mean, 7559 worst
//  freq < 0.25: 17.9 mean, 78 worst
void FmOpKernel::compute_pure(int32_t *output, int32_t phase0, int32_t freq,
                              int32_t gain1, int32_t gain2, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
#ifdef HIGH_ACCURACY
    int32_t u = floor(gain * sin(phase * (M_PI / (1 << 23))) + 0.5);
    int32_t v = floor(gain * cos((phase - freq * 0.5) * (M_PI / (1 << 23))) + 0.5);
    int32_t a = floor((1 << 25) * sin(freq * (M_PI / (1 << 24))) + 0.5);
#else
    int32_t u = Sin::compute(phase);
    u = ((int64_t)u * gain) >> 24;
    int32_t v = Sin::compute(phase + (1 << 22) - (freq >> 1));
    v = ((int64_t)v * gain) >> 24;
    int32_t a = Sin::compute(freq >> 1) << 1;
#endif
    for (int i = 0; i < N; i++) {
        output[i] = u;
        v -= ((int64_t)a * (int64_t)u) >> 24;
        u += ((int64_t)a * (int64_t)v) >> 24;
    }
}
#endif

#if 0
// Results: accuracy 370.0 mean, 15480 worst case (near freq = 0.5)
// with double accuracy initialization: mean 1.55, worst 58 (near freq = 0)
// with high accuracy: mean 4.2, worst 292 (near freq = 0.5)
void FmOpKernel::compute_pure(int32_t *output, int32_t phase0, int32_t freq,
                              int32_t gain1, int32_t gain2, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
#ifdef DOUBLE_ACCURACY
    int32_t u = floor((1 << 30) * sin(phase * (M_PI / (1 << 23))) + 0.5);
    double a_d = sin(freq * (M_PI / (1 << 24)));
    int32_t v = floor((1LL << 31) * a_d * cos((phase - freq * 0.5) *
                                              (M_PI / (1 << 23))) + 0.5);
    int32_t aa = floor((1LL << 31) * a_d * a_d + 0.5);
#else
#ifdef HIGH_ACCURACY
    int32_t u = Sin::compute10(phase << 6);
    int32_t v = Sin::compute10((phase << 6) + (1 << 28) - (freq << 5));
    int32_t a = Sin::compute10(freq << 5);
    v = ((int64_t)v * (int64_t)a) >> 29;
    int32_t aa = ((int64_t)a * (int64_t)a) >> 29;
#else
    int32_t u = Sin::compute(phase) << 6;
    int32_t v = Sin::compute(phase + (1 << 22) - (freq >> 1));
    int32_t a = Sin::compute(freq >> 1);
    v = ((int64_t)v * (int64_t)a) >> 17;
    int32_t aa = ((int64_t)a * (int64_t)a) >> 17;
#endif
#endif
    
    if (aa < 0) aa = (1 << 31) - 1;
    for (int i = 0; i < N; i++) {
        gain += dgain;
        output[i] = ((int64_t)u * (int64_t)gain) >> 30;
        v -= ((int64_t)aa * (int64_t)u) >> 29;
        u += v;
    }
}
#endif

#if 0
// Results:: accuracy 112.3 mean, 4262 worst (near freq = 0.5)
// high accuracy 2.9 mean, 143 worst
void FmOpKernel::compute_pure(int32_t *output, int32_t phase0, int32_t freq,
                              int32_t gain1, int32_t gain2, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
#ifdef HIGH_ACCURACY
    int32_t u = Sin::compute10(phase << 6);
    int32_t lastu = Sin::compute10((phase - freq) << 6);
    int32_t a = Sin::compute10((freq << 6) + (1 << 28)) << 1;
#else
    int32_t u = Sin::compute(phase) << 6;
    int32_t lastu = Sin::compute(phase - freq) << 6;
    int32_t a = Sin::compute(freq + (1 << 22)) << 7;
#endif
    if (a < 0 && freq < 256) a = (1 << 31) - 1;
    if (a > 0 && freq > 0x7fff00) a = -(1 << 31);
    for (int i = 0; i < N; i++) {
        gain += dgain;
        output[i] = ((int64_t)u * (int64_t)gain) >> 30;
        //output[i] = u;
        int32_t newu = (((int64_t)u * (int64_t)a) >> 30) - lastu;
        lastu = u;
        u = newu;
    }
}
#endif

