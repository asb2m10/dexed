/*
 * Copyright (C) 2014  Pascal Gauthier.
 * Copyright (C) 2012  Steffen Ohrendorf <steffen.ohrendorf@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 *
 * Original Java Code: Copyright (C) 2008 Robson Cozendey <robson@cozendey.com>
 *
 * Some code based on forum posts in: http://forums.submarine.org.uk/phpBB/viewforum.php?f=9,
 * Copyright (C) 2010-2013 by carbon14 and opl3
 *
 */

#include "EngineOpl.h"

#ifdef _WIN32
    __declspec(align(16)) const int zeros[N] = {0};
#else
    const int32_t __attribute__ ((aligned(16))) zeros[N] = {0};
#endif

uint16_t SignBit = 0x8000;

uint16_t sinLogTable[256] = {
    2137, 1731, 1543, 1419, 1326, 1252, 1190, 1137, 1091, 1050, 1013, 979, 949, 920, 894, 869,
    846, 825, 804, 785, 767, 749, 732, 717, 701, 687, 672, 659, 646, 633, 621, 609,
    598, 587, 576, 566, 556, 546, 536, 527, 518, 509, 501, 492, 484, 476, 468, 461,
    453, 446, 439, 432, 425, 418, 411, 405, 399, 392, 386, 380, 375, 369, 363, 358,
    352, 347, 341, 336, 331, 326, 321, 316, 311, 307, 302, 297, 293, 289, 284, 280,
    276, 271, 267, 263, 259, 255, 251, 248, 244, 240, 236, 233, 229, 226, 222, 219,
    215, 212, 209, 205, 202, 199, 196, 193, 190, 187, 184, 181, 178, 175, 172, 169,
    167, 164, 161, 159, 156, 153, 151, 148, 146, 143, 141, 138, 136, 134, 131, 129,
    127, 125, 122, 120, 118, 116, 114, 112, 110, 108, 106, 104, 102, 100, 98, 96,
    94, 92, 91, 89, 87, 85, 83, 82, 80, 78, 77, 75, 74, 72, 70, 69,
    67, 66, 64, 63, 62, 60, 59, 57, 56, 55, 53, 52, 51, 49, 48, 47,
    46, 45, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30,
    29, 28, 27, 26, 25, 24, 23, 23, 22, 21, 20, 20, 19, 18, 17, 17,
    16, 15, 15, 14, 13, 13, 12, 12, 11, 10, 10, 9, 9, 8, 8, 7,
    7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2,
    2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0
};

uint16_t sinExpTable[256] = {
    0, 3, 6, 8, 11, 14, 17, 20, 22, 25, 28, 31, 34, 37, 40, 42,
    45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90,
    93, 96, 99, 102, 105, 108, 111, 114, 117, 120, 123, 126, 130, 133, 136, 139,
    142, 145, 148, 152, 155, 158, 161, 164, 168, 171, 174, 177, 181, 184, 187, 190,
    194, 197, 200, 204, 207, 210, 214, 217, 220, 224, 227, 231, 234, 237, 241, 244,
    248, 251, 255, 258, 262, 265, 268, 272, 276, 279, 283, 286, 290, 293, 297, 300,
    304, 308, 311, 315, 318, 322, 326, 329, 333, 337, 340, 344, 348, 352, 355, 359,
    363, 367, 370, 374, 378, 382, 385, 389, 393, 397, 401, 405, 409, 412, 416, 420,
    424, 428, 432, 436, 440, 444, 448, 452, 456, 460, 464, 468, 472, 476, 480, 484,
    488, 492, 496, 501, 505, 509, 513, 517, 521, 526, 530, 534, 538, 542, 547, 551,
    555, 560, 564, 568, 572, 577, 581, 585, 590, 594, 599, 603, 607, 612, 616, 621,
    625, 630, 634, 639, 643, 648, 652, 657, 661, 666, 670, 675, 680, 684, 689, 693,
    698, 703, 708, 712, 717, 722, 726, 731, 736, 741, 745, 750, 755, 760, 765, 770,
    774, 779, 784, 789, 794, 799, 804, 809, 814, 819, 824, 829, 834, 839, 844, 849,
    854, 859, 864, 869, 874, 880, 885, 890, 895, 900, 906, 911, 916, 921, 927, 932,
    937, 942, 948, 953, 959, 964, 969, 975, 980, 986, 991, 996, 1002, 1007, 1013, 1018
};

