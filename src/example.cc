#include "bitvec/core.h"
#include <fmt/core.h>

int main() {
  auto foo = BitVec::create(12, 32);
  auto bar = BitVec::create(7);
  fmt::print("BitVec(12, 32) = {}\n", foo.to_string());
  fmt::print("BitVec(12, 32) = {}\n", foo.to_string("b"));
  fmt::print("BitVec(7) = {}\n", bar.to_string());
  return 0;
}