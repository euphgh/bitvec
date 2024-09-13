#include "bitvec/core.h"
#include "bitvec/utils.h"
#include <bitset>
#include <fmt/core.h>
#include <gtest/gtest.h>

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

template <std::size_t N>
bool operator<(const std::bitset<N> &x, const std::bitset<N> &y) {
  for (int i = N - 1; i >= 0; i--) {
    if (x[i] ^ y[i])
      return y[i];
  }
  return false;
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
};

TEST_F(BitVecOp, helloTest) {
  for (size_t i = 0; i < 100; i++) {
    rand_init();
    EXPECT_EQ(ref0 < ref1, dut0 < dut1) << fmt::format(
        "test[{}] fail: {} {} {}", i, rand0, (ref0 < ref1 ? "<" : ">="), rand1);
  }
}

// TEST(BitVecTest, changable) {}

TEST(BitVecUtils, min_len) {
  EXPECT_EQ(bv::min_len(1), 1);
  EXPECT_EQ(bv::min_len(0), 1);
  EXPECT_EQ(bv::min_len(0b1000), 4);
  EXPECT_EQ(bv::min_len(0b1100), 4);
  EXPECT_EQ(bv::min_len(0b1010), 4);
  EXPECT_EQ(bv::min_len(0b1011), 4);
}

TEST(BitVecUtils, bin_char) {
  EXPECT_EQ(bin_char(0), '0');
  EXPECT_EQ(bin_char(1), '1');
  EXPECT_EQ(bin_char(2), '0');
}

TEST(BitVecUtils, hex_char) {
  EXPECT_EQ(bv::hex_char(0), '0');
  EXPECT_EQ(bv::hex_char(1), '1');
  EXPECT_EQ(bv::hex_char(2), '2');
  EXPECT_EQ(bv::hex_char(9), '9');
  EXPECT_EQ(bv::hex_char(0xa), 'a');
  EXPECT_EQ(bv::hex_char(0xb), 'b');
  EXPECT_EQ(bv::hex_char(0xc), 'c');
  EXPECT_EQ(bv::hex_char(0xd), 'd');
  EXPECT_EQ(bv::hex_char(0xe), 'e');
  EXPECT_EQ(bv::hex_char(0xf), 'f');
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
