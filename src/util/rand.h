#ifndef FOOLGO_SRC_MATH_RAND_H_
#define FOOLGO_SRC_MATH_RAND_H_

#include <chrono>
#include <cstdint>


namespace foolgo {

uint32_t Rand(uint32_t max, uint32_t seed);

inline uint32_t GetTimeSeed() {
  static unsigned time_seed =
      std::chrono::system_clock::now().time_since_epoch().count();
  return time_seed;
}

}

#endif
