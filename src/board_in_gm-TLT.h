#ifndef BOARD_IN_GM_TLT_H
#define BOARD_IN_GM_TLT_H

#include "board_in_gm.h"

#include <cstring>

#include <bitset>
#include <algorithm>

#include "airset_util.h"

using namespace std;


inline static PlayerColor OppstColor(PlayerColor color)
{
    return ~color & 1;
}


template <BoardLen BOARD_LEN>
BoardInGm<BOARD_LEN>::BoardInGm() {}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::Init()
{
    board_.Init();
    for (int i=0; i<2; ++i) chain_sets_[i].Init();
    for (int i=0; i<2; ++i) pntindx_sets_[i].Init();
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::Copy(const BoardInGm &b)
{
    board_.Copy(b.board_);
    for (int i=0; i<2; ++i) chain_sets_[i].Copy(b.chain_sets_[i]);
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::PlayMove(const Move &move)
{
    FOO_ASSERT(board_.GetPoint(move.indx_) == EMPTY_POINT);
    board_.SetPoint(move.indx_, move.color_);
    for (int i=0; i<2; ++i) {
        chain_sets_[i].LetAdjcntChainsSetAir(move.indx_, false);
    }
    PlayerColor oc = OppstColor(move.color_);
    bitset<FOO_SQUARE(BOARD_LEN)> air_set;
    const PosCalculator<BOARD_LEN> &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(move.indx_);

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;
        PointIndex adj_indx = ins.GetIndex(adj_pos);

        if (board_.GetPoint(adj_pos) == oc &&
                chain_sets_[(int)oc].GetAirCountByPiece(adj_indx) == 0) {
            Move toremove = {oc, adj_indx};
            this->RemoveChain(toremove);
        }
        if (board_.GetPoint(adj_pos) == EMPTY_POINT) {
            SetAir<BOARD_LEN>(&air_set, adj_pos);
        }
    }

    chain_sets_[(int)move.color_].AddPiece(move.indx_, air_set);
    return chain_sets_[(int)move.color_].GetAirCountByPiece(move.indx_) == 0 ?
        false : true;
}


template <BoardLen BOARD_LEN>
PosCalculator<BOARD_LEN> &BoardInGm<BOARD_LEN>::GetPosClcltr() const
{
    return board_.GetPosClcltr();
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsEye(const Move &move) const
{
    FOO_ASSERT(board_.GetPoint(move.indx_) == EMPTY_POINT);
    const Position &pos = this->GetPosClcltr().GetPos(move.indx_);

    for (int i=0; i<4; ++i) {
        if (board_.GetPoint(pos.AdjcntPos(i)) != move.color_) return false;
    }

    return true;
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsMoveSuiside(const Move &move) const
{
    FOO_ASSERT(board_.GetPoint(move.indx_) == EMPTY_POINT);
    BoardInGm<BOARD_LEN> copy;
    copy.Copy(*this);
    return !copy.PlayMove(move);
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::RemoveChain(const Move &move)
{
    ChainSet<BOARD_LEN> *p_chnset = chain_sets_ + move.color_;
    PntIndxVector trp = p_chnset->GetPieces(move.indx_);
    chain_sets_[(int)move.color_].RemoveListByPiece(move.indx_);
    PntIndxVector::iterator it;
    PlayerColor oc = OppstColor(move.color_);

    for (it=trp.begin(); it!=trp.end(); ++it) {
        board_.SetPoint(*it, EMPTY_POINT);
        chain_sets_[(int)oc].LetAdjcntChainsSetAir(*it, true);
    }
}


#endif
