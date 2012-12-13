#include "rand.h"

#include <random>


uint32_t g_Rand(uint32_t max)
{
    static unsigned ts =
        std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 gen(ts);
    std::uniform_int_distribution<uint32_t> dstrbtion(0, max);
    return dstrbtion(gen);
}
