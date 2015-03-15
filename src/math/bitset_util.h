#ifndef FOOLGO_SRC_MATH_BITSET_UTIL_H_
#define FOOLGO_SRC_MATH_BITSET_UTIL_H_

#include <bitset>
#include <cassert>
#include <vector>

#include "../board/position.h"

namespace foolgo {
namespace math {

template<int LEN>
int GetLowestOne(const std::bitset<LEN> &b) {
  assert(b.count() > 0);

  for (int i = 0;; ++i) {
    if (b[i]) {
      return i;
    }
  }

  return -1;
}

template<int LEN>
int CalSpecifiedOneOccurrenceTimeIndex(const std::bitset<LEN> &b, int x) {
  assert((int)b.count() > x);

  for (int i = 0;; ++i) {
    if (b[i] && --x < 0) {
      return i;
    }
  }

  return -1;
}

template<int LEN>
std::vector<board::PositionIndex> GetOnePositionIndexes(
    const std::bitset<LEN> &b) {
  board::PositionIndex c = b.count();
  std::vector<board::PositionIndex> v(c);
  board::PositionIndex vi = 0;

  for (int i = 0; i < b.size(); ++i) {
    if (b[i]) {
      v[vi++] = i;
    }
  }

  return v;
}

}
}

#endif
