#include "../include/utils.h"
#include "bitvec/core.h"
#include <bitset>
#include <fmt/core.h>
#include <gtest/gtest.h>

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(BitVecTest, create) {
  auto foo = bv::BitVec(12, 32);
  EXPECT_EQ(foo.width(), 32);
  EXPECT_EQ(foo.to_string(), "0000000c");

  auto bar = bv::BitVec(12);
  EXPECT_EQ(bar.width(), 4);
  EXPECT_EQ(bar.to_string(), "c");

  auto baz = bv::BitVec(0b101);
  EXPECT_EQ(baz.width(), 3);
  EXPECT_EQ(baz.to_string(), "(x101)5");
}

class BitVecOp : public testing::Test {
public:
  std::bitset<64> ref0;
  std::bitset<64> ref1;
  bv::BitVec dut0;
  bv::BitVec dut1;
  size_t rand0;
  size_t rand1;

  BitVecOp() = default;

  void rand_init() {
    rand0 = rand();
    rand1 = rand();

    ref0 = std::bitset<64>(rand0);
    ref1 = std::bitset<64>(rand1);

    dut0 = bv::BitVec(rand0);
    dut1 = bv::BitVec(rand1);
  }

  void set_value(size_t lhs, size_t rhs) {
    rand0 = lhs;
    rand1 = rhs;

    ref0 = std::bitset<64>(rand0);
    ref1 = std::bitset<64>(rand1);

    dut0 = bv::BitVec(rand0);
    dut1 = bv::BitVec(rand1);
  }
};

TEST_F(BitVecOp, lessTest) {
  set_value(0b101110111, 0b101110110);
  EXPECT_EQ(rand0 < rand1, dut0 < dut1);
  set_value(0b111110111, 0b011110111);
  EXPECT_EQ(rand0 < rand1, dut0 < dut1);
  for (size_t i = 0; i < 10000; i++) {
    rand_init();
    EXPECT_EQ(rand0 < rand1, dut0 < dut1)
        << fmt::format("test {}: {} < {}", i, rand0, rand1);
  }
}

TEST_F(BitVecOp, greatTest) {
  for (size_t i = 0; i < 10000; i++) {
    rand_init();
    EXPECT_EQ(rand0 > rand1, dut0 > dut1)
        << fmt::format("test {}: {} > {}", i, rand0, rand1);
  }
}

TEST(BitVecTest, atTest) {
  std::bitset<64> ref;
  bv::BitVec dut;
  for (size_t i = 0; i < 100; i++) {
    const auto value = rand();
    ref = std::bitset<64>(value);
    dut = bv::BitVec(value, 64);
    for (size_t idx = 0; idx < 64; idx++) {
      EXPECT_EQ(ref[idx], dut[idx])
          << fmt::format("test{} = {:x}[{}]", i, value, idx);
    }
  }
}

TEST(BitVecTest, right_shift) {
  std::bitset<32> ref;
  bv::BitVec dut;
  for (size_t i = 0; i < 1000; i++) {
    auto value = rand();
    auto shift = rand() % 36;
    ref = std::bitset<32>(value) >> shift;
    dut = bv::BitVec(value, 32) >> shift;
    EXPECT_EQ(ref.to_ullong(), dut.get()) << fmt::format(
        "test{}: {:x} >> {} = {}", i, value, shift, dut.to_string());
  }
}

TEST(BitVecTest, left_shift) {
  std::bitset<32> ref;
  bv::BitVec dut;
  for (size_t i = 0; i < 1000; i++) {
    auto value = rand();
    auto shift = rand() % 36;
    ref = std::bitset<32>(value) << shift;
    dut = bv::BitVec(value, 32) << shift;
    EXPECT_EQ(ref.to_ullong(), dut.get()) << fmt::format(
        "test{}: {:x} << {} = {}", i, value, shift, dut.to_string());
  }
}

TEST(BitVecTest, and) {
  const auto l = uniform_rand();
  const auto r = uniform_rand();
  const auto lbv = bv::BitVec(l);
  const auto rbv = bv::BitVec(l);
}
