#ifndef MC_SIMULATOR_TLT_H
#define MC_SIMULATOR_TLT_H

#include "mc_simulator.h"

#include <bitset>
#include <random>


template <BoardLen BOARD_LEN>
MCSimulator<BOARD_LEN> &MCSimulator<BOARD_LEN>::Ins()
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

    do {
        PlayerColor last_player = bingm.LastPlayer();
        PlayerColor cur_player = OppstColor(last_player);
        FOO_PRINT_LINE("cur_player = %d", cur_player);
        const auto &playable = bingm.PlayableIndexes(cur_player);

        std::bitset<BLSq<BOARD_LEN>()> noko_plbl(playable);
        PointIndex ko = bingm.KoIndex();
        if (ko != BoardInGm<BOARD_LEN>::NONE) {
            std::bitset<BLSq<BOARD_LEN>()> kobits;
            kobits.set();
            kobits.reset(ko);
            noko_plbl &= kobits;
        }

        PointIndex play_c = noko_plbl.count();
        FOO_PRINT_LINE("play_c = %d", play_c);
#ifdef FOO_TEST
        if (play_c > 0) {
            bingm.PRINT_PLAYABLE();
            FOO_PRINT_LINE("play_c = %d, playable indx = %d",
                    play_c,
                    GetLowest1<BLSq<BOARD_LEN>()>(playable));
        }
#endif
        if (play_c > 0) {
            PointIndex rand = this->Rand(play_c - 1);
            Point cur_indx =
                GetXst1<BLSq<BOARD_LEN>()>(noko_plbl, rand);
            FOO_PRINT_LINE("cur_indx = %d", cur_indx);
            bingm.PlayMove(Move(cur_player, cur_indx));
        } else {
            bingm.Pass(cur_player);
        }
        FOO_PRINT_LINE("\n ");
    } while(bingm.PlayableIndexes(BLACK_PLAYER).count() > 0 ||
            bingm.PlayableIndexes(WHITE_PLAYER).count() > 0);

    return bingm.BlackRegion();
}


template <BoardLen BOARD_LEN>
PointIndex MCSimulator<BOARD_LEN>::Rand(PointIndex max) const
{
    static unsigned ts =
        std::chrono::system_clock::now().time_since_epoch().count();
    static std::mt19937 gen(ts);
    std::uniform_int_distribution<PointIndex> dstrbtion(0, max);
    return dstrbtion(gen);
}


#endif
