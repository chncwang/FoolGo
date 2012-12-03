#ifndef BOARD_IN_GM_TLT_H
#define BOARD_IN_GM_TLT_H

#include "board_in_gm.h"

#include "airset_util.h"

using namespace std;


inline static PlayerColor OppstColor(PlayerColor color)
{
    return ~color & 1;
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::Move(PlayerColor color, const Position &pos)
{
    board_.SetPoint(pos, color);
    for (int i=0; i<2; ++i) chain_sets_[i].LetAdjcntChainsSetAir(pos, false);
    PlayerColor oc = OppstColor(color);
    ChainSet<BOARD_LEN> *p_oppst_chain = chain_sets_ + oc;
    bitset<FOO_SQUARE(BOARD_LEN)> air_set;

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!board_.GetPosClcltr().IsInBoard(adj_pos)) continue;

        if (board_.GetPoint(adj_pos) == EMPTY_POINT) {
            SetAir<BOARD_LEN>(&air_set, adj_pos);
        } else if (board_.GetPoint(adj_pos) == oc &&
                p_oppst_chain->GetAirCountByPiece(adj_pos) == 0) {
            PntIndxVector trp = p_oppst_chain->GetPieces(adj_pos);
            p_oppst_chain->RemoveListByPiece(adj_pos);
            PntIndxVector::iterator it;

            for (it=trp.begin(); it!=trp.end(); ++it) {
                board_.SetPoint(*it, EMPTY_POINT);
                chain_sets_[(int)color].LetAdjcntChainsSetAir(*it, true);
            }
        }
    }

    chain_sets_[(int)color].AddPiece(pos, air_set);
}


#endif
