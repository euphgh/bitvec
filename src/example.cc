#include "bitvec/core.h"
#include <fmt/core.h>

int main() {
  auto foo = bv::BitVec(12, 12);
  auto bar = bv::BitVec(7);
  fmt::print("BitVec(12, 32) = {}\n", foo.to_string());
  fmt::print("BitVec(12, 32) = {}\n", foo.to_string("b"));
  fmt::print("BitVec(7) = {}\n", bar.to_string());
  return 0;
}