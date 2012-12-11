#ifndef MC_SIMULATOR_TEST_H
#define MC_SIMULATOR_TEST_H

#include "mc_simulator.h"


template <BoardLen BOARD_LEN>
void MCSimulator<BOARD_LEN>::TEST()
{
    for (int i=0; i<1; ++i) {
        BoardInGm<TEST_LEN> b;
        b.Init();
        auto &mcs = MCSimulator<TEST_LEN>::Ins();
        mcs.Simulate(b);
    }
    printf("simulate complte.\n");
    FOO_PRINT_LINE("r = %d.", r);
}


#endif
