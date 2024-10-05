#ifndef __BITVEC_TEST_UTILS_H__
#define __BITVEC_TEST_UTILS_H__

#include "bitvec/core.h"
#include <bitset>

size_t uniform_rand(size_t min = 0, size_t max = SIZE_MAX);

template <size_t width>
bool bv_cmp_bs(const bv::BitVec &bv, const std::bitset<width> &bs) {
  const auto max_width = std::max(bv.width(), width);
  const auto min_width = std::min(bv.width(), width);
  for (size_t i = 0; i < min_width; i++) {
    if (bv[i] == bs[i]) {
      fmt::println("BitVec[{}]: {} != bitset[{}]: {}", i, bv[i], i, bs[i]);
      return false;
    }
  }
  for (size_t i = min_width; i < max_width; i++) {
    const auto v = i < bv.width() ? bv[i] : false;
    const auto s = i < width ? bs[i] : false;
    if (v != s) {
      fmt::println("BitVec[{}]: {} != bitset[{}]: {}", i, bv[i], i, bs[i]);
      return false;
    }
  }
  return true;
}

#endif // __BITVEC_TEST_UTILS_H__