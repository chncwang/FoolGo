#ifndef DEF_H
#define DEF_H

#include <cassert>

#define CANNOT_RUN_HERE() {\
            assert(false);\
        }

#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
    ClassName(const ClassName &) = delete; \
    ClassName &operator =(const ClassName &) = delete;
#define TEST_LEN (9)
#endif
