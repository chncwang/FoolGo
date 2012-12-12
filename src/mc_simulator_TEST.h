#ifndef MC_SIMULATOR_TEST_H
#define MC_SIMULATOR_TEST_H

#include "mc_simulator.h"

#include <ctime>
#include <cmath>


template <BoardLen BOARD_LEN>
void MCSimulator<BOARD_LEN>::TEST()
{
    int begin = clock();
    int sum = 0;
    const int a = 10000;
    for (int i=0; i<a; ++i) {
        BoardInGm<TEST_LEN> b;
        b.Init();
        auto &mcs = MCSimulator<TEST_LEN>::Ins();
        int r = mcs.Simulate(b);
        sum += r;
    }
    int end = clock();
    printf("time = %f\n", (float)(end - begin) / 1000000);
    printf("simulate complte.\n");
    printf("average black = %f\n", (float)sum / a);
}


#endif
