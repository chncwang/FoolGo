#define FOO_TEST

#include <cstdio>
#include <cassert>

#ifdef FOO_TEST

#define FOO_ASSERT(a) assert(a);
#define FOO_PRINT_LINE(...) {\
            printf(__VA_ARGS__);\
            printf("\n");\
        }
#define CANNOT_RUN_HERE() {\
            FOO_PRINT_LINE("error: cannot run here!");\
            FOO_ASSERT(false);\
        }

#else

#define FOO_ASSERT(a)
#define FOO_PRINT_LINE(...)
#define CANNOT_RUN_HERE() 

#endif
