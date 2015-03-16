#ifndef FOOLGO_SRC_DEF_H_
#define FOOLGO_SRC_DEF_H_

#include <cstdint>

#define CANNOT_RUN_HERE() {\
            assert(false);\
        }

#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
    ClassName(const ClassName &) = delete; \
    ClassName &operator =(const ClassName &) = delete;

#define DISALLOW_COPY_AND_ASSIGN_AND_MOVE(ClassName) \
    ClassName(const ClassName &) = delete; \
    ClassName &operator =(const ClassName &) = delete; \
    ClassName(ClassName &&) = delete; \
    ClassName &operator =(ClassName &&) = delete;

namespace foolgo {

typedef uint32_t HashKey;

typedef char PointState;
const PointState BLACK_POINT = 0;
const PointState WHITE_POINT = 1;
const PointState EMPTY_POINT = 2;

}

#endif
