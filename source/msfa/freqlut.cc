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

// Resolve frequency signal (1.0 in Q24 format = 1 octave) to phase delta.

// The LUT is just a global, and we'll need the init function to be called before
// use.

#include <stdint.h>
#include <math.h>

#include "freqlut.h"

#define LG_N_SAMPLES 10
#define N_SAMPLES (1 << LG_N_SAMPLES)
#define SAMPLE_SHIFT (24 - LG_N_SAMPLES)

#define MAX_LOGFREQ_INT 20

int32_t lut[N_SAMPLES + 1];

void Freqlut::init(double sample_rate) {
  double y = (1LL << (24 + MAX_LOGFREQ_INT)) / sample_rate;
  double inc = pow(2, 1.0 / N_SAMPLES);
  for (int i = 0; i < N_SAMPLES + 1; i++) {
    lut[i] = (int32_t)floor(y + 0.5);
    y *= inc;
  }
}

// Note: if logfreq is more than 20.0, the results will be inaccurate. However,
// that will be many times the Nyquist rate.
int32_t Freqlut::lookup(int32_t logfreq) {
  int ix = (logfreq & 0xffffff) >> SAMPLE_SHIFT;

  int32_t y0 = lut[ix];
  int32_t y1 = lut[ix + 1];
  int lowbits = logfreq & ((1 << SAMPLE_SHIFT) - 1);
  int32_t y = y0 + ((((int64_t)(y1 - y0) * (int64_t)lowbits)) >> SAMPLE_SHIFT);
  int hibits = logfreq >> 24;
  return y >> (MAX_LOGFREQ_INT - hibits);
}
