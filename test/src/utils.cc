#include "include/utils.h"

size_t uniform_rand(size_t min, size_t max) {
  static std::default_random_engine rd;
  auto engin = std::uniform_int_distribution<size_t>(min, max);
  return engin(rd);
}