//#define FOO_TEST

#include <cstdio>
#include <cassert>

#ifdef FOO_TEST

#define ASSERT(a) assert(a)
#define PRINT_LINE(...) {\
            printf(__VA_ARGS__);\
            printf("\n");\
        }
#define CANNOT_RUN_HERE() {\
            PRINT_LINE("error: cannot run here!");\
            ASSERT(false);\
        }

#else

#define ASSERT(a)
#define PRINT_LINE(...)
#define CANNOT_RUN_HERE() 

#endif

#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
    ClassName(const ClassName &) = delete; \
    ClassName &operator =(const ClassName &) = delete;
#define TEST_LEN (9)
