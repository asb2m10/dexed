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

#ifndef SYNTH_MODULE_H
#define SYNTH_MODULE_H

#include <stdint.h>

class Module {
 public:
  static const int lg_n = 6;
  static const int n = 1 << lg_n;
  virtual void process(const int32_t **inbufs, const int32_t *control_in,
					   const int32_t *control_last, int32_t **outbufs) = 0;
};

#endif  // SYNTH_MODULE_H

