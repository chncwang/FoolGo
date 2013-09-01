#ifndef BOARD_IN_GM_TLT_H
#define BOARD_IN_GM_TLT_H

#include "board_in_gm.h"

#include <cstring>

#include <bitset>
#include <vector>
#include <algorithm>
#include <functional>

#include "bitset_util.h"
#include "vector_util.h"
#include "zob_hasher.h"


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::Init()
{
    Board<BOARD_LEN>::Init();
    for (int i=0; i<2; ++i) playable_indxs_[i].set();
    delegate_ = &ZobHasher<BOARD_LEN>::Ins();
    hash_key_ = delegate_->GetHash(*this);
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::Copy(const BoardInGm &b)
{
    Board<BOARD_LEN>::Copy(b);

    ko_indx_ = b.ko_indx_;
    last_player_ = b.last_player_;
    b_pcs_c_ = b.b_pcs_c_;
    hash_key_ = b.hash_key_;
    delegate_ = b.delegate_;

    for (int i=0; i<2; ++i) {
        playable_indxs_[i] = b.playable_indxs_[i];
        eye_sets_[i].Copy(b.eye_sets_[i]);
        chain_sets_[i].Copy(b.chain_sets_[i]);
    }
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::PlayMove(const Move &move)
{
    PlayerColor color = move.color_;
    PointIndex indx = move.indx_;
    ASSERT(this->GetPosClcltr().IsInBoard(indx));
    ASSERT(this->GetPoint(indx) == EMPTY_POINT);
    BrdChange change;
    change.Init(this->LastPlayer(), this->KoIndex());
    b_pcs_c_ += OppstColor(color);

    for (int i=0; i<2; ++i) {
        playable_indxs_[i].reset(indx);
        eye_sets_[i].SetEye(indx, true);
        eye_sets_[i].SetRealEye(indx, false);
    }

    PlayerColor oc = OppstColor(color);
    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);
    typename BoardInGm<BOARD_LEN>::PointIndxVector ate_points[4], sssdv;

    this->PlayBasicMove(move, ate_points, &sssdv);

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;
        if (this->GetPoint(adj_pos) != EMPTY_POINT) continue;

        PointIndex adj_indx = ins.GetIndex(adj_pos);
        this->UpdateEye(Move(color, adj_indx));
    }

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;
        if (this->GetPoint(adj_pos) != EMPTY_POINT) continue;

        PointIndex adj_indx = ins.GetIndex(adj_pos);
        Move adj_move(color, adj_indx);
        this->UpdateRealEye(adj_move);
        if (IsFakeEye(eye_sets_[adj_move.color_], adj_move.indx_)) {
            playable_indxs_[oc][adj_indx] =
                    this->IsSuiside(Move(oc, adj_indx));
        }
    }

    for (int i=0; i<4; ++i) {
        Position oblq_pos = pos.ObliquePos(i);
        if (!ins.IsInBoard(oblq_pos)) continue;
        if (this->GetPoint(oblq_pos) != EMPTY_POINT) continue;

        PointIndex oblq_indx = ins.GetIndex(oblq_pos);
        Move oblq_move(color, oblq_indx);
        this->UpdateRealEye(oblq_move);
    }

