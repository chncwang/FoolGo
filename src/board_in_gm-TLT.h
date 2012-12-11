#ifndef BOARD_IN_GM_TLT_H
#define BOARD_IN_GM_TLT_H

#include "board_in_gm.h"

#include <cstring>

#include <bitset>
#include <vector>
#include <algorithm>
#include <functional>

#include "bitset_util.h"


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::Init()
{
    board_.Init();
    for (int i=0; i<2; ++i) playable_indxs_[i].set();
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::Copy(const BoardInGm &b)
{
    this->BasicCopy(b);
    ko_indx_ = b.ko_indx_;
    last_player_ = b.last_player_;
    b_pcs_c_ = b.b_pcs_c_;

    for (int i=0; i<2; ++i) {
        playable_indxs_[i] = b.playable_indxs_[i];
        eyes_[i] = b.eyes_[i];
        real_eyes_[i] = b.real_eyes_[i];
    }
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::PlayMove(const Move &move)
{
    PlayerColor color = move.color_;
    PointIndex indx = move.indx_;
    FOO_ASSERT(board_.GetPoint(indx) == EMPTY_POINT);
    b_pcs_c_ += OppstColor(color);

    for (int i=0; i<2; ++i) {
        playable_indxs_[i].reset(indx);
        eyes_[i].reset(indx);
        real_eyes_[i].reset(indx);
    }

    PlayerColor oc = OppstColor(color);
    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);
    typename BoardInGm<BOARD_LEN>::PointIndxVector ate_points[4];
#ifdef FOO_TEST
    BoardInGm<BOARD_LEN> copy;
    copy.Copy(*this);
    bool r = this->PlayBasicMove(move, ate_points);
    if (!r) {
        FOO_PRINT_LINE("color = %d, indx = %d", color, indx);
        CANNOT_RUN_HERE();
    }
#else
    this->PlayBasicMove(move, ate_points);
#endif

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;
        if (board_.GetPoint(adj_pos) != EMPTY_POINT) continue;

        PointIndex adj_indx = ins.GetIndex(adj_pos);
        this->UpdateEye(Move(color, adj_indx));
    }

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;
        if (board_.GetPoint(adj_pos) != EMPTY_POINT) continue;

        PointIndex adj_indx = ins.GetIndex(adj_pos);
        Move adj_move(color, adj_indx);
        this->UpdateRealEye(adj_move);
        if (this->IsFakeEye(adj_move)) {
            playable_indxs_[oc][adj_indx] =
                this->IsSuiside(Move(oc, adj_indx));
        }
    }

    for (int i=0; i<4; ++i) {
        Position oblq_pos = pos.ObliquePos(i);
        if (!ins.IsInBoard(oblq_pos)) continue;
        if (board_.GetPoint(oblq_pos) != EMPTY_POINT) continue;

        PointIndex oblq_indx = ins.GetIndex(oblq_pos);
        Move oblq_move(color, oblq_indx);
        this->UpdateRealEye(oblq_move);
    }

    this->UpdtPlblIndxsArnd(indx);
    for(int i=0; i<4; ++i) this->UpdtAtePcsAdjChns(ate_points[i], oc);
    ko_indx_ = BoardInGm<BOARD_LEN>::NONE;
    PointIndex single_ate = BoardInGm<BOARD_LEN>::NONE;

    for (int i=0; i<4; ++i) {
        if (ate_points[i].size() == 1) {
            single_ate = ate_points[i][0];
            break;
        }
    }

    if (single_ate != BoardInGm<BOARD_LEN>::NONE &&
            chain_sets_[color].GetAirCountByPiece(indx) == 1) {
        ko_indx_ = single_ate;
    }

    last_player_ = color;
}


