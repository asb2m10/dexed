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

#ifndef SYNTH_RINGBUFFER_H_
#define SYNTH_RINGBUFFER_H_

class RingBuffer {
 public:
  RingBuffer();

  // Returns number of bytes available for reading.
  int BytesAvailable();

  // Returns number of bytes that can be written without blocking.
  int WriteBytesAvailable();

  // Reads bytes. It is the caller's responsibility to make sure that
  // size <= a previous value of BytesAvailable().
  int Read(int size, uint8_t *bytes);

  // Writes bytes into the buffer. If the buffer is full, the method will
  // block until space is available.
  int Write(const uint8_t *bytes, int size);
 private:
  static const int kBufSize = 8192;
  uint8_t buf_[kBufSize];
  volatile unsigned int rd_ix_;
  volatile unsigned int wr_ix_;
};

#endif  // SYNTH_RINGBUFFER_H_
