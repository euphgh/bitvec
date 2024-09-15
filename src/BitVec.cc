#include "bitvec/core.h"
#include "bitvec/utils.h"
#include <algorithm>
#include <vector>

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

#define COMP_IMPL(op)                                                          \
  bool BitVec::operator op(const BitVec & rhs) const {                         \
    auto min_size = std::min(data_size(), rhs.data_size());                    \
    return memcmp(data.data(), rhs.data.data(), min_size * sizeof(unit_t))     \
        op 0;                                                                  \
  }

#define FOREACH_OP(_) _(==) _(!=) _(>) _(<) _(>=) _(<=)
bool BitVec ::operator==(const BitVec &rhs) const {
  auto min_size = std ::min(data_size(), rhs.data_size());
  return memcmp(data.data(), rhs.data.data(), min_size * sizeof(unit_t)) == 0;
}
bool BitVec ::operator!=(const BitVec &rhs) const {
  auto min_size = std ::min(data_size(), rhs.data_size());
  return memcmp(data.data(), rhs.data.data(), min_size * sizeof(unit_t)) != 0;
}
bool BitVec ::operator>(const BitVec &rhs) const {
  auto min_size = std ::min(data_size(), rhs.data_size());
  return memcmp(data.data(), rhs.data.data(), min_size * sizeof(unit_t)) > 0;
}
bool BitVec ::operator<(const BitVec &rhs) const {
  BV_INFO("call bitvec {} < {}", to_string(), rhs.to_string());
  BV_INFO("lhs.data_size = {}, rhs.data_size = {}", data_size(),
          rhs.data_size());
  BV_INFO("lhs.check_data = {}, rhs.check_data = {}", data_check(),
          rhs.data_check());
  BV_INFO("lhs.width = {}, rhs.width = {}", width(), rhs.width());
  auto min_size = std ::min(data_size(), rhs.data_size());
  // from msb to lsb
  if (rhs.data_size() > min_size) {
    for (size_t i = rhs.data_size(); i > min_size; i--) {
      if (rhs.data[i - 1] != 0)
        return true;
    }
  } else if (data_size() > min_size) {
    for (size_t i = data_size(); i > min_size; i--) {
      if (data[i - 1] != 0)
        return false;
    }
  } else {
    for (size_t i = min_size; i > 0; i--) {
      if (data[i - 1] != rhs.data[i - 1]) {
        return data[i - 1] < rhs.data[i - 1];
      }
    }
  }
  return false;
}
bool BitVec ::operator>=(const BitVec &rhs) const {
  auto min_size = std ::min(data_size(), rhs.data_size());
  return memcmp(data.data(), rhs.data.data(), min_size * sizeof(unit_t)) >= 0;
}
bool BitVec ::operator<=(const BitVec &rhs) const {
  auto min_size = std ::min(data_size(), rhs.data_size());
  return memcmp(data.data(), rhs.data.data(), min_size * sizeof(unit_t)) <= 0;
}
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
  return {msb - lsb + 1, mem};
}