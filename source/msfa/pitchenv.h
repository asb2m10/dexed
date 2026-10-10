/*
 * Copyright 2013 Google Inc.
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

#ifndef __PITCHENV_H
#define __PITCHENV_H

// Computation of the DX7 pitch envelope

class PitchEnv {
 public:
  static void init(double sample_rate);

  // The rates and levels arrays are calibrated to match the Dx7 parameters
  // (ie, value 0..99).
  void set(const int rates[4], const int levels[4]);

  // Result is in Q24/octave
  int32_t getsample();
  void keydown(bool down);
  void getPosition(char *step);
 private:
  static int unit_;
  int rates_[4];
  int levels_[4];
  int32_t level_;
  int targetlevel_;
  bool rising_;
  int ix_;
  int inc_;

  bool down_;

  void advance(int newix);
};

extern const uint8_t pitchenv_rate[];
extern const int8_t pitchenv_tab[];

 #endif  // __PITCHENV_H

