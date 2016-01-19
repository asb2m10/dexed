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

// Low frequency oscillator, compatible with DX7

class Lfo {
 public:
  static void init(double sample_rate);
  void reset(const uint8_t params[6]);

  // result is 0..1 in Q24
  int32_t getsample();

  // result is 0..1 in Q24
  int32_t getdelay();

  void keydown();
 private:
  static uint32_t unit_;

  uint32_t phase_;  // Q32
  uint32_t delta_;
  uint8_t waveform_;
  uint8_t randstate_;
  bool sync_;

  uint32_t delaystate_;
  uint32_t delayinc_;
  uint32_t delayinc2_;
};
