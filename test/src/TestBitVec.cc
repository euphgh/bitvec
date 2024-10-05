#include "bitvec/core.h"

int main() {
  auto foo = bv::BitVec(12, 32);
  auto bar = bv::BitVec(12, 32);
  foo[12] = true;
  foo[12] = bar[14];
  return 0;
}