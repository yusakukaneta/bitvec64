/**
 *
 * Copyright (c) 2011, Yusaku Kaneta
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in the 
 *       documentation and/or other materials provided with the distribution.
 * 
 *     * Neither the name of the author nor the names of its contributors 
 *       may be used to endorse or promote products derived from this 
 *       software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BITVEC_HPP
#define BITVEC_HPP

#include <iostream>
#include <stdint.h>


class Bitvec {

public:
  static const std::size_t W = 64;
  static const std::size_t SHIFT = 6;
  static const uint64_t MASK  = 0x000000000000003FULL;

  Bitvec(std::size_t nbits = 0);
  ~Bitvec();

  std::size_t size() const;
  std::string to_string() const;
  void print(std::ostream &os = std::cout) const;

  uint64_t get_bit(std::size_t bit_pos) const;
  void set_bit(std::size_t bit_pos);
  void unset_bit(std::size_t bit_pos);
  void clear();

  Bitvec& operator<<=(std::size_t wid);
  Bitvec& operator>>=(std::size_t wid);
  Bitvec& operator~();
  Bitvec& operator&=(const Bitvec &bitvec);
  Bitvec& operator|=(const Bitvec &bitvec);
  Bitvec& operator^=(const Bitvec &bitvec);
  Bitvec& operator+=(const Bitvec &bitvec);
  Bitvec& operator-=(const Bitvec &bitvec);

private:
  std::size_t block_size() const;

  uint64_t get_block(std::size_t block_pos) const;
  void set_block(std::size_t block_pos, uint64_t value);
  void unset_block(std::size_t block_pos);

  void inplace_lshift(std::size_t wid);
  void inplace_rshift(std::size_t wid);
  inline void inplace_not();
  inline void inplace_and(const Bitvec &bitvec);
  inline void inplace_or(const Bitvec &bitvec);
  inline void inplace_xor(const Bitvec &bitvec);
  inline void inplace_add(const Bitvec &bitvec);
  inline void inplace_sub(const Bitvec &bitvec);

private:
  const std::size_t nbits_;
  const std::size_t nblocks_;
  uint64_t *blocks_;

};

#endif
