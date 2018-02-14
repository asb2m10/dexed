/*
 * Copyright 2017 Pascal Gauthier. 
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

#ifndef __ENV_H
#define __ENV_H

#include "synth.h"

// DX7 envelope generation

#define ACCURATE_ENVELOPE

class Env {
 public:

  // The rates and levels arrays are calibrated to match the Dx7 parameters
  // (ie, value 0..99). The outlevel parameter is calibrated in microsteps
  // (ie units of approx .023 dB), with 99 * 32 = nominal full scale. The
  // rate_scaling parameter is in qRate units (ie 0..63).
  void init(const int rates[4], const int levels[4], int outlevel,
      int rate_scaling);

  void update(const int rates[4], const int levels[4], int outlevel,
      int rate_scaling);
  // Result is in Q24/doubling log format. Also, result is subsampled
  // for every N samples.
  // A couple more things need to happen for this to be used as a gain
  // value. First, the # of outputs scaling needs to be applied. Also,
  // modulation.
  // Then, of course, log to linear.
  int32_t getsample();

  void keydown(bool down);
  static int scaleoutlevel(int outlevel);
  void getPosition(char *step);
    
  static void init_sr(double sample_rate);
  void transfer(Env &src);
    
 private:

  // PG: This code is normalized to 44100, need to put a multiplier
  // if we are not using 44100.
  static uint32_t sr_multiplier;

  int rates_[4];
  int levels_[4];
  int outlevel_;
  int rate_scaling_;
  // Level is stored so that 2^24 is one doubling, ie 16 more bits than
  // the DX7 itself (fraction is stored in level rather than separate
  // counter)
  int32_t level_;
  int targetlevel_;
  bool rising_;
  int ix_;
  int inc_;
#ifdef ACCURATE_ENVELOPE
  int staticcount_;
#endif

  bool down_;

  void advance(int newix);
};

#endif  // __ENV_H

