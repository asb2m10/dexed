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

#ifndef __FM_CORE_H
#define __FM_CORE_H

#include "aligned_buf.h"
#include "synth.h"

struct FmOpParams {
  int32_t gain[2];
  int32_t freq;
  int32_t phase;
};

class FmCore {
 public:
  static void dump();
  void compute(int32_t *output, FmOpParams *params, int algorithm,
               int32_t *fb_buf, int32_t feedback_gain);
 private:
  AlignedBuf<int32_t, N>buf_[2];
};

#endif  // __FM_CORE_H
