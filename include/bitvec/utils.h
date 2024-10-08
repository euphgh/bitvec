#ifndef __BITVEC_UTILS_H__
#define __BITVEC_UTILS_H__

#include "bitvec/debug.h"
#include "bitvec/type.h"
#include <random>
#include <sstream>
#include <stack>

namespace bv {

inline char hex_char(size_t data) {
  if (data < 10) {
    auto res = (data & 0xf) + '0';
    return *(char *)&res;
  }
  if (data < 16) {
    auto res = ((data - 10) & 0xf) + 'a';
    return *(char *)&res;
  }
  require(false, "call hex_char with {}", data);
}

#define bin_char(bit) ((char)(((bit) & 0x1) + '0'))

using mark_t = enum { hex, bin, otx };

inline std::string byte_string(uint8_t data, mark_t mark) {
  std::string res;
  switch (mark) {
  case hex:
    return fmt::format("{:02x}", data);
  case bin:
    return fmt::format("{:08b}", data);
  default:
    require(false, "not expect exec to here");
  }
}

// from 0 to len - 1
inline std::string bits_string(uint8_t data, width_t len, mark_t mark) {
  auto res = std::stringstream();
  if (mark == hex) {
    auto buf = std::stack<uint8_t>();
    for (size_t i = 0; i < len / 4; i++) {
      buf.push((data & 0xf));
      data >>= 4;
    }
    if (len % 4) {
      buf.push(data);
      res << "(";
      for (size_t i = len % 4; i < 4; i++) {
        res << "x";
      }
      for (size_t i = len % 4; i > 0; i--) {
        res << (char)(((data >> (i - 1)) & 0x1) + '0');
      }
      res << ")";
    }

    while (!buf.empty()) {
      res << hex_char(buf.top());
      buf.pop();
    }
    return res.str();
  }
  if (mark == bin) {
    for (size_t i = len - 1; i >= 0; i--) {
      res << bin_char(data >> i);
    }
    return res.str();
  }
  require(false, "fail to bits_string with not support format");
}

// count zero before first one
template <typename data_t> size_t count_lead_zero(data_t x) {
  size_t res = 0;
  while (x < HALF_OF(data_t)) {
    res++;
    x <<= 1;
  }
  return res;
}

// find the minimal len for a value
inline size_t min_len(word_t x) {
  auto *arr = (uint8_t *)&x;
  auto res = sizeof(word_t) * 8;
  for (auto i = (sizeof(x) / sizeof(uint8_t)); i > 0; i--) {
    auto byte = arr[i - 1];
    if (byte) {
      res -= count_lead_zero<uint8_t>(byte);
      break;
    }
    res -= sizeof(uint8_t) * 8;
  }
  return res == 0 ? 1 : res;
}

inline size_t rand_unit() {
  // static std::random_device rd;
  static std::default_random_engine rd;
  static std::uniform_int_distribution<size_t> engin(0, ~(unit_t(0)));
  return engin(rd);
}

/** Gnerate single bit mask
 * @param pos which pos should be set 1
 * @return one bit set mask
 */
inline size_t one_bit_mask(width_t pos) { return (1ULL << pos); }

/** Generate low bit mask
 * @param num from 0, how many 1 in result
 * @return mask with num 1
 */
inline size_t low_bit_mask(width_t num) { return (1ULL << num) - 1; }

}; // namespace bv

#endif // __BITVEC_UTILS_H__