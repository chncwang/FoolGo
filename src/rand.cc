#include "rand.h"

#include <random>
#include <chrono>


HashKeyType Rand(HashKeyType max)
{
    static unsigned ts =
        std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 gen(ts);
    std::uniform_int_distribution<uint32_t> dstrbtion(0, max);
    return dstrbtion(gen);
}
