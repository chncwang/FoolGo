#ifndef BITSET_UTIL_H
#define BITSET_UTIL_H

#include <bitset>

#include "def.h"


template <int LEN>
int GetLowestOne(const std::bitset<LEN> &b)
{
    FOO_ASSERT(b.count() > 0);

    for (int i=0; ; ++i) {
        if (b[i]) return i;
    }

    return -1;
}


#endif
