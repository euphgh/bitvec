#include "bitvec/core.h"
#include "bitvec/debug.h"
#include "fmt/core.h"

#define EXTEND_IMPL(start, bit)                                                \
  do {                                                                         \
    require(start < width(), "extend start should less than width");           \
    auto uidx = start / ubits;                                                 \
    auto uoffset = start % ubits;                                              \
    /* set unit high bits */                                                   \
    data[uidx] <<= (ubits - uoffset - 1);                                      \
    data[uidx] = (signed)(data[uidx]) >> (ubits - uoffset - 1);                \
    /* set high elements */                                                    \
    memset(data.data() + uidx + 1, bit,                                        \
           sizeof(unit_t) * (data_size() - uidx - 1));                         \
  } while (0)

// changable, assert start < width()
void BitVec::sign_extend(width_t start) { EXTEND_IMPL(start, (*this)[start]); }
void BitVec::zero_extend(width_t start) { EXTEND_IMPL(start, 0); }

void BitVec::resize(width_t width) {

  require(false, "hello");
  if (width == len)
    return;

  auto old_size = data_size();
  len = width;
  data_resize();
  auto new_size = data_size();

  // clear appended high elements
  if (old_size < new_size)
    memset(data.data() + old_size - 1, 0, new_size - old_size);
}

void BitVec::set(width_t pos, bool value) {
  auto uidx = pos / ubits;
  auto uoffset = pos % ubits;
  // not include pos bit
  unit_t low_mask = (1L << uoffset) - 1;
  // not include pos bit
  unit_t high_mask = ~((1 << (uoffset + 1)) - 1);
  data[uidx] =
      (data[uidx] & low_mask) | (value << pos) | (data[uidx] & high_mask);
}

// assert bits < value
void BitVec::set(void *value, width_t bits) {
  require(bits < width(), "bits should less than value");
  require(bits > 0, "bits should greater than zero");
  auto copy_nr = bits / ubits;
  if (copy_nr) {
    memcpy(data.data(), value, copy_nr);
  }

  auto offset = bits % ubits;
  if (offset) {
    auto low_mask = (1L << offset) - 1;
    auto last_unit = ((unit_t *)value)[copy_nr];
    data[copy_nr] = (data[copy_nr] & ~low_mask) | (last_unit & low_mask);
  }
}

void BitVec::get(void *value, width_t bits) const {
  require(bits < width(), "bits should less than value");
  require(bits > 0, "bits should greater than zero");
  auto copy_nr = bits / ubits;
  if (copy_nr) {
    memcpy(value, data.data(), copy_nr);
  }

  auto offset = bits % ubits;
  if (offset) {
    auto low_mask = (1L << offset) - 1;
    auto &last_unit = ((unit_t *)value)[copy_nr];
    last_unit = (last_unit & ~low_mask) | (data[copy_nr] & low_mask);
  }
}

uint64_t BitVec::get() const { return *(uint64_t *)(data.data()); }