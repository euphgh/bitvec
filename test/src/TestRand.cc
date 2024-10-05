#include <fmt/core.h>
#include <random>

#define DEBUG(var) fmt::println(#var " = {}", var)

int main() {
  // 每次执行，时候生成的随机数都不相同，适合发布版
  std::random_device rd;
  // 每次执行，生成的随机数相同，适合调试
  std::default_random_engine dre;
  for (size_t i = 0; i < 10; i++) {
    fmt::println("rd[{}] = {}", i, rd());
  }

  std::uniform_int_distribution<size_t> dist(0, SIZE_MAX);
  for (size_t i = 0; i < 10; i++) {
    fmt::println("engin[{}] = {}", i, dist(dre));
  }
  uint8_t uint8_max = ~0;
  DEBUG(uint8_max);
  uint16_t uint16_max = ~0;
  DEBUG(uint16_max);
  uint32_t uint32_max = ~0;
  DEBUG(uint32_max);
  uint64_t uint64_max = ~0;
  DEBUG(uint64_max);
}