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

#ifndef __SYNTH_H
#define __SYNTH_H

// This IS not be present on MSVC.
// See http://stackoverflow.com/questions/126279/c99-stdint-h-header-and-ms-visual-studio
#include <stdint.h>
#ifdef _MSC_VER
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int16 SInt16;
#endif

const static int LG_N = 6;
const static int N = (1 << LG_N);

#if defined(__APPLE__)
#include <libkern/OSAtomic.h>
#define SynthMemoryBarrier() OSMemoryBarrier()
#elif defined(__GNUC__)
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 1)
#define SynthMemoryBarrier() __sync_synchronize()
#endif
#endif


// #undef SynthMemoryBarrier()

#ifndef SynthMemoryBarrier
// need to understand why this must be defined
// #warning Memory barrier is not enabled
#define SynthMemoryBarrier()
#endif

template<typename T>
inline static T min(const T& a, const T& b) {
    return a < b ? a : b;
}

template<typename T>
inline static T max(const T& a, const T& b) {
    return a > b ? a : b;
}


void dexed_trace(const char *source, const char *fmt, ...);

#define QER(n,b) ( ((float)n)/(1<<b) )

#ifdef _MSC_VER
#define TRACE(fmt, ...) dexed_trace(__FUNCTION__,fmt,##__VA_ARGS__)
#else
#define TRACE(fmt, ...) dexed_trace(__PRETTY_FUNCTION__,fmt,##__VA_ARGS__)
#endif


#endif  // __SYNTH_H
