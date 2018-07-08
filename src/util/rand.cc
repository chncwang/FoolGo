#include "rand.h"

#include <algorithm>
#include <cstdint>
#include <random>


namespace foolgo {
namespace util {

uint32_t Rand(uint32_t max, uint32_t seed) {
  static std::mt19937 gen(seed);
  std::uniform_int_distribution<uint32_t> dstrbtion(0, max);
  return dstrbtion(gen);
}

}
}
