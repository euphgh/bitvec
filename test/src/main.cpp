#include "bitvec/core.h"
#include <gtest/gtest.h>

TEST(BitVecTest, create) {
  auto foo = bv::BitVec(12, 32);
  EXPECT_EQ(foo.to_string(), "c") << "fail" ;
  auto bar = bv::BitVec(12, 32);
}

TEST(BitVecTest, changable) {}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
