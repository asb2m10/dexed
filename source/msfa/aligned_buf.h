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

// A convenient wrapper for buffers with alignment constraints

// Note that if we were on C++11, we'd use aligned_storage or somesuch.

#ifndef __ALIGNED_BUF_H
#define __ALIGNED_BUF_H

#include<stddef.h>

template<typename T, size_t size, size_t alignment = 16>
class AlignedBuf {
 public:
  T *get() {
    return (T *)((((intptr_t)storage_) + alignment - 1) & -alignment);
  }
 private:
  unsigned char storage_[size * sizeof(T) + alignment];
};

#endif  // __ALIGNED_BUF_H
