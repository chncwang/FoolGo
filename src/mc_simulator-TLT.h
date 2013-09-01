#ifndef MC_SIMULATOR_TLT_H
#define MC_SIMULATOR_TLT_H

#include "mc_simulator.h"
#include "rand.h"

#include <bitset>
#include <random>


template <BoardLen BOARD_LEN>
inline MCSimulator<BOARD_LEN> &MCSimulator<BOARD_LEN>::Ins()
{
    static MCSimulator<BOARD_LEN> ins;
    return ins;
}


template <BoardLen BOARD_LEN>
PointIndex
MCSimulator<BOARD_LEN>::Simulate(const BoardInGm<BOARD_LEN> &input_board) const
{
    BoardInGm<BOARD_LEN> bingm;
    bingm.Copy(input_board);
    int count = 0;

    while(!IsEnd(bingm)) {
        if (++count > MoveLimit<BOARD_LEN>()) break;
//        PRINT_LINE(" ");
//        bingm.PRINT_BOARD();
//        bingm.PRINT_PLAYABLE();
        PlayerColor last_player = bingm.LastPlayer();
        PlayerColor cur_player = OppstColor(last_player);
//        PRINT_LINE("cur_player = %d", cur_player);
        auto noko_plbl = NokoPlayableIndexes(bingm, cur_player);

        PointIndex play_c = noko_plbl.count();
        if (play_c > 0) {
            PointIndex rand = Rand(play_c - 1);
            PointIndex cur_indx =
                GetXst1<BoardLenSquare<BOARD_LEN>()>(noko_plbl, rand);
//            PRINT_LINE("cur_indx = %d", cur_indx);
            bingm.PlayMove(Move(cur_player, cur_indx));
        } else {
            bingm.Pass(cur_player);
        }
//        PRINT_LINE("\n ");
    };
//    PRINT_LINE("count = %d", count);
//    bingm.PRINT_BOARD();
    return bingm.BlackRegion();
}


#endif