inline uint16_t sinLog(uint16_t phi) {
    const uint8_t index = (phi & 0xff);

    switch( ( phi & 0x0300 ) ) {
        case 0x0000:
            // rising quarter wave  Shape A
            return sinLogTable[index];
        case 0x0100:
            // falling quarter wave  Shape B
            return sinLogTable[index ^ 0xFF];
        case 0x0200:
            // rising quarter wave -ve  Shape C
            return sinLogTable[index] | SignBit;
        default:
            // falling quarter wave -ve  Shape D
            return sinLogTable[index ^ 0xFF] | SignBit;
    }
}

// 16 env units are ~3dB and halve the output
/**
 * @brief OPL Sine Wave calculation
 * @param[in] phase Wave phase (0..1023)
 * @param[in] env Envelope value (0..511)
 * @warning @a env will not be checked for correct values.
 */
inline int16_t oplSin( uint16_t phase, uint16_t env ) {
    uint16_t expVal = sinLog(phase) + (env << 3);
    const bool isSigned = expVal & SignBit;

    expVal &= ~SignBit;
    // expVal: 0..2137+511*8 = 0..6225
    // result: 0..1018+1024
    uint32_t result = 0x0400 + sinExpTable[( expVal & 0xff ) ^ 0xFF];
    result <<= 1;
    result >>= ( expVal >> 8 ); // exp

    if( isSigned ) {
        // -1 for one's complement
        return -result - 1;
    } else {
        return result;
    }    
}

void EngineOpl::compute(int32_t *output, const int32_t *input, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
    const int32_t *adder = add ? output : zeros;

    for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t y = oplSin((phase+input[i]) >> 14, gain);
        output[i] = (y << 14) + adder[i];
        phase += freq;
    }

}
void EngineOpl::compute_pure(int32_t *output, int32_t phase0, int32_t freq, int32_t gain1, int32_t gain2, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
    const int32_t *adder = add ? output : zeros;

    for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t y = oplSin(phase >> 14, gain);
        output[i] = (y << 14) + adder[i];
        phase += freq;
    }
}

void EngineOpl::compute_fb(int32_t *output, int32_t phase0, int32_t freq,
                                    int32_t gain1, int32_t gain2,
                              int32_t *fb_buf, int fb_shift, bool add) {
    int32_t dgain = (gain2 - gain1 + (N >> 1)) >> LG_N;
    int32_t gain = gain1;
    int32_t phase = phase0;
    const int32_t *adder = add ? output : zeros;
    int32_t y0 = fb_buf[0];
    int32_t y = fb_buf[1];
    
    for (int i = 0; i < N; i++) {
        gain += dgain;
        int32_t scaled_fb = (y0 + y) >> (fb_shift + 1);
        y0 = y;
        y = oplSin((phase+scaled_fb) >> 14, gain) << 14;
        output[i] = y + adder[i];
        phase += freq;
    }
    
    fb_buf[0] = y0;
    fb_buf[1] = y;
}


void EngineOpl::render(int32_t *output, FmOpParams *params, int algorithm,
                        int32_t *fb_buf, int32_t feedback_shift) {
    const int kLevelThresh = 507;  // really ????
    const FmAlgorithm alg = algorithms[algorithm];
    bool has_contents[3] = { true, false, false };
    for (int op = 0; op < 6; op++) {
        int flags = alg.ops[op];
        bool add = (flags & OUT_BUS_ADD) != 0;
        FmOpParams &param = params[op];
        int inbus = (flags >> 4) & 3;
        int outbus = flags & 3;
        int32_t *outptr = (outbus == 0) ? output : buf_[outbus - 1].get();
        int32_t gain1 = param.gain_out == 0 ? 511 : param.gain_out;
        int32_t gain2 = 512-(param.level_in >> 19);
        param.gain_out = gain2;
        
        if (gain1 <= kLevelThresh || gain2 <= kLevelThresh) {
            if (!has_contents[outbus]) {
                add = false;
            }
            if (inbus == 0 || !has_contents[inbus]) {
                // todo: more than one op in a feedback loop
                if ((flags & 0xc0) == 0xc0 && feedback_shift < 16) {
                    // cout << op << " fb " << inbus << outbus << add << endl;
                    compute_fb(outptr, param.phase, param.freq,
                                           gain1, gain2,
                                           fb_buf, feedback_shift, add);
                } else {
                    // cout << op << " pure " << inbus << outbus << add << endl;
                    compute_pure(outptr, param.phase, param.freq,
                                             gain1, gain2, add);
                }
            } else {
                // cout << op << " normal " << inbus << outbus << " " << param.freq << add << endl;
                compute(outptr, buf_[inbus - 1].get(),
                                    param.phase, param.freq, gain1, gain2, add);
            }
            has_contents[outbus] = true;
        } else if (!add) {
            has_contents[outbus] = false;
        }
        param.phase += param.freq << LG_N;
    }
}












