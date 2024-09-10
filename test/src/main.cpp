#include "bitvec/core.h"
#include <gtest/gtest.h>

TEST(BitVecTest, create) {
  auto foo = BitVec::create(12, 32);
  EXPECT_EQ(foo.to_string(), "c") << "fail" ;
  auto bar = BitVec::create(12, 32);
}

TEST(BitVecTest, changable) {}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
