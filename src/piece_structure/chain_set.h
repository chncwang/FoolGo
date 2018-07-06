#ifndef FOOLGO_SRC_PIECE_STRUCTURE_CHAIN_SET_H_
#define FOOLGO_SRC_PIECE_STRUCTURE_CHAIN_SET_H_

#include <boost/format.hpp>
#include <bitset>
#include <cassert>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <vector>

#include "../board/pos_cal.h"
#include "../board/position.h"
#include "../def.h"

namespace foolgo {
namespace piece_structure {

typedef char AirCount;

template<board::BoardLen BOARD_LEN>
class ChainSet {
 public:
  ChainSet() = default;
  ~ChainSet() = default;
  void Copy(const ChainSet &c);

  board::BitSet<BOARD_LEN> GetAirSetByPiece(board::PositionIndex piece_i) const;
  AirCount GetAirCount(board::PositionIndex piece_i) const;
  std::vector<board::PositionIndex> GetPieces(
      board::PositionIndex piece_i) const;

  void SetAir(board::PositionIndex indx, board::PositionIndex air_i, bool v);
  void AddPiece(board::PositionIndex indx,
                const board::BitSet<BOARD_LEN> &air_set);
  void RemoveListByPiece(board::PositionIndex piece_i);

 private:
  static const board::PositionIndex NONE_LIST = -1;

  struct Node {
    board::PositionIndex next_ = 0, list_head_;
    Node()
        : list_head_(NONE_LIST) {
    }
  } nodes_[board::BoardLenSquare<BOARD_LEN>()];

  struct List {
    board::PositionIndex tail_, len_;
    board::BitSet<BOARD_LEN> air_set_;
    AirCount air_count_;
  } lists_[board::BoardLenSquare<BOARD_LEN>()];

  inline board::PstionAndIndxCcltr<BOARD_LEN> &GetPosClcltr() const {
    return board::PstionAndIndxCcltr<BOARD_LEN>::Ins();
  }

  inline board::PositionIndex GetListHead(board::PositionIndex node_i) const {
    return nodes_[node_i].list_head_;
  }
  void CreateList(board::PositionIndex node_i,
                  const board::BitSet<BOARD_LEN> &air_set);

  board::PositionIndex MergeLists(board::PositionIndex head_a,
                               board::PositionIndex head_b);

  void RemoveList(board::PositionIndex head);

