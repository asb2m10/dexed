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

#include <math.h>

#include "synth.h"
#include "env.h"

//using namespace std;

uint32_t Env::sr_multiplier = (1<<24);

const int levellut[] = {
    0, 5, 9, 13, 17, 20, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 42, 43, 45, 46
};

void Env::init_sr(double sampleRate) {
    sr_multiplier = (44100.0 / sampleRate) * (1<<24);
}

void Env::init(const int r[4], const int l[4], int32_t ol, int rate_scaling) {
  for (int i = 0; i < 4; i++) {
    rates_[i] = r[i];
    levels_[i] = l[i];
  }
  outlevel_ = ol;
  rate_scaling_ = rate_scaling;
  level_ = 0;
  down_ = true;
  advance(0);
}

int32_t Env::getsample() {
  if (ix_ < 3 || ((ix_ < 4) && !down_)) {
    if (rising_) {
      const int jumptarget = 1716;
      if (level_ < (jumptarget << 16)) {
        level_ = jumptarget << 16;
      }
      level_ += (((17 << 24) - level_) >> 24) * inc_;
      // TODO: should probably be more accurate when inc is large
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
  // TODO: this would be a good place to set level to 0 when under threshold
  return level_;
}

void Env::keydown(bool d) {
  if (down_ != d) {
     down_ = d;
     advance(d ? 0 : 3);
  }
}

int Env::scaleoutlevel(int outlevel) {
  return outlevel >= 20 ? 28 + outlevel : levellut[outlevel];
}

void Env::advance(int newix) {
  ix_ = newix;
  if (ix_ < 4) {
    int newlevel = levels_[ix_];
    int actuallevel = scaleoutlevel(newlevel) >> 1;
    actuallevel = (actuallevel << 6) + outlevel_ - 4256;
    actuallevel = actuallevel < 16 ? 16 : actuallevel;
    // level here is same as Java impl
    targetlevel_ = actuallevel << 16;
    rising_ = (targetlevel_ > level_);

    // rate
    int qrate = (rates_[ix_] * 41) >> 6;
    qrate += rate_scaling_;
    qrate = min(qrate, 63);
    inc_ = (4 + (qrate & 3)) << (2 + LG_N + (qrate >> 2));

    // meh, this should be fixed elsewhere
    inc_ = ((int64_t)inc_ * (int64_t)sr_multiplier) >> 24;
  }
}

void Env::update(const int r[4], const int l[4], int32_t ol, int rate_scaling) {
  for (int i = 0; i < 4; i++) {
      rates_[i] = r[i];
      levels_[i] = l[i];
  }
  outlevel_ = ol;
  rate_scaling_ = rate_scaling;
  if ( down_ ) {
    // for now we simply reset ourselve at level 3
    int newlevel = levels_[2];
    int actuallevel = scaleoutlevel(newlevel) >> 1;
    actuallevel = (actuallevel << 6) - 4256;
    actuallevel = actuallevel < 16 ? 16 : actuallevel;
    targetlevel_ = actuallevel << 16;
    advance(2);
  }
}

void Env::getPosition(char *step) {
    *step = ix_;
}

void Env::transfer(Env &src) {
    for(int i=0;i<4;i++) {
        rates_[i] = src.rates_[i];
        levels_[i] = src.levels_[i];
    }
    outlevel_ = src.outlevel_;
    rate_scaling_ = src.rate_scaling_;
    level_ = src.level_;
    targetlevel_ = src.targetlevel_;
    rising_= src.rising_;
    ix_ = src.ix_;
    inc_ = src.inc_;
    down_ = src.down_;
}

