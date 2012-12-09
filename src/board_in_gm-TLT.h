#ifndef BOARD_IN_GM_TLT_H
#define BOARD_IN_GM_TLT_H

#include "board_in_gm.h"

#include <cstring>

#include <bitset>
#include <vector>
#include <algorithm>
#include <functional>

#include "bitset_util.h"

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
    for (int i=0; i<2; ++i) playable_indxs_[i].set();
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::Copy(const BoardInGm &b)
{
    board_.Copy(b.board_);
    for (int i=0; i<2; ++i) {
        chain_sets_[i].Copy(b.chain_sets_[i]);
        playable_indxs_[i] = b.playable_indxs_[i];
        eyes_[i] = b.eyes_[i];
        real_eyes_[i] = b.real_eyes_[i];
    }
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::PlayBasicMove(const Move &move)
{
    PlayerColor color = move.color_;
    PointIndex indx = move.indx_;
    FOO_ASSERT(board_.GetPoint(indx) == EMPTY_POINT);
    board_.SetPoint(indx, color);

    for (int i=0; i<2; ++i) chain_sets_[i].LetAdjcntChainsSetAir(indx, false);
    bitset<BoardLenSquare<BOARD_LEN>()> air_set;
    PlayerColor oc = OppstColor(color);
    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        PointIndex adj_indx = ins.GetIndex(adj_pos);
        if (board_.GetPoint(adj_indx) == oc &&
                chain_sets_[(int)oc].GetAirCountByPiece(adj_indx) == 0) {
            RemoveChain(Move(oc, adj_indx));
        }
        if (board_.GetPoint(adj_pos) == EMPTY_POINT) {
            air_set.set(adj_indx);
        }
    }

    chain_sets_[(int)color].AddPiece(indx, air_set);
    return chain_sets_[(int)color].GetAirCountByPiece(indx) == 0 ?
        false : true;
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::PlayMove(const Move &move)
{
    PlayerColor color = move.color_;
    PointIndex indx = move.indx_;
    FOO_ASSERT(board_.GetPoint(indx) == EMPTY_POINT);
    board_.SetPoint(indx, color);

    for (int i=0; i<2; ++i) {
        playable_indxs_[i].reset(indx);
        chain_sets_[i].LetAdjcntChainsSetAir(indx, false);
        eyes_[i].reset(indx);
        real_eyes_[i].reset(indx);
    }

    PlayerColor oc = OppstColor(color);
    bitset<BoardLenSquare<BOARD_LEN>()> air_set;
    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);
    typename BoardInGm<BOARD_LEN>::PointIndxVector ate_points[4];

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        PointIndex adj_indx = ins.GetIndex(adj_pos);
        if (board_.GetPoint(adj_indx) == oc &&
                chain_sets_[(int)oc].GetAirCountByPiece(adj_indx) == 0) {
            ate_points[i] = this->RemoveChain(Move(oc, adj_indx));
        }
        if (board_.GetPoint(adj_pos) == EMPTY_POINT) {
            air_set.set(adj_indx);
        }
    }

    chain_sets_[(int)color].AddPiece(indx, air_set);

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;
        if (board_.GetPoint(adj_pos) != EMPTY_POINT) continue;

        PointIndex adj_indx = ins.GetIndex(adj_pos);
        Move adj_move(color, adj_indx);
        this->UpdateEyes(adj_move);
        this->UpdateRealEyes(adj_move); //NOTE: This order is very important!!!
        if (this->IsFakeEye(adj_move)) {
            playable_indxs_[(int)oc][adj_indx] =
                this->IsMoveSuiside(Move(oc, adj_indx));
        }
    }

    for (int i=0; i<4; ++i) {
        Position oblq_pos = pos.ObliquePos(i);
        if (!ins.IsInBoard(oblq_pos)) continue;
        if (board_.GetPoint(oblq_pos) != EMPTY_POINT) continue;

        PointIndex oblq_indx = ins.GetIndex(oblq_pos);
        Move oblq_move(color, oblq_indx);
        this->UpdateRealEyes(oblq_move);
    }

    this->UpdtPlblIndxsArnd(indx);
    for(int i=0; i<4; ++i) this->UpdtAtePcsAdjChns(ate_points[i], oc);
    return chain_sets_[(int)color].GetAirCountByPiece(indx) == 0 ?
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
    return eyes_[(int)move.color_][move.indx_];
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsRealEye(const Move &move) const
{
    return real_eyes_[(int)move.color_][move.indx_];
}


