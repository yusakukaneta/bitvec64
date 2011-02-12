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

#include <algorithm>
#include <string>
#include <cstdlib>
#include "bitvec.hpp"


Bitvec::Bitvec(std::size_t nbits): nbits_(nbits), nblocks_(1 + nbits/W), blocks_(0) {
  blocks_ = new uint64_t[nblocks_];
  clear();
}


Bitvec::~Bitvec() {
  delete [] blocks_;
}


std::size_t Bitvec::size() const {
  return nbits_;
}


void Bitvec::print(std::ostream &os) const { // to be fixed
  os << to_string() << std::endl;
}


std::string Bitvec::to_string() const {
  std::string res;
  for (std::size_t i = 0; i < nbits_; ++i) {
    char ch = (get_bit(i) == 1) ? '1' : '0';
    res.push_back(ch);
  }
  std::reverse(res.begin(), res.end());
  return res;
}


uint64_t Bitvec::get_bit(std::size_t bit_pos) const {
  return (blocks_[bit_pos>>SHIFT] >> (bit_pos & MASK)) & 1ULL;
}


void Bitvec::set_bit(std::size_t bit_pos) { 
  blocks_[bit_pos>>SHIFT] |= (1ULL << (bit_pos & MASK)); 
}


void Bitvec::unset_bit(std::size_t bit_pos) { 
  blocks_[bit_pos>>SHIFT] &= ~(1ULL << (bit_pos & MASK)); 
}


void Bitvec::clear() {
  for (std::size_t i = 0; i < nblocks_; ++i) {
    blocks_[i] = 0ULL;
  }
}


Bitvec& Bitvec::operator<<=(std::size_t wid) {
  inplace_lshift(wid);
  return *this;
};


Bitvec& Bitvec::operator>>=(std::size_t wid) {
  inplace_rshift(wid);
  return *this;
};


Bitvec& Bitvec::operator~() {
  inplace_not();
  return *this;
};


Bitvec& Bitvec::operator&=(const Bitvec &bitvec) {
  inplace_and(bitvec);
  return *this;
};


Bitvec& Bitvec::operator|=(const Bitvec &bitvec) {
  inplace_or(bitvec);
  return *this;
};


Bitvec& Bitvec::operator^=(const Bitvec &bitvec) {
  inplace_xor(bitvec);
  return *this;
};


Bitvec& Bitvec::operator+=(const Bitvec &bitvec) {
  inplace_add(bitvec);
  return *this;
};


Bitvec& Bitvec::operator-=(const Bitvec &bitvec) {
  inplace_sub(bitvec);
  return *this;
};


std::size_t Bitvec::block_size() const {
  return nblocks_;
}


uint64_t Bitvec::get_block(std::size_t block_pos) const {
  return blocks_[block_pos];
}


void Bitvec::set_block(std::size_t block_pos, uint64_t value) {
  blocks_[block_pos] = value;
}


void Bitvec::unset_block(std::size_t block_pos) {
  blocks_[block_pos] = 0ULL;
}


void Bitvec::inplace_lshift(std::size_t wid) {
  int width = wid >> SHIFT;
  int offset = wid & MASK;
  for (int i = nblocks_ - 1; i >= 0; --i) {
    uint64_t new_block1 = (i-width >= 0) ? (blocks_[i-width] << offset) : 0ULL;
    uint64_t new_block2 = (i-width-1 >= 0) ? (blocks_[i-width-1] >> (W - offset)) : 0ULL;
    blocks_[i] = new_block1 | new_block2;
  }      
}


void Bitvec::inplace_rshift(std::size_t wid) {
  int width = wid >> SHIFT;
  int offset = wid & MASK;
  for (int i = 0; i < nblocks_; ++i) {
    uint64_t new_block1 = (i+width+1 < nblocks_) ? blocks_[i+width+1] << (W - offset) : 0ULL;
    uint64_t new_block2 = (i+width < nblocks_) ? blocks_[i+width] >> offset : 0ULL;
    blocks_[i] = new_block1 | new_block2;
  }
}

void Bitvec::inplace_not() {
  for (std::size_t i = 0; i < nblocks_; ++i) {
    blocks_[i] = ~blocks_[i];
  }
}


void Bitvec::inplace_and(const Bitvec &bitvec) {
  for (std::size_t i = 0; i < nblocks_; ++i) {
    blocks_[i] &= bitvec.blocks_[i];
  }
}


void Bitvec::inplace_or(const Bitvec &bitvec) {
  for (std::size_t i = 0; i < nblocks_; ++i) {
    blocks_[i] |= bitvec.blocks_[i];
  }
}

void Bitvec::inplace_xor(const Bitvec &bitvec) {
  for (std::size_t i = 0; i < nblocks_; ++i) {
    blocks_[i] ^= bitvec.blocks_[i];
  }
}


void Bitvec::inplace_add(const Bitvec &bitvec) {
  int carry = 0;
  for (std::size_t i = 0; i < nblocks_; ++i) {
    uint64_t new_block = blocks_[i] + bitvec.blocks_[i] + carry;
    carry = ((new_block < blocks_[i]) || 
             (bitvec.blocks_[i] == 0xFFFFFFFFFFFFFFFFULL)) ? 1 : 0;
    blocks_[i] = new_block;
  }
}


void Bitvec::inplace_sub(const Bitvec &bitvec) {
  int carry = 0;
  for (std::size_t i = 0; i < nblocks_; ++i) {
    uint64_t new_block = blocks_[i] - bitvec.blocks_[i] - carry;
    carry = ((new_block > blocks_[i]) || 
             ((bitvec.blocks_[i] == 0xFFFFFFFFFFFFFFFFULL) && (carry == 1))) ? 1 : 0;
    blocks_[i] = new_block;
  }
}
