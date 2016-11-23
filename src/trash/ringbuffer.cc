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

#include <time.h>
#include <string.h>

#include "synth.h"
#include "ringbuffer.h"

RingBuffer::RingBuffer() {
  rd_ix_ = 0;
  wr_ix_ = 0;
}

int RingBuffer::BytesAvailable() {
  return (wr_ix_ - rd_ix_) & (kBufSize - 1);
}

int RingBuffer::WriteBytesAvailable() {
  return (rd_ix_ - wr_ix_ - 1) & (kBufSize - 1);
}

int RingBuffer::Read(int size, uint8_t *bytes) {
  int rd_ix = rd_ix_;
  SynthMemoryBarrier();  // read barrier, make sure data is committed before ix
  int fragment_size = min(size, kBufSize - rd_ix);
  memcpy(bytes, buf_ + rd_ix, fragment_size);
  if (size > fragment_size) {
    memcpy(bytes + fragment_size, buf_, size - fragment_size);
  }
  SynthMemoryBarrier();  // full barrier, make sure read commits before updating
  rd_ix_ = (rd_ix + size) & (kBufSize - 1);
  return size;
}

int RingBuffer::Write(const uint8_t *bytes, int size) {
  int remaining = size;
  while (remaining > 0) {
    int rd_ix = rd_ix_;
    int wr_ix = wr_ix_;
    int space_available = (rd_ix - wr_ix - 1) & (kBufSize - 1);
    if (space_available == 0) {
      struct timespec sleepTime;
      sleepTime.tv_sec = 0;
      sleepTime.tv_nsec = 1000000;
      nanosleep(&sleepTime, NULL);
    } else {
      int wr_size = min(remaining, space_available);
      int fragment_size = min(wr_size, kBufSize - wr_ix);
      memcpy(buf_ + wr_ix, bytes, fragment_size);
      if (wr_size > fragment_size) {
        memcpy(buf_, bytes + fragment_size, wr_size - fragment_size);
      }
      SynthMemoryBarrier();  // write barrier, make sure data commits
      wr_ix_ = (wr_ix + wr_size) & (kBufSize - 1);
      remaining -= wr_size;
      bytes += wr_size;
    }
  }
  // JJK : defined as returning int
  return 0;
}

