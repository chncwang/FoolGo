#ifndef FOOLGO_SRC_MATH_VECTOR_UTIL_H_
#define FOOLGO_SRC_MATH_VECTOR_UTIL_H_

#include <vector>

#include "../board/position.h"

namespace foolgo {
namespace math {

void RandomizeVector(std::vector<board::PositionIndex> *vctr, uint32_t seed);

template<typename T>
std::vector<T> ConcatVectors(const std::vector<T> vctrs[4]) {
  std::vector<T> r(vctrs[0]);

  for (int i = 1; i < 4; ++i) {
    for (const T &v : vctrs[i])
      r.push_back(v);
  }

  return r;
}

}
}

#endif
