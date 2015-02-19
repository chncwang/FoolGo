#ifndef PIECES_STR_SET_INL_H
#define PIECES_STR_SET_INL_H

#include <chain_set.h>
#include <pos_cal.h>
#include <position.h>
#include <bitset>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <boost/format.hpp>

using boost::format;

#define IS_POINT_NOT_EMPTY(piece_i) \
    (this->GetListHead(piece_i) != ChainSet<BOARD_LEN>::NONE_LIST)

template<BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::Copy(const ChainSet<BOARD_LEN> &c) {
  memcpy(nodes_, c.nodes_, sizeof(nodes_));
  for (int i = 0; i < BoardLenSquare<BOARD_LEN>(); ++i) {
    lists_[i] = c.lists_[i];
  }
}

template<BoardLen BOARD_LEN>
typename ChainSet<BOARD_LEN>::AirSet
inline ChainSet<BOARD_LEN>::GetAirSetByPiece(PointIndex piece_i) const {
  return GetAirSetOfChain(nodes_[piece_i].list_head_);
}

template<BoardLen BOARD_LEN>
inline AirCount ChainSet<BOARD_LEN>::GetAirCountByPiece(
    PointIndex piece_i) const {
  assert(IS_POINT_NOT_EMPTY(piece_i));
  return this->GetAirCountOfChain(this->GetListHead(piece_i));
}

template<BoardLen BOARD_LEN>
typename ChainSet<BOARD_LEN>::PntIndxVector
inline ChainSet<BOARD_LEN>::GetPieces(PointIndex piece_i) const {
  assert(IS_POINT_NOT_EMPTY(piece_i));
  return this->GetPiecesOfChain(this->GetListHead(piece_i));
}

template<BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::SetAir(PointIndex indx, PointIndex air_i, bool v) {
  AirSet air_set;
  if (!v)
    air_set.set();
  air_set[air_i] = v;
  PointIndex head = this->GetListHead(indx);
  assert(head != ChainSet<BOARD_LEN>::NONE_LIST);
  List *pl = lists_ + head;
  const AirSet &r_as = pl->air_set_;
  pl->air_set_ = v ? (r_as | air_set) : (r_as & air_set);
  pl->air_count_ = pl->air_set_.count();
}

template<BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::AddPiece(PointIndex indx,
                                   const ChainSet<BOARD_LEN>::AirSet &air_set) {
  const PosCalculator<BOARD_LEN> &ins = this->GetPosClcltr();
  this->CreateList(indx, air_set);
  PointIndex list_i = indx;
  const Position &pos = ins.GetPos(indx);

  for (int i = 0; i < 4; ++i) {
    Position adj_pos = pos.AdjcntPos(i);
    if (ins.IsInBoard(adj_pos)) {
      PointIndex adj_i = ins.GetIndex(adj_pos);
      PointIndex adj_list = this->GetListHead(adj_i);
      if (adj_list == ChainSet<BOARD_LEN>::NONE_LIST) {
        continue;
      }
      if (adj_list == list_i) {
        continue;
      }

      list_i = this->MergeLists(list_i, adj_list);
    }
  }
}

template<BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::RemoveListByPiece(PointIndex piece_i) {
  assert(IS_POINT_NOT_EMPTY(piece_i));
  PointIndex list_i = this->GetListHead(piece_i);
  this->RemoveList(list_i);
}

template<BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::CreateList(
    PointIndex node_i, const ChainSet<BOARD_LEN>::AirSet &air_set) {
  nodes_[node_i].list_head_ = node_i;
  List *list = lists_ + node_i;
  list->tail_ = node_i;
  list->len_ = 1;
  list->air_set_ = air_set;
  list->air_count_ = air_set.count();
}

template<BoardLen BOARD_LEN>
PointIndex ChainSet<BOARD_LEN>::MergeLists(PointIndex head_a,
                                           PointIndex head_b) {
  assert(head_a != head_b);
  if (lists_[head_a].len_ > lists_[head_b].len_)
    std::swap(head_a, head_b);

  for (int i = head_a;; i = nodes_[i].next_) {
    nodes_[i].list_head_ = head_b;
    if (i == lists_[head_a].tail_) {
      break;
    }
    assert(i != nodes_[i].next_);
  }

  List *list_b = lists_ + head_b;
  nodes_[list_b->tail_].next_ = head_a;
  list_b->tail_ = lists_[head_a].tail_;
  list_b->len_ += lists_[head_a].len_;
  list_b->air_set_ |= lists_[head_a].air_set_;
  list_b->air_count_ = list_b->air_set_.count();
  return head_b;
}

template<BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::RemoveList(PointIndex head) {
  for (int i = head;; i = nodes_[i].next_) {
    nodes_[i].list_head_ = ChainSet<BOARD_LEN>::NONE_LIST;
    if (i == lists_[head].tail_)
      break;
  }
}

template<BoardLen BOARD_LEN>
typename ChainSet<BOARD_LEN>::AirSet
inline ChainSet<BOARD_LEN>::GetAirSetOfChain(PointIndex head) const {
  return lists_[head].air_set_;
}

template<BoardLen BOARD_LEN>
inline AirCount ChainSet<BOARD_LEN>::GetAirCountOfChain(
    PointIndex list_i) const {
  return lists_[list_i].air_count_;
}

template<BoardLen BOARD_LEN>
typename ChainSet<BOARD_LEN>::PntIndxVector ChainSet<BOARD_LEN>::GetPiecesOfChain(
    PointIndex list_i) const {
  auto pl = lists_ + list_i;
  ChainSet<BOARD_LEN>::PntIndxVector v(pl->len_);
  int vi = 0;

  for (int i = list_i;; i = nodes_[i].next_) {
    v[vi++] = i;
    if (i == pl->tail_) {
      break;
    }
  }

  return move(v);
}

template<BoardLen BOARD_LEN>
std::ostream &operator <<(std::ostream &os,
                          const ChainSet<BOARD_LEN> &chain_set) {
  PosCalculator<BOARD_LEN> &ins = chain_set.GetPosClcltr();
  bool lists_exist[BoardLenSquare<BOARD_LEN>()];
  memset(lists_exist, false, sizeof(lists_exist));

  for (int y = 0; y < BOARD_LEN; ++y) {
    for (int x = 0; x < BOARD_LEN; ++x) {
      int index = ins.GetIndex(Position(x, y));
      int n = chain_set.nodes_[index].next_;
      const Position &np = ins.GetPos(n);
      int nx = np.x_;
      int ny = np.y_;
      os << format("{n=%1%,%2% ") % nx % ny;
      int h = chain_set.nodes_[index].list_head_;
      if (h == ChainSet<BOARD_LEN>::NONE_LIST) {
        os << "h=x,x} ";
      } else {
        lists_exist[h] = true;
        const Position &hp = ins.GetPos(h);
        int hx = hp.x_;
        int hy = hp.y_;
        os << format("h=%1%,%2%}") % hx % hy;
      }
    }

    os << "\n\n";
  }

  for (int i = 0; i < BoardLenSquare<BOARD_LEN>(); ++i) {
    if (lists_exist[i]) {
      const Position &pos = ins.GetPos(i);
      const Position &tp = ins.GetPos(chain_set.lists_[i].tail_);
      os
          << format(
              "(%1%, %2%): tail_ = (%3%, %4%), len_ = %5%, air_count - %6%\n")
              % pos.x_ % pos.y_ % tp.x_ % tp.y_ % chain_set.lists_[i].len_
              % chain_set.lists_[i].air_set_.count();
    }
  }

  return os;
}

#endif
