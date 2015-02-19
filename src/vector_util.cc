#include <position.h>
#include <rand.h>
#include "vector_util.h"
#include <cassert>
#include <type_traits>
#include <vector>

void RandomizeVector(std::vector<PointIndex> *vctr) {
  int len = vctr->size();
  assert(len > 0);

  for (int i = 0; i < len; ++i) {
    int max = len - i - 1;
    int rand = Rand(max) + i;
    std::swap(vctr->at(i), vctr->at(rand));
  }
}
