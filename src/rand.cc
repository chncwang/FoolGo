#include <types_and_constants.h>
#include "rand.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <random>

HashKeyType Rand(HashKeyType max) {
  static unsigned ts =
      std::chrono::system_clock::now().time_since_epoch().count();
  static std::mt19937 gen(ts);
  std::uniform_int_distribution<uint32_t> dstrbtion(0, max);
  return dstrbtion(gen);
}
