#include "def.h"

#include "board.h"
#include "board_in_gm.h"
#include "pos_cal.h"
#include "chain_set.h"
#include "board_in_gm.h"
#include "mc_simulator.h"
#include "zob_hasher.h"
#include "engine.h"
#include "vector_util.h"


int main(int argc, const char *argv[])
{
//    Board<9>::TEST();
//    PosCalculator<9>::TEST();
//    ChainSet<9>::TEST();
//    BoardInGm<9>::TEST();
//    MCSimulator<TEST_LEN>::TEST();
//    ZobHasher<TEST_LEN>::TEST();
    Engine<TEST_LEN>::TEST();
//    VectorUtil_TEST();
    return 0;
}