  board::BitSet<BOARD_LEN> GetAirSetOfChain(board::PositionIndex head) const;
  AirCount GetAirCountOfChain(board::PositionIndex list_i) const;
  std::vector<board::PositionIndex> GetPiecesOfChain(
      board::PositionIndex list_i) const;
};

#define IS_POINT_NOT_EMPTY(piece_i) \
    (GetListHead(piece_i) != ChainSet<BOARD_LEN>::NONE_LIST)

template<board::BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::Copy(const ChainSet<BOARD_LEN> &c) {
  for (int i = 0; i < board::BoardLenSquare<BOARD_LEN>(); ++i) {
    lists_[i] = c.lists_[i];
    nodes_[i] = c.nodes_[i];
  }
}

template<board::BoardLen BOARD_LEN>
board::BitSet<BOARD_LEN> ChainSet<BOARD_LEN>::GetAirSetByPiece(
    board::PositionIndex piece_i) const {
  return GetAirSetOfChain(nodes_[piece_i].list_head_);
}

template<board::BoardLen BOARD_LEN>
inline AirCount ChainSet<BOARD_LEN>::GetAirCount(
    board::PositionIndex piece_i) const {
  assert(IS_POINT_NOT_EMPTY(piece_i));
  return GetAirCountOfChain(GetListHead(piece_i));
}

template<board::BoardLen BOARD_LEN>
std::vector<board::PositionIndex> ChainSet<BOARD_LEN>::GetPieces(
    board::PositionIndex piece_i) const {
  assert(IS_POINT_NOT_EMPTY(piece_i));
  return GetPiecesOfChain(GetListHead(piece_i));
}

template<board::BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::SetAir(board::PositionIndex indx,
                                 board::PositionIndex air_i, bool v) {
  board::BitSet<BOARD_LEN> air_set;
  if (!v)
    air_set.set();
  air_set[air_i] = v;
  board::PositionIndex head = GetListHead(indx);
  assert(head != ChainSet<BOARD_LEN>::NONE_LIST);
  List *pl = lists_ + head;
  const board::BitSet<BOARD_LEN> &r_as = pl->air_set_;
  pl->air_set_ = v ? (r_as | air_set) : (r_as & air_set);
  pl->air_count_ = pl->air_set_.count();
}

template<board::BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::AddPiece(board::PositionIndex indx,
                                   const board::BitSet<BOARD_LEN> &air_set) {
  const board::PstionAndIndxCcltr<BOARD_LEN> &ins = GetPosClcltr();
  CreateList(indx, air_set);
  board::PositionIndex list_i = indx;
  const board::Position &pos = ins.GetPosition(indx);

  for (int i = 0; i < 4; ++i) {
    board::Position adj_pos = AdjacentPosition(pos, i);
    if (ins.IsInBoard(adj_pos)) {
      board::PositionIndex adj_i = ins.GetIndex(adj_pos);
      board::PositionIndex adj_list = GetListHead(adj_i);
      if (adj_list == ChainSet<BOARD_LEN>::NONE_LIST) {
        continue;
      }
      if (adj_list == list_i) {
        continue;
      }

      list_i = MergeLists(list_i, adj_list);
    }
  }
}

template<board::BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::RemoveListByPiece(board::PositionIndex piece_i) {
  assert(IS_POINT_NOT_EMPTY(piece_i));
  board::PositionIndex list_i = GetListHead(piece_i);
  RemoveList(list_i);
}

template<board::BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::CreateList(
    board::PositionIndex node_i, const board::BitSet<BOARD_LEN> &air_set) {
  nodes_[node_i].list_head_ = node_i;
  List *list = lists_ + node_i;
  list->tail_ = node_i;
  list->len_ = 1;
  list->air_set_ = air_set;
  list->air_count_ = air_set.count();
}

template<board::BoardLen BOARD_LEN>
board::PositionIndex ChainSet<BOARD_LEN>::MergeLists(board::PositionIndex head_a,
                                                  board::PositionIndex head_b) {
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

template<board::BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::RemoveList(board::PositionIndex head) {
  for (int i = head;; i = nodes_[i].next_) {
    nodes_[i].list_head_ = ChainSet<BOARD_LEN>::NONE_LIST;
    if (i == lists_[head].tail_)
      break;
  }
}

template<board::BoardLen BOARD_LEN>
board::BitSet<BOARD_LEN> ChainSet<BOARD_LEN>::GetAirSetOfChain(
    board::PositionIndex head) const {
  return lists_[head].air_set_;
}

template<board::BoardLen BOARD_LEN>
inline AirCount ChainSet<BOARD_LEN>::GetAirCountOfChain(
    board::PositionIndex list_i) const {
  return lists_[list_i].air_count_;
}

template<board::BoardLen BOARD_LEN>
std::vector<board::PositionIndex> ChainSet<BOARD_LEN>::GetPiecesOfChain(
    board::PositionIndex list_i) const {
  auto pl = lists_ + list_i;
  std::vector<board::PositionIndex> v(pl->len_);
  int vi = 0;

  for (int i = list_i;; i = nodes_[i].next_) {
    v[vi++] = i;
    if (i == pl->tail_) {
      break;
    }
  }

  return move(v);
}

template<board::BoardLen BOARD_LEN>
std::ostream &operator <<(std::ostream &os,
                          const ChainSet<BOARD_LEN> &chain_set) {
  board::PstionAndIndxCcltr<BOARD_LEN> &ins = chain_set.GetPosClcltr();
  bool lists_exist[board::BoardLenSquare<BOARD_LEN>()];
  memset(lists_exist, false, sizeof(lists_exist));

  for (int y = 0; y < BOARD_LEN; ++y) {
    for (int x = 0; x < BOARD_LEN; ++x) {
      int index = ins.GetIndex(board::Position(x, y));
      int n = chain_set.nodes_[index].next_;
      const board::Position &np = ins.GetPos(n);
      int nx = np.x;
      int ny = np.y;
      os << boost::format("{n=%1%,%2% ") % nx % ny;
      int h = chain_set.nodes_[index].list_head_;
      if (h == ChainSet<BOARD_LEN>::NONE_LIST) {
        os << "h=x,x} ";
      } else {
        lists_exist[h] = true;
        const board::Position &hp = ins.GetPos(h);
        int hx = hp.x;
        int hy = hp.y;
        os << boost::format("h=%1%,%2%}") % hx % hy;
      }
    }

    os << "\n\n";
  }

  for (int i = 0; i < board::BoardLenSquare<BOARD_LEN>(); ++i) {
    if (lists_exist[i]) {
      const board::Position &pos = ins.GetPos(i);
      const board::Position &tp = ins.GetPos(chain_set.lists_[i].tail_);
      os
          << boost::format(
              "(%1%, %2%): tail_ = (%3%, %4%), len_ = %5%, air_count - %6%\n")
              % pos.x % pos.y % tp.x % tp.y % chain_set.lists_[i].len_
              % chain_set.lists_[i].air_set_.count();
    }
  }

  return os;
}
}
}

#endif
