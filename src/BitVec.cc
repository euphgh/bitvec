#include "bitvec/core.h"
#include "bitvec/utils.h"

using namespace bv;

BitVec::BitVec(word_t value, width_t width) {
  data = stdvec(BASE_SIZE);
  // alloc space and assign
  *((word_t *)data.data()) = value;
  // set len
  len = width == 0 ? min_len(value) : width;

  // remove redundent arr by width
  data_resize();
}

bool BitVec::operator[](width_t pos) const {
  return (data[pos / ubits] >> pos % ubits) & 0x1;
}

#define BITVEC_COMPARE_TEMPLATE(ret)                                           \
  do {                                                                         \
    size_t max_size = data_size();                                             \
    size_t min_size = rhs.data_size();                                         \
    if (data_size() < rhs.data_size()) {                                       \
      max_size = rhs.data_size();                                              \
      min_size = data_size();                                                  \
    }                                                                          \
    for (size_t i = max_size; i > min_size; i--) {                             \
      auto l = 0;                                                              \
      auto r = 0;                                                              \
      if (i <= data_size()) {                                                  \
        l = data[i - 1];                                                       \
      }                                                                        \
      if (i <= rhs.data_size()) {                                              \
        r = rhs.data[i - 1];                                                   \
      }                                                                        \
      if (l != r) {                                                            \
        return (ret);                                                          \
      }                                                                        \
    }                                                                          \
    for (size_t i = min_size; i > 0; i--) {                                    \
      const auto l = data[i - 1];                                              \
      const auto r = rhs.data[i - 1];                                          \
      if (l != r) {                                                            \
        return (ret);                                                          \
      }                                                                        \
    }                                                                          \
    return false;                                                              \
  } while (0)

bool BitVec::operator!=(const BitVec &rhs) const {
  BITVEC_COMPARE_TEMPLATE(true);
}
bool BitVec::operator<(const BitVec &rhs) const {
  BITVEC_COMPARE_TEMPLATE(l < r);
}
bool BitVec::operator>(const BitVec &rhs) const {
  BITVEC_COMPARE_TEMPLATE(l > r);
}
bool BitVec::operator>=(const BitVec &rhs) const { return !(*this < rhs); }
bool BitVec::operator<=(const BitVec &rhs) const { return !(*this > rhs); }
bool BitVec::operator==(const BitVec &rhs) const { return !(*this != rhs); }

#define RIGHT_SHIFT(pos)                                                       \
  auto mem = stdvec(data_size());                                              \
  do {                                                                         \
    const auto nblk = (pos) / ubits;                                           \
    if (nblk < data_size()) {                                                  \
      memcpy(mem.data(), data.data() + nblk,                                   \
             (data_size() - nblk) * sizeof(unit_t));                           \
    }                                                                          \
    const auto offset = (pos) % ubits;                                         \
    if (offset) {                                                              \
      for (size_t i = 0; i < data_size() - 1; i++)                             \
        mem[i] = (mem[i] >> offset) | (mem[i + 1] << (ubits - offset));        \
      mem[data_size() - 1] = (mem[data_size() - 1] >> offset);                 \
    }                                                                          \
  } while (0)

[[nodiscard]] BitVec BitVec::operator>>(width_t pos) const {
  BV_INFO("call >> {} for bitvec {}", pos, to_string());
  RIGHT_SHIFT(pos);
  return {width(), mem};
}

[[nodiscard]] BitVec BitVec::operator<<(width_t pos) const {
  BV_INFO("call << {} for bitvec {}", pos, to_string());
  auto mem = stdvec(data_size());
  // block
  const auto nblk = pos / ubits;
  // set lsb 0
  memset(mem.data(), 0, nblk * sizeof(unit_t));
  // set msb value
  memcpy(mem.data() + nblk, data.data(), (data_size() - nblk) * sizeof(unit_t));

  // offset
  const auto offset = pos % ubits;
  if (offset) {
    for (size_t i = data_size() - 1; i > 0; i--) {
      BV_INFO("mem[{}] = (mem[{}]({:02x}) << {}) | (mem[{}]({:02x}) >> {})", i,
              i, mem[i], offset, i - 1, mem[i - 1], (ubits - offset));
      mem[i] = (mem[i] << offset) | (mem[i - 1] >> (ubits - offset));
    }
    mem[0] = mem[0] << offset;
  }
  return {width(), mem};
}

[[nodiscard]] BitVec BitVec::slice(width_t msb, width_t lsb) const {
  RIGHT_SHIFT(lsb);
  return {msb - lsb + 1, mem};
}