template <BoardLen BOARD_LEN>
inline void BoardInGm<BOARD_LEN>::Pass(PlayerColor color)
{
    last_player_ = color;
    ko_indx_ = BoardInGm<BOARD_LEN>::NONE;
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::BasicCopy(const BoardInGm<BOARD_LEN> &b)
{
    board_.Copy(b.board_);
    for (int i=0; i<2; ++i) chain_sets_[i].Copy(b.chain_sets_[i]);
}


template <BoardLen BOARD_LEN>
PosCalculator<BOARD_LEN> &BoardInGm<BOARD_LEN>::GetPosClcltr() const
{
    return board_.GetPosClcltr();
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsEye(const Move &move) const
{
    return eyes_[move.color_][move.indx_];
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsRealEye(const Move &move) const
{
    return real_eyes_[move.color_][move.indx_];
}


template <BoardLen BOARD_LEN>
inline bool BoardInGm<BOARD_LEN>::IsFakeEye(const Move &move) const
{
    return this->IsEye(move) && !this->IsRealEye(move);
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsSelfPieceOrEye(const Move &move) const
{
    return board_.GetPoint(move.indx_) == move.color_ || this->IsEye(move);
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
bool BoardInGm<BOARD_LEN>::IsSuiside(const Move &move) const
{
    FOO_ASSERT(board_.GetPoint(move.indx_) == EMPTY_POINT);
    BoardInGm<BOARD_LEN> copy;
    copy.BasicCopy(*this);
    return !copy.PlayBasicMove(move);
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsPlayable(const Move &move) const
{
    return playable_indxs_[move.color_][move.indx_];
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::PlayBasicMove(
        const Move &move,
        typename BoardInGm<BOARD_LEN>::PointIndxVector *v)
{
    PlayerColor color = move.color_;
    PointIndex indx = move.indx_;
    FOO_ASSERT(board_.GetPoint(indx) == EMPTY_POINT);
    board_.SetPoint(indx, color);

    for (int i=0; i<2; ++i) chain_sets_[i].LetAdjcntChainsSetAir(indx, false);
    std::bitset<BLSq<BOARD_LEN>()> air_set;
    PlayerColor oc = OppstColor(color);
    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);

    std::function<void(const typename BoardInGm<BOARD_LEN>::PointIndxVector &,
            int)> Fun;
    if (v == nullptr) {
        Fun = [](const typename BoardInGm<BOARD_LEN>::PointIndxVector &r, int i)
        {};
    } else {
        Fun =
        [v](const typename BoardInGm<BOARD_LEN>::PointIndxVector &r, int i) {
            v[i] = r;
        };
    }

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        PointIndex adj_indx = ins.GetIndex(adj_pos);
        if (board_.GetPoint(adj_indx) == oc &&
                chain_sets_[oc].GetAirCountByPiece(adj_indx) == 0) {
            auto r = RemoveChain(Move(oc, adj_indx));
            Fun(r, i);
        }
        if (board_.GetPoint(adj_pos) == EMPTY_POINT) {
            air_set.set(adj_indx);
        }
    }

    chain_sets_[color].AddPiece(indx, air_set);
    return chain_sets_[color].GetAirCountByPiece(indx) == 0 ?
        false : true;
}


template <BoardLen BOARD_LEN>
typename BoardInGm<BOARD_LEN>::PointIndxVector
BoardInGm<BOARD_LEN>::RemoveChain(const Move &move)
{
    auto p_chnset = chain_sets_ + move.color_;
    auto trp = p_chnset->GetPieces(move.indx_);
    chain_sets_[move.color_].RemoveListByPiece(move.indx_);
    PlayerColor oc = OppstColor(move.color_);

    for (PointIndex indx : trp) {
        board_.SetPoint(indx, EMPTY_POINT);
        chain_sets_[oc].LetAdjcntChainsSetAir(indx, true);
        for (int i=0; i<2; ++i) playable_indxs_[i].set(indx);
    }

    b_pcs_c_ -= trp.size() * OppstColor(move.color_);
    return trp;
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdateEye(const Move &move)
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
            eyes_[color].reset(indx);
            return;
        }
    }

    eyes_[color].set(indx);

    for (int i=0; i<4; ++i) {
        const Position oblq_pos = pos.ObliquePos(i);
        if (!ins.IsInBoard(oblq_pos)) continue;

        PointIndex oblq_indx = ins.GetIndex(oblq_pos);
        if (board_.GetPoint(oblq_indx) == EMPTY_POINT) {
            this->UpdateRealEye(Move(color, oblq_indx));
        }
    }
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdateRealEye(const Move &move)
{
    FOO_ASSERT(board_.GetPoint(move.indx_) == EMPTY_POINT);
    PlayerColor color = move.color_;
    PointIndex indx = move.indx_;
//    FOO_PRINT_LINE("color = %d, indx = %d", (int)color, indx);
    if (!this->IsEye(move)) {
        real_eyes_[color].reset(indx);
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

        PointIndex oblq_indx = ins.GetIndex(oblq_pos);
        count = this->IsSelfPieceOrEye(Move(color, oblq_indx)) ?
                count + 1 : count;
    }

    static const PointIndex TABLE[3] = {3, 2, 1};
    real_eyes_[color][indx] = (TABLE[status] <= count);
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdtAdjPlblIndxsOfChn(PointIndex indx)
{
    FOO_ASSERT(board_.GetPoint(indx) != EMPTY_POINT);
#ifdef FOO_TEST
//    auto &ins = this->GetPosClcltr();
//    const Position &pos = ins.GetPos(indx);
//    FOO_PRINT_LINE("\nin UpdtAdj call.");
//    FOO_PRINT_LINE("x=%d, y=%d", pos.x_, pos.y_);
#endif
    PlayerColor color = board_.GetPoint(indx);
    const auto p_c = chain_sets_ + color;
    AirCount air_c = p_c->GetAirCountByPiece(indx);
    auto air_set = chain_sets_[color].GetAirSetByPiece(indx);
    auto c_playable = playable_indxs_ + color;
    auto oc_playable = playable_indxs_ + OppstColor(color);
    if (air_c == 1) {
        FOO_PRINT_LINE("air_c == 1.");
        PointIndex air_indx =
            GetLowest1<BLSq<BOARD_LEN>()>(air_set);
        Move move(color, air_indx);
        if (this->IsRealEye(move)) {
            c_playable->reset(air_indx);
            oc_playable->set(air_indx);
        } else if (this->IsFakeEye(move)) {
            c_playable->set(air_indx, !this->IsSuiside(move));
            oc_playable->set(air_indx);
        } else /* not eye */ {
            FOO_PRINT_LINE("else.");
            c_playable->set(air_indx, !this->IsSuiside(move));
            oc_playable->set(air_indx);
        }
    } else {
        auto not_real_eyes_ = ~real_eyes_[color];
        *c_playable = (*c_playable | air_set) & not_real_eyes_;
        *oc_playable &= ~(air_set & real_eyes_[color]); //bug!!!TODO
        auto notreal_in_airset = not_real_eyes_ & air_set;
        if (notreal_in_airset.count() > 0) {
            auto v = Get1s<BLSq<BOARD_LEN>()>(notreal_in_airset);

            for (PointIndex ele : v) {
                PlayerColor oc = OppstColor(color);
                oc_playable->set(ele, !this->IsSuiside(Move(oc, ele)));
            }
        }
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
    std::function<void(PointIndex)> Funs[3];

    Funs[color] = [](PointIndex) {};

    Funs[oc] = [this](PointIndex i) {
        this->UpdtAdjPlblIndxsOfChn(i);
    };

    Funs[EMPTY_POINT] = [this](PointIndex index) {
        FOO_PRINT_LINE("index = %d");
        if (!this->IsEmptySingly(index)) return;
        FOO_PRINT_LINE("is single empty.");

        for (int i=0; i<2; ++i) {
            if (this->IsSuiside(Move(i, index))) {
                playable_indxs_[i].reset(index);
            }
        }
    };

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        Point point = board_.GetPoint(adj_pos);
        PointIndex adj_indx = ins.GetIndex(adj_pos);
        Funs[point](adj_indx);
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
