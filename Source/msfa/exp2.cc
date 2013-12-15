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

#define _USE_MATH_DEFINES
#include <math.h>

#include "synth.h"
#include "exp2.h"

#include <stdio.h>

#ifdef _MSC_VER
#define exp2(arg) pow(2.0, arg)
#endif



int32_t exp2tab[EXP2_N_SAMPLES << 1];

void Exp2::init() {
  double inc = exp2(1.0 / EXP2_N_SAMPLES);
  double y = 1 << 30;
  for (int i = 0; i < EXP2_N_SAMPLES; i++) {
    exp2tab[(i << 1) + 1] = (int32_t)floor(y + 0.5);
    y *= inc;
  }
  for (int i = 0; i < EXP2_N_SAMPLES - 1; i++) {
    exp2tab[i << 1] = exp2tab[(i << 1) + 3] - exp2tab[(i << 1) + 1];
  }
  exp2tab[(EXP2_N_SAMPLES << 1) - 2] = (1U << 31) - exp2tab[(EXP2_N_SAMPLES << 1) - 1];
}

int32_t tanhtab[TANH_N_SAMPLES << 1];

static double dtanh(double y) {
  return 1 - y * y;
}

void Tanh::init() {
  double step = 4.0 / TANH_N_SAMPLES;
  double y = 0;
  for (int i = 0; i < TANH_N_SAMPLES; i++) {
    tanhtab[(i << 1) + 1] = (1 << 24) * y + 0.5;
    //printf("%d\n", tanhtab[(i << 1) + 1]);
    // Use a basic 4th order Runge-Kutte to compute tanh from its
    // differential equation.
    double k1 = dtanh(y);
    double k2 = dtanh(y + 0.5 * step * k1);
    double k3 = dtanh(y + 0.5 * step * k2);
    double k4 = dtanh(y + step * k3);
    double dy = (step / 6) * (k1 + k4 + 2 * (k2 + k3));
    y += dy;
  }
  for (int i = 0; i < TANH_N_SAMPLES - 1; i++) {
    tanhtab[i << 1] = tanhtab[(i << 1) + 3] - tanhtab[(i << 1) + 1];
  }
  int32_t lasty = (1 << 24) * y + 0.5;
  tanhtab[(TANH_N_SAMPLES << 1) - 2] = lasty - tanhtab[(TANH_N_SAMPLES << 1) - 1];
}