template <BoardLen BOARD_LEN>
inline bool BoardInGm<BOARD_LEN>::IsFakeEye(const Move &move) const
{
    return this->IsEye(move) && !this->IsRealEye(move);
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsEmptySingly(PointIndex indx) const
{
    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        if (board_.GetPoint(adj_pos) == EMPTY_POINT) return false;
    }

    return true;
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsMoveSuiside(const Move &move) const
{
    FOO_ASSERT(board_.GetPoint(move.indx_) == EMPTY_POINT);
    BoardInGm<BOARD_LEN> copy;
    copy.Copy(*this);
    return !copy.PlayBasicMove(move);
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsPlayable(const Move &move) const
{
    return playable_indxs_[(int)move.color_][move.indx_];
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdateEyes(const Move &move)
{
    FOO_ASSERT(board_.GetPoint(move.indx_) == EMPTY_POINT);
    auto &ins = this->GetPosClcltr();
    PointIndex indx = move.indx_;
    const Position &pos = ins.GetPos(indx);
    PlayerColor color = move.color_;

    for (int i=0; i<4; ++i) {
        const Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) {
            continue;
        } else if (board_.GetPoint(pos.AdjcntPos(i)) != color) {
            eyes_[(int)color].reset(indx);
            return;
        }
    }

    eyes_[(int)color].set(indx);
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdateRealEyes(const Move &move)
{
    FOO_ASSERT(board_.GetPoint(move.indx_) == EMPTY_POINT);
    PlayerColor color = move.color_;
    PointIndex indx = move.indx_;
//    FOO_PRINT_LINE("color = %d, indx = %d", (int)color, indx);
    if (!this->IsEye(move)) {
        real_eyes_[(int)color].reset(indx);
        return;
    }

    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);
    auto status = ins.GetPosStatus(pos);
//    FOO_PRINT_LINE("status = %d", status);
    PointIndex count = 0;

    for (int i=0; i<4; ++i) {
        Position oblq_pos = pos.ObliquePos(i);
        if (!ins.IsInBoard(oblq_pos)) continue;

        count = (board_.GetPoint(oblq_pos) == color) ? count + 1 : count;
    }

    static const PointIndex TABLE[3] = {3, 2, 1};
    real_eyes_[(int)color][indx] = (TABLE[status] <= count);
}


template <BoardLen BOARD_LEN>
typename BoardInGm<BOARD_LEN>::PointIndxVector
BoardInGm<BOARD_LEN>::RemoveChain(const Move &move)
{
    auto p_chnset = chain_sets_ + move.color_;
    auto trp = p_chnset->GetPieces(move.indx_);
    chain_sets_[(int)move.color_].RemoveListByPiece(move.indx_);
    PlayerColor oc = OppstColor(move.color_);

    for (PointIndex indx : trp) {
        board_.SetPoint(indx, EMPTY_POINT);
        chain_sets_[(int)oc].LetAdjcntChainsSetAir(indx, true);
        for (int i=0; i<2; ++i) playable_indxs_[i].set(indx);
    }

    return trp;
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdtAdjPlblIndxsOfChn(PointIndex indx)
{
    FOO_ASSERT(board_.GetPoint(indx) != EMPTY_POINT);
    PlayerColor color = board_.GetPoint(indx);
    const auto p_c = chain_sets_ + color;
    AirCount air_c = p_c->GetAirCountByPiece(indx);
    auto air_set = chain_sets_[(int)color].GetAirSetByPiece(indx);
    auto c_playable = playable_indxs_ + color;
    auto oc_playable = playable_indxs_ + OppstColor(color);
    if (air_c == 1) {
        PointIndex air_indx =
            GetLowestOne<BoardLenSquare<BOARD_LEN>()>(air_set);
        Move move(color, air_indx);
        if (this->IsRealEye(move)) {
            c_playable->reset(air_indx);
            oc_playable->set(air_indx);
        } else if (this->IsFakeEye(move)) {
            c_playable->set(air_indx, !this->IsMoveSuiside(move));
            oc_playable->set(air_indx);
        } else /* not eye */ {
            c_playable->set(air_indx, !this->IsMoveSuiside(move));
            oc_playable->set(air_indx);
        }
    } else {
        auto not_real_eyes_ = ~real_eyes_[(int)color];
        *c_playable = (*c_playable | air_set) & not_real_eyes_;
        *oc_playable &= not_real_eyes_; //bug!!
//        auto c_fake_eyes = eyes_[(int)color] & ~real_eyes_[(int)color];
//        *oc_playable |= air_set & ~c_fake_eyes & ~real_eyes_[(int)color];
    }
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdtPlblIndxsArnd(PointIndex indx)
{
    this->UpdtAdjPlblIndxsOfChn(indx);
    auto &ins = this->GetPosClcltr();
    Position pos = ins.GetPos(indx);
    PlayerColor color = board_.GetPoint(indx);
    PlayerColor oc = OppstColor(color);
    function<void(PointIndex)> Funs[3];

    Funs[(int)color] = [](PointIndex) {};

    Funs[(int)oc] = [this](PointIndex indx) {
        this->UpdtAdjPlblIndxsOfChn(indx);
    };

    Funs[(int)EMPTY_POINT] = [this](PointIndex indx) {
        if (!this->IsEmptySingly(indx)) return;

        for (int i=0; i<2; ++i) {
            if (this->IsMoveSuiside(Move(i, indx))) {
                playable_indxs_[i].reset(indx);
            }
        }
    };

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        Point point = board_.GetPoint(adj_pos);
        PointIndex adj_indx = ins.GetIndex(adj_pos);
        Funs[(int)point](adj_indx);
    }
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdtAtePcsAdjChns(
        const BoardInGm<BOARD_LEN>::PointIndxVector &v,
        PlayerColor ate_color)
{
    auto &ins = this->GetPosClcltr();

    for (PointIndex indx : v) {
        const Position &pos = ins.GetPos(indx);
        for (int i=0; i<4; ++i) {
            Position adj_pos = pos.AdjcntPos(i);
            if (!ins.IsInBoard(adj_pos)) continue;

            if (board_.GetPoint(adj_pos) == OppstColor(ate_color)) {
                PointIndex adj_indx = ins.GetIndex(adj_pos);
                this->UpdtAdjPlblIndxsOfChn(adj_indx);
            }
        }
    }
}


#endif
