#ifndef VECTOR_UTIL_H
#define VECTOR_UTIL_H

#include <vector>

#include "position.h"
#include "def.h"

void RandomizeVector(std::vector<PointIndex> *vctr);

template <typename T>
std::vector<T> ConcatVectors(const std::vector<T> vctrs[4])
{
    std::vector<T> r(vctrs[0]);

    for (int i=1; i<4; ++i) {
        for (T v : vctrs[i]) r.push_back(v);
    }

    return r;
}



#ifdef DTEST
void VectorUtil_TEST();
#endif

#endif
