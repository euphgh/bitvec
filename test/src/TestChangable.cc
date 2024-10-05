#include "bitvec/core.h"
#include "bitvec/type.h"
#include <bitset>
#include <cstddef>
#include <cstdlib>
#include <fmt/core.h>
#include <gtest/gtest.h>

TEST(TestBitVecChangable, sign_extend) {
  const auto value = rand();
  const auto width = rand() % 32;
  auto bv = bv::BitVec(value, width);
  for (bv::width_t pos = 0; pos < width; pos++) {
    auto dut = bv;
    dut.sign_extend(pos);
    for (bv::width_t idx = 0; idx < pos; idx++) {
    }
  }
}
