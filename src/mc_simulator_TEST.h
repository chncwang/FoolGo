#ifndef MC_SIMULATOR_TEST_H
#define MC_SIMULATOR_TEST_H

#include "mc_simulator.h"

#include <ctime>
#include <cmath>


template <BoardLen BOARD_LEN>
void MCSimulator<BOARD_LEN>::TEST()
{
    int begin = clock();
    const int a = 10000;
    for (int i=0; i<a; ++i) {
        BoardInGm<TEST_LEN> b;
        b.Init();
//        PRINT_LINE("i = %d", i);
        auto &mcs = MCSimulator<TEST_LEN>::Ins();
        mcs.Simulate(b);
    }
    int end = clock();
    printf("time = %f\n", (float)(end - begin) / 1000000);
    printf("simulate complte.\n");
}


#endif
