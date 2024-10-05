#include "bitvec/utils.h"
#include <climits>
#include <gtest/gtest.h>

using namespace bv;

TEST(BitVecUtils, minLen) {
  EXPECT_EQ(min_len(1), 1);
  EXPECT_EQ(min_len(0), 1);
  EXPECT_EQ(min_len(0b1000), 4);
  EXPECT_EQ(min_len(0b1100), 4);
  EXPECT_EQ(min_len(0b1010), 4);
  EXPECT_EQ(min_len(0b1011), 4);
}

TEST(BitVecUtils, binChar) {
  EXPECT_EQ(bin_char(0), '0');
  EXPECT_EQ(bin_char(1), '1');
  EXPECT_EQ(bin_char(2), '0');
}

TEST(BitVecUtils, hexChar) {
  EXPECT_EQ(hex_char(0), '0');
  EXPECT_EQ(hex_char(1), '1');
  EXPECT_EQ(hex_char(2), '2');
  EXPECT_EQ(hex_char(9), '9');
  EXPECT_EQ(hex_char(0xa), 'a');
  EXPECT_EQ(hex_char(0xb), 'b');
  EXPECT_EQ(hex_char(0xc), 'c');
  EXPECT_EQ(hex_char(0xd), 'd');
  EXPECT_EQ(hex_char(0xe), 'e');
  EXPECT_EQ(hex_char(0xf), 'f');
}

TEST(BitVecUtils, bitMask) {
  EXPECT_EQ(one_bit_mask(0), 1L);
  EXPECT_EQ(one_bit_mask(1), 2L);
  EXPECT_EQ(one_bit_mask(2), 4L);
  EXPECT_EQ(one_bit_mask(3), 8L);
  EXPECT_EQ(one_bit_mask(4), 16L);
  EXPECT_EQ(one_bit_mask(10), 1024L);
  EXPECT_EQ(one_bit_mask(63), ~SSIZE_MAX);

  EXPECT_EQ(low_bit_mask(0), 0L);
  EXPECT_EQ(low_bit_mask(1), 1L);
  EXPECT_EQ(low_bit_mask(2), 3L);
  EXPECT_EQ(low_bit_mask(3), 7L);
  EXPECT_EQ(low_bit_mask(4), 15L);
  EXPECT_EQ(low_bit_mask(10), 1023L);
  EXPECT_EQ(low_bit_mask(63), SSIZE_MAX);
  EXPECT_EQ(low_bit_mask(64), SIZE_MAX);
}