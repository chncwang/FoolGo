#include "vector_util.h"

#include <cassert>
#include <type_traits>
#include <vector>

#include "../board/position.h"
#include "rand.h"

namespace foolgo {
namespace util {

using board::PositionIndex;

void RandomizeVector(std::vector<PositionIndex> *vctr, uint32_t seed) {
  int len = vctr->size();
  assert(len > 0);

  for (int i = 0; i < len; ++i) {
    int max = len - i - 1;
    int rand = Rand(max, seed) + i;
    std::swap(vctr->at(i), vctr->at(rand));
  }
}

}
}
