#include "rand.h"

#include <algorithm>
#include <cstdint>
#include <random>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace foolgo {

uint32_t Rand(uint32_t max, uint32_t seed) {
  return rand() % (max + 1);
}

}