//    PRINT_LINE(" ");
//    this->PRINT_BOARD();
    this->UpdtPlblIndxsArnd(move);
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

    if (this->GetPoint(indx) == EMPTY_POINT) {
        change.SetNow(ko_indx_, indx, true, sssdv);
    } else {
        auto ates = ConcatVectors(ate_points);
        change.SetNow(ko_indx_, indx, false, ates);
    }

    hash_key_ = delegate_->GetHash(hash_key_, change);
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::Pass(PlayerColor color)
{
    last_player_ = color;
    ko_indx_ = BoardInGm<BOARD_LEN>::NONE;
    hash_key_ = delegate_->GetHash(*this);
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::LetAdjChnsSetAir(PointIndex indx, bool v)
{
    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        PointIndex adj_i = ins.GetIndex(adj_pos);
        Point pnt = this->GetPoint(adj_i);
        if (pnt == EMPTY_POINT) continue;

        chain_sets_[pnt].SetAir(adj_i, indx, v);
    }
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsSelfPieceOrEye(const Move &move) const
{
    PointIndex indx = move.indx_;
    PlayerColor color = move.color_;
    return this->GetPoint(indx) == color || eye_sets_[color].IsEye(indx);
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsEmptySingly(PointIndex indx) const
{
    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        if (this->GetPoint(adj_pos) == EMPTY_POINT) return false;
    }

    return true;
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsSuiside(const Move &move) const
{
    ASSERT(this->GetPoint(move.indx_) == EMPTY_POINT);

    auto &pc = this->GetPosClcltr();
    PointIndex indx = move.indx_;
    const Position &pos = pc.GetPos(indx);
    PlayerColor color = move.color_;
    PlayerColor oc = OppstColor(color);

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!pc.IsInBoard(adj_pos)) continue;

        Point point = this->GetPoint(adj_pos);
        PointIndex adj_indx = pc.GetIndex(adj_pos);
        if (point == EMPTY_POINT) {
            return false;
        } else if (point == color) {
            if (chain_sets_[color].GetAirCountByPiece(adj_indx) > 1) {
                return false;
            }
        } else {
            if (chain_sets_[oc].GetAirCountByPiece(adj_indx) == 1) return false;
        }
    }

    return true;
}


template <BoardLen BOARD_LEN>
bool BoardInGm<BOARD_LEN>::IsPlayable(const Move &move) const
{
    return playable_indxs_[move.color_][move.indx_];
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::PlayBasicMove(
        const Move &move,
        typename BoardInGm<BOARD_LEN>::PointIndxVector *v,
        typename BoardInGm<BOARD_LEN>::PointIndxVector *sssdv)
{
    ASSERT(v != nullptr);
    PlayerColor color = move.color_;
    PointIndex indx = move.indx_;
    ASSERT(this->GetPoint(indx) == EMPTY_POINT);

    std::bitset<BoardLenSquare<BOARD_LEN>()> air_set;
    PlayerColor oc = OppstColor(color);
    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        PointIndex adj_indx = ins.GetIndex(adj_pos);
        if (this->GetPoint(adj_indx) == oc &&
                chain_sets_[oc].GetAirCountByPiece(adj_indx) == 1) {
            v[i] = RemoveChain(Move(oc, adj_indx));
        }
        if (this->GetPoint(adj_pos) == EMPTY_POINT) {
            air_set.set(adj_indx);
        }
    }

    this->SetPoint(indx, color);
    this->LetAdjChnsSetAir(indx, false);
    chain_sets_[color].AddPiece(indx, air_set);
    if (chain_sets_[color].GetAirCountByPiece(indx) == 0) {
        *sssdv = this->RemoveChain(move);
    }
}


