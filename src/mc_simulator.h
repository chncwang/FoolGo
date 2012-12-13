#ifndef MC_SIMULATOR_H
#define MC_SIMULATOR_H

#include "def.h"
#include "position.h"
#include "board_in_gm.h"



template <BoardLen BOARD_LEN>
class MCSimulator
{
public:
    static MCSimulator &Ins();

    PointIndex Simulate(const BoardInGm<BOARD_LEN> &input_board) const;
    // return black region;
//#ifdef FOO_TEST
    static void TEST();
//#endif

private:
    MCSimulator() = default;
    DISALLOW_COPY_AND_ASSIGN(MCSimulator);
};



#include "mc_simulator-TLT.h"
//#ifdef FOO_TEST
#include "mc_simulator_TEST.h"
//#endif

#endif
