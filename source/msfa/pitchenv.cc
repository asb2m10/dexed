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

#include "synth.h"
#include "pitchenv.h"

int PitchEnv::unit_;

void PitchEnv::init(double sample_rate) {
  unit_ = N * (1 << 24) / (21.3 * sample_rate) + 0.5;
}

const uint8_t pitchenv_rate[] = {
  1, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12,
  12, 13, 13, 14, 14, 15, 16, 16, 17, 18, 18, 19, 20, 21, 22, 23, 24,
  25, 26, 27, 28, 30, 31, 33, 34, 36, 37, 38, 39, 41, 42, 44, 46, 47,
  49, 51, 53, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 79, 82,
  85, 88, 91, 94, 98, 102, 106, 110, 115, 120, 125, 130, 135, 141, 147,
  153, 159, 165, 171, 178, 185, 193, 202, 211, 232, 243, 254, 255
};

const int8_t pitchenv_tab[] = {
  -128, -116, -104, -95, -85, -76, -68, -61, -56, -52, -49, -46, -43,
  -41, -39, -37, -35, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24,
  -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10,
  -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
  28, 29, 30, 31, 32, 33, 34, 35, 38, 40, 43, 46, 49, 53, 58, 65, 73,
  82, 92, 103, 115, 127
};

void PitchEnv::set(const int r[4], const int l[4]) {
  for (int i = 0; i < 4; i++) {
    rates_[i] = r[i];
    levels_[i] = l[i];
  }
  level_ = pitchenv_tab[l[3]] << 19;
  down_ = true;
  advance(0);
}

int32_t PitchEnv::getsample() {
  if (ix_ < 3 || ((ix_ < 4) && !down_)) {
    if (rising_) {
      level_ += inc_;
      if (level_ >= targetlevel_) {
        level_ = targetlevel_;
        advance(ix_ + 1);
      }
    } else {  // !rising
      level_ -= inc_;
      if (level_ <= targetlevel_) {
        level_ = targetlevel_;
        advance(ix_ + 1);
      }
    }
  }
  return level_;
}

void PitchEnv::keydown(bool d) {
  if (down_ != d) {
    down_ = d;
    advance(d ? 0 : 3);
  }
}

void PitchEnv::advance(int newix) {
  ix_ = newix;
  if (ix_ < 4) {
    int newlevel = levels_[ix_];
    targetlevel_ = pitchenv_tab[newlevel] << 19;
    rising_ = (targetlevel_ > level_);
    inc_ = pitchenv_rate[rates_[ix_]] * unit_;
  }
}

void PitchEnv::getPosition(char *step) {
  *step = ix_;
}


