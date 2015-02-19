#ifndef BITSET_UTIL_H
#define BITSET_UTIL_H

#include <def.h>
#include <position.h>
#include <bitset>
#include <vector>

template<int LEN>
int GetLowest1(const std::bitset<LEN> &b) {
  assert(b.count() > 0);

  for (int i = 0;; ++i) {
    if (b[i]) {
      return i;
    }
  }

  return -1;
}

template<int LEN>
int GetXst1(const std::bitset<LEN> &b, int x) {
  assert((int)b.count() > x);

  for (int i = 0;; ++i) {
    if (b[i] && --x < 0) {
      return i;
    }
  }

  return -1;
}

template<int LEN>
std::vector<PointIndex> Get1s(const std::bitset<LEN> &b) {
  PointIndex c = b.count();
  std::vector<PointIndex> v(c);
  PointIndex vi = 0;

  for (int i = 0; i < b.size(); ++i) {
    if (b[i]) {
      v[vi++] = i;
    }
  }

  return v;
}

#endif
