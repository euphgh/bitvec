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
  return (data[pos / sizeof(unit_t)] >> pos % sizeof(unit_t)) & 0x1;
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

#define RIGHT_SHIFT(name, op, pos)                                             \
  auto name = stdvec(data_size());                                             \
  do {                                                                         \
    if (pos % ubits) {                                                         \
      for (size_t i = 0; i < data_size(); i++)                                 \
        name[i] = data[i] op(pos % ubits);                                     \
    }                                                                          \
    size_t offset = pos / ubits;                                               \
    if (offset) {                                                              \
      memcpy(name.data(), data.data() + offset,                                \
             (data_size() - offset) * sizeof(unit_t));                         \
    }                                                                          \
  } while (0);

[[nodiscard]] BitVec BitVec::operator>>(width_t pos) const {
  RIGHT_SHIFT(mem, >>, pos);
  return {width(), mem};
}

[[nodiscard]] BitVec BitVec::operator<<(width_t pos) const {
  auto mem = stdvec(data_size());
  // offset
  if (pos % ubits) {
    for (size_t i = 0; i < data_size(); i++)
      mem[i] = data[i] << (pos % ubits);
  }
  // block
  size_t offset = pos / ubits;
  if (offset) {
    // set lsb 0
    memset(mem.data(), 0, offset * sizeof(unit_t));
    // set msb value
    memcpy(mem.data() + offset, data.data(),
           (data_size() - offset) * sizeof(unit_t));
  }
  return {width(), mem};
}

[[nodiscard]] BitVec BitVec::slice(width_t msb, width_t lsb) const {
  RIGHT_SHIFT(mem, >>, lsb);
  return {msb - lsb + 1, mem};
}