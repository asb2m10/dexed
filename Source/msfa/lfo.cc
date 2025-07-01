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

#include "synth.h"

#include "sin.h"
#include "lfo.h"

// source table
static double lfoSource[] = {
    0.062541, 0.125031, 0.312393, 0.437120, 0.624610,
    0.750694, 0.936330, 1.125302, 1.249609, 1.436782,
    1.560915, 1.752081, 1.875117, 2.062494, 2.247191,
    2.374451, 2.560492, 2.686728, 2.873976, 2.998950,
    3.188013, 3.369840, 3.500175, 3.682224, 3.812065,
    4.000800, 4.186202, 4.310716, 4.501260, 4.623209,
    4.814636, 4.930480, 5.121901, 5.315191, 5.434783,
    5.617346, 5.750431, 5.946717, 6.062811, 6.248438,
    6.431695, 6.564264, 6.749460, 6.868132, 7.052186,
    7.250580, 7.375719, 7.556294, 7.687577, 7.877738,
    7.993605, 8.181967, 8.372405, 8.504848, 8.685079,
    8.810573, 8.986341, 9.122423, 9.300595, 9.500285,
    9.607994, 9.798158, 9.950249, 10.117361, 11.251125,
    11.384335, 12.562814, 13.676149, 13.904338, 15.092062,
    16.366612, 16.638935, 17.869907, 19.193858, 19.425019,
    20.833333, 21.034918, 22.502250, 24.003841, 24.260068,
    25.746653, 27.173913, 27.578599, 29.052876, 30.693677,
    31.191516, 32.658393, 34.317090, 34.674064, 36.416606,
    38.197097, 38.550501, 40.387722, 40.749796, 42.625746,
    44.326241, 44.883303, 46.772685, 48.590865, 49.261084
};


uint32_t Lfo::unit_;
uint32_t Lfo::lforatio_;

void Lfo::init(double sample_rate) {
    // constant is 1 << 32 / 15.5s / 11
    Lfo::unit_ = (int32_t)(N * 25190424 / sample_rate + 0.5);

    double ratio = 4437500000.0 * N;
    Lfo::lforatio_ = ratio / sample_rate;
}

void Lfo::reset(const uint8_t params[6]) {
    int rate = params[0];  // 0..99
    delta_ = lfoSource[rate] * lforatio_;
    int a = 99 - params[1];  // LFO delay
    if (a == 99) {
        delayinc_ = ~0u;
        delayinc2_ = ~0u;
    } else {
        a = (16 + (a & 15)) << (1 + (a >> 4));
        delayinc_ = unit_ * a;
        a &= 0xff80;
        a = max(0x80, a);
        delayinc2_ = unit_ * a;
    }
    waveform_ = params[5];
    sync_ = params[4] != 0;
}

int32_t Lfo::getsample() {
    phase_ += delta_;
    int32_t x;
    switch (waveform_) {
        case 0:  // triangle
            x = phase_ >> 7;
            x ^= -(phase_ >> 31);
            x &= (1 << 24) - 1;
            return x;
        case 1:  // sawtooth down
            return (~phase_ ^ (1U << 31)) >> 8;
        case 2:  // sawtooth up
            return (phase_ ^ (1U << 31)) >> 8;
        case 3:  // square
            return ((~phase_) >> 7) & (1 << 24);
        case 4:  // sine
            return (1 << 23) + (Sin::lookup(phase_ >> 8) >> 1);
        case 5:  // s&h
            if (phase_ < delta_) {
                randstate_ = (randstate_ * 179 + 17) & 0xff;
            }
            x = randstate_ ^ 0x80;
            return (x + 1) << 16;
    }
    return 1 << 23;
}

int32_t Lfo::getdelay() {
    uint32_t delta = delaystate_ < (1U << 31) ? delayinc_ : delayinc2_;
    uint64_t d = ((uint64_t)delaystate_) + delta;
    if (d > ~0u) {
        return 1 << 24;
    }
    delaystate_ = d;
    if (d < (1U << 31)) {
        return 0;
    } else {
        return (d >> 7) & ((1 << 24) - 1);
    }
}

void Lfo::keydown() {
    if (sync_) {
        phase_ = (1U << 31) - 1;
    }
    delaystate_ = 0;
}
