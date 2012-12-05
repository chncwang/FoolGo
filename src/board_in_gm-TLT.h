#ifndef BOARD_IN_GM_TLT_H
#define BOARD_IN_GM_TLT_H

#include "board_in_gm.h"

#include <bitset>

#include "airset_util.h"

using namespace std;


inline static PlayerColor OppstColor(PlayerColor color)
{
    return ~color & 1;
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::Move(PlayerColor color, const Position &pos)
{
    FOO_ASSERT(board_.GetPoint(pos) == EMPTY_POINT);
    board_.SetPoint(pos, color);
    for (int i=0; i<2; ++i) chain_sets_[i].LetAdjcntChainsSetAir(pos, false);
    PlayerColor oc = OppstColor(color);
    bitset<FOO_SQUARE(BOARD_LEN)> air_set;

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!board_.GetPosClcltr().IsInBoard(adj_pos)) continue;

        if (board_.GetPoint(adj_pos) == oc &&
                chain_sets_[(int)oc].GetAirCountByPiece(adj_pos) == 0) {
            this->RemoveChain(adj_pos, oc);
        }
        if (board_.GetPoint(adj_pos) == EMPTY_POINT) {
            SetAir<BOARD_LEN>(&air_set, adj_pos);
        }
    }

    chain_sets_[(int)color].AddPiece(pos, air_set);
    return chain_sets_[(int)color].GetAirCountByPiece(pos) == 0 ? false : true;
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsMoveSuiside(PlayerColor color,
                                         const Position &pos) const
{
    FOO_ASSERT(board_.GetPoint(pos) == EMPTY_POINT);
    BoardInGm<BOARD_LEN> copy(*this);
    return !copy.Move(color, pos);

//    ChainSet<BOARD_LEN> store[2];
//    for (int i=0; i<2; ++i) store[i] = chain_sets_[i];
//    Board<BOARD_LEN> sboard(board_);
//    bool r = const_cast<BoardInGm<BOARD_LEN> *>(this)->Move(color, pos);

//    for (int i=0; i<2; ++i) {
//        swap(store[i], chain_sets_[i]);
//    }

//    swap(sboard, board_);
//    return !r;
}


template <BoardLen BOARD_LEN>
void
BoardInGm<BOARD_LEN>::RemoveChain(const Position &pos, PlayerColor color)
{
    ChainSet<BOARD_LEN> *p_chnset = chain_sets_ + color;
    PntIndxVector trp = p_chnset->GetPieces(pos);
    chain_sets_[(int)color].RemoveListByPiece(pos);
    PntIndxVector::iterator it;
    PlayerColor oc = OppstColor(color);

    for (it=trp.begin(); it!=trp.end(); ++it) {
        board_.SetPoint(*it, EMPTY_POINT);
        chain_sets_[(int)oc].LetAdjcntChainsSetAir(*it, true);
    }
}


template <BoardLen BOARD_LEN>
bool
BoardInGm<BOARD_LEN>::IsRealEye(const Position &pos, PlayerColor color) const
{
    FOO_ASSERT(board_.GetPoint(pos) == EMPTY_POINT);
    PointIndex illegal_c(0), empty_c(0);

    for (int i=0; i<8; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!board_.GetPosClcltr().IsInBoard(adj_pos)) {
            ++illegal_c;
            continue;
        }

//        if (board_.GetPoint(adj_pos) == 
    }
}


#endif
