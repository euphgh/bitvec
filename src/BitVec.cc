#include "bitvec/core.h"
#include <algorithm>

template <typename data_t> size_t first_one_pos(data_t x) {
  size_t res = sizeof(data_t) * 8;
  while (x < HALF_OF(data_t)) {
    res--;
    x <<= 1;
  }
  return res;
}

// find the minimal len for a value
size_t min_len(word_t x) {
  auto *arr = (uint8_t *)&x;
  size_t res = sizeof(word_t) * 8;
  for (auto i = (sizeof(x) / sizeof(uint8_t)); i > 0; i--) {
    auto byte = arr[i - 1];
    if (byte) {
      res -= first_one_pos<uint8_t>(byte);
      break;
    }
    res -= sizeof(uint8_t) * 8;
  }
  return res == 0 ? 1 : res;
}

BitVec BitVec::create(word_t value, width_t width) {
  auto data = stdvec(BASE_SIZE);
  // alloc space and assign
  *((word_t *)data.data()) = value;
  // set len
  width_t len = width == 0 ? min_len(value) : width;

  BitVec ret = BitVec(len, data);
  // remove redundent arr by width
  ret.data_resize();
  return ret;
}

bool BitVec::operator[](width_t pos) const {
  return (data[pos / sizeof(unit_t)] >> pos % sizeof(unit_t)) & 0x1;
}

#define COMP_IMPL(op)                                                          \
  bool BitVec::operator op(const BitVec & rhs) const {                         \
    auto min_size = std::min(data_size(), rhs.data_size());                    \
    return memcmp(data.data(), rhs.data.data(), min_size * sizeof(unit_t))     \
        op 0;                                                                  \
  }

#define FOREACH_OP(_) _(==) _(!=) _(>) _(<) _(>=) _(<=)
FOREACH_OP(COMP_IMPL)
#undef FOREACH_OP
#undef COMP_IMPL

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
  return { msb - lsb + 1, mem };
}