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

class Sin {
 public:
  Sin();

  static void init();
  static int32_t lookup(int32_t phase);
  static int32_t compute(int32_t phase);

  // A more accurate sine, both input and output Q30
  static int32_t compute10(int32_t phase);
};

#define SIN_LG_N_SAMPLES 10
#define SIN_N_SAMPLES (1 << SIN_LG_N_SAMPLES)

#define SIN_INLINE

// Use twice as much RAM for the LUT but avoid a little computation
#define SIN_DELTA

#ifdef SIN_DELTA
extern int32_t sintab[SIN_N_SAMPLES << 1];
#else
extern int32_t sintab[SIN_N_SAMPLES + 1];
#endif

#ifdef SIN_INLINE
inline
int32_t Sin::lookup(int32_t phase) {
  const int SHIFT = 24 - SIN_LG_N_SAMPLES;
  int lowbits = phase & ((1 << SHIFT) - 1);
#ifdef SIN_DELTA
  int phase_int = (phase >> (SHIFT - 1)) & ((SIN_N_SAMPLES - 1) << 1);
  int dy = sintab[phase_int];
  int y0 = sintab[phase_int + 1];

  return y0 + (((int64_t)dy * (int64_t)lowbits) >> SHIFT);
#else
  int phase_int = (phase >> SHIFT) & (SIN_N_SAMPLES - 1);
  int y0 = sintab[phase_int];
  int y1 = sintab[phase_int + 1];

  return y0 + (((int64_t)(y1 - y0) * (int64_t)lowbits) >> SHIFT);
#endif
}
#endif
