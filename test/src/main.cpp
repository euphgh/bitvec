#include "bitvec/core.h"
#include "bitvec/utils.h"
#include <gtest/gtest.h>

TEST(BitVecTest, create) {
  auto foo = bv::BitVec(12, 32);
  EXPECT_EQ(foo.to_string(), "000c");
  auto bar = bv::BitVec(12);
  EXPECT_EQ(bar.to_string(), "c");
}

TEST(BitVecTest, changable) {}

TEST(BitVecUtils, min_len) {
  EXPECT_EQ(bv::min_len(1), 1);
  EXPECT_EQ(bv::min_len(1), 0);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