template <BoardLen BOARD_LEN>
typename BoardInGm<BOARD_LEN>::PointIndxVector
BoardInGm<BOARD_LEN>::RemoveChain(const Move &move)
{
    auto p_chnset = chain_sets_ + move.color_;
    auto trp = p_chnset->GetPieces(move.indx_);
    chain_sets_[move.color_].RemoveListByPiece(move.indx_);

    for (PointIndex indx : trp) {
        this->SetPoint(indx, EMPTY_POINT);
        for (int i=0; i<2; ++i) playable_indxs_[i].set(indx);
    }

    for (PointIndex indx : trp) this->LetAdjChnsSetAir(indx, true);

    b_pcs_c_ -= trp.size() * OppstColor(move.color_);
    return trp;
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdateEye(const Move &move)
{
    ASSERT(this->GetPoint(move.indx_) == EMPTY_POINT);
    auto &ins = this->GetPosClcltr();
    PointIndex indx = move.indx_;
    const Position &pos = ins.GetPos(indx);
    PlayerColor color = move.color_;

    for (int i=0; i<4; ++i) {
        const Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) {
            continue;
        } else if (this->GetPoint(pos.AdjcntPos(i)) != color) {
            eye_sets_[color].SetEye(indx, false);
            return;
        }
    }

    eye_sets_[color].SetEye(indx, true);

    for (int i=0; i<4; ++i) {
        const Position oblq_pos = pos.ObliquePos(i);
        if (!ins.IsInBoard(oblq_pos)) continue;

        PointIndex oblq_indx = ins.GetIndex(oblq_pos);
        if (this->GetPoint(oblq_indx) == EMPTY_POINT) {
            this->UpdateRealEye(Move(color, oblq_indx));
        }
    }
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdateRealEye(const Move &move)
{
    ASSERT(this->GetPoint(move.indx_) == EMPTY_POINT);
    PlayerColor color = move.color_;
    PointIndex indx = move.indx_;
//    PRINT_LINE("color = %d, indx = %d", (int)color, indx);
    if (!eye_sets_[color].IsEye(indx)) {
        eye_sets_[color].SetRealEye(indx, false);
        return;
    }

    auto &ins = this->GetPosClcltr();
    const Position &pos = ins.GetPos(indx);
    auto status = ins.GetPosStatus(pos);
//    PRINT_LINE("status = %d", status);
    PointIndex count = 0;

    for (int i=0; i<4; ++i) {
        Position oblq_pos = pos.ObliquePos(i);
        if (!ins.IsInBoard(oblq_pos)) continue;

        PointIndex oblq_indx = ins.GetIndex(oblq_pos);
        count = this->IsSelfPieceOrEye(Move(color, oblq_indx)) ?
                count + 1 : count;
    }

    static const PointIndex TABLE[3] = {3, 2, 1};
    eye_sets_[color].SetRealEye(indx, TABLE[status] <= count);
    playable_indxs_[color].reset(indx);
    playable_indxs_[OppstColor(color)].reset(indx);
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdtAdjPlblIndxsOfChn(PointIndex indx)
{
    ASSERT(this->GetPoint(indx) != EMPTY_POINT);
    PlayerColor color = this->GetPoint(indx);
    const auto p_c = chain_sets_ + color;
    AirCount air_c = p_c->GetAirCountByPiece(indx);
    auto air_set = chain_sets_[color].GetAirSetByPiece(indx);
    auto c_playable = playable_indxs_ + color;
    auto oc_playable = playable_indxs_ + OppstColor(color);
    if (air_c == 1) {
//        PRINT_LINE("air_c == 1.");
        PointIndex air_indx =
            GetLowest1<BoardLenSquare<BOARD_LEN>()>(air_set);
        Move move(color, air_indx);
        if (eye_sets_[color].IsRealEye(air_indx)) {
            c_playable->reset(air_indx);
            oc_playable->set(air_indx);
        } else if (IsFakeEye(eye_sets_[color], air_indx)) {
            c_playable->set(air_indx);
            oc_playable->set(air_indx);
        } else /* not eye */ {
//            PRINT_LINE("else.");
            c_playable->set(air_indx);
            oc_playable->set(air_indx);
        }
    } else {
//        PRINT_LINE("else");
        const auto &real_eyes = eye_sets_[color].GetRealEyes();
        auto not_real_eyes = ~real_eyes;
        *c_playable = (*c_playable | air_set) & not_real_eyes;
        *oc_playable &= ~(air_set & real_eyes);
    }
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::UpdtPlblIndxsArnd(const Move &move)
{
    PointIndex indx = move.indx_;
    if (this->GetPoint(indx) != EMPTY_POINT) this->UpdtAdjPlblIndxsOfChn(indx);

    auto &ins = this->GetPosClcltr();
    Position pos = ins.GetPos(indx);
    PlayerColor color = move.color_;
    PlayerColor oc = OppstColor(color);
    std::function<void(PointIndex)> Funs[3];

    Funs[color] = [](PointIndex) {
    };

    Funs[oc] = [this](PointIndex i) {
//        PRINT_LINE("in lambda, i = %d", i);
        this->UpdtAdjPlblIndxsOfChn(i);
    };

    Funs[EMPTY_POINT] = [this](PointIndex index) {
    };

    for (int i=0; i<4; ++i) {
        Position adj_pos = pos.AdjcntPos(i);
        if (!ins.IsInBoard(adj_pos)) continue;

        Point point = this->GetPoint(adj_pos);
        PointIndex adj_indx = ins.GetIndex(adj_pos);
//        PRINT_LINE("point = %d", point);
//        PRINT_LINE("i = %d", i);
        Funs[point](adj_indx);
//        PRINT_LINE("here.");
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

            if (this->GetPoint(adj_pos) == OppstColor(ate_color)) {
                PointIndex adj_indx = ins.GetIndex(adj_pos);
                this->UpdtAdjPlblIndxsOfChn(adj_indx);
            }
        }
    }
}


#endif
