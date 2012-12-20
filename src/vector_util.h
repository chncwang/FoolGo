#ifndef VECTOR_UTIL_H
#define VECTOR_UTIL_H

#include <vector>

#include "position.h"
#include "def.h"

void RandomizeVector(std::vector<PointIndex> *vctr);
#ifdef DTEST
void VectorUtil_TEST();
#endif

#endif
