/*
 * Copyright 2019 Jean Pierre Cimalando.
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
#include "porta.h"
#include "synth.h"

void Porta::init_sr(double sampleRate)
{
    // compute portamento for CC 7-bit range

    for (unsigned int i = 0; i < 128; ++i) {
        // number of semitones travelled
        double sps = 350.0 * pow(2.0, -0.062 * i);  // per second
        double spf = sps / sampleRate;              // per frame
        double spp = spf * N;                       // per period
        const int step = (1 << 24) / 12;
        rates[i] = (int32_t)(0.5f + step * spp);    // to pitch units
    }
}

int32_t Porta::rates[128];
