#include "bitvec/core.h"
#include "include/utils.h"
#include <cstddef>
#include <gtest/gtest.h>

TEST(BitRefTest, assignConst) {
  const auto width = 48;
  auto bv = bv::BitVec(0, width);
  for (size_t i = 0; i < width; i++) {
    bv[i] = true;
    EXPECT_TRUE(bv[i]);
    EXPECT_EQ(bv.get(), (1ULL << (i + 1)) - 1) << bv.data_check();
  }
}

TEST(BitRefTest, copyFunc) {
  const auto width = 1024;
  for (size_t r = 0; r < 1024; r++) {
    auto src = bv::BitVec::random(width);
    auto dst = bv::BitVec(0, width);
    for (size_t i = 0; i < width; i++) {
      dst[i] = src[i];
      EXPECT_EQ(dst[i], src[i]);
    }
  }
}

TEST(BitRefTest, flip) {
  const auto width = 64;
  for (size_t r = 0; r < 1024; r++) {
    const auto value = uniform_rand();
    auto bv = bv::BitVec(value, width);
    for (size_t i = 0; i < width; i++) {
      bv[i].flip();
    }
    EXPECT_EQ(bv.get(), ~value) << "origin = " << value;
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
