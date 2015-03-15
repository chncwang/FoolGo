#ifndef FOOLGO_SRC_GAME_ENGINE_TLT_H_
#define FOOLGO_SRC_GAME_ENGINE_TLT_H_

#include <stdint.h>
#include <cmath>
#include <iterator>
#include <vector>

#include "../board/force.h"
#include "../board/full_board.h"
#include "../board/position.h"
#include "../def.h"
#include "../math/vector_util.h"
#include "engine.h"

namespace foolgo {
namespace game {

template<board::BoardLen BOARD_LEN>
inline void Engine<BOARD_LEN>::Init() {
  brdingm_.Init();
}

template<board::BoardLen BOARD_LEN>
void Engine<BOARD_LEN>::Play(board::PositionIndex nexti) {
  if (!GetPosClcltr().IsInBoard(nexti)) {
    return;
  }
  brdingm_.PlayMove(board::Move(next_player_, nexti));
  next_player_ = board::OppositeForce(next_player_);
}

template<board::BoardLen BOARD_LEN>
void Engine<BOARD_LEN>::Pass() {
  brdingm_.Pass(next_player_);
  next_player_ = board::OppositeForce(next_player_);
}

template<board::BoardLen BOARD_LEN>
board::PositionIndex Engine<BOARD_LEN>::NextMove() const {
  board::FullBoard<BOARD_LEN> root(zob_hasher_ptr_);
  root.Copy(brdingm_);
  int mc_count = 0;

  do {
    board::FullBoard<BOARD_LEN> cur_node(zob_hasher_ptr_);
    cur_node.Copy(root);
    std::vector<HashKey> path;

    while (true) {
      board::PositionIndex chld_i(0);
      if (IsEnd(cur_node)) {
        return BestChild(root);
      } else if (HasNewChild(cur_node, &chld_i)) {
        board::Force color = NextForce(cur_node);
        board::FullBoard<BOARD_LEN> child(zob_hasher_ptr_);
        child.Copy(cur_node);
        child.PlayMove(board::Move(color, chld_i));
        board::PositionIndex black = Simulator().Simulate(child);
        ++mc_count;
        board::PositionIndex result =
            child.LastForce() == board::BLACK_FORCE ?
                black : board::BoardLenSquare<BOARD_LEN>() - black;
        float profit =
            result > (board::BoardLenSquare<BOARD_LEN>() / 2.0) ? 1 : 0;
        Engine<BOARD_LEN>::TableItem item(1, profit);
        table_[child.HashKey()] = item;

        if (path.size() > 0) {
          for (auto it = path.end() - 1; it >= path.begin(); --it) {
            profit = 1 - profit;
            auto pathitem = table_[*it];
            auto t = pathitem.visited_times_;
            auto p = pathitem.avg_prft_;
            ++pathitem.visited_times_;
            pathitem.avg_prft_ = (p * t + profit) / (t + 1);
            table_[*it] = pathitem;
          }
        }
        break;
      } else {
        board::PositionIndex nexti = MaxUCBChild(cur_node);
        if (nexti == -1) {
          return -1;
        }
        board::Force cur_player = NextForce(cur_node);
        cur_node.PlayMove(board::Move(cur_player, nexti));
        path.push_back(cur_node.HashKey());
      }
    }
  } while (mc_count < Engine<BOARD_LEN>::SIMULATE_COUNT);

  return BestChild(root);
}

template<board::BoardLen BOARD_LEN>
HashKey Engine<BOARD_LEN>::ChildKey(
    const board::FullBoard<BOARD_LEN> &parent, board::PositionIndex indx) const {
  HashKey prnt_key = parent.HashKey();
  HashKey chldrn_key = table_[prnt_key].children_key_[indx];
  if (chldrn_key == Engine<BOARD_LEN>::NONE) {
    board::FullBoard<BOARD_LEN> b(zob_hasher_ptr_);
    b.Copy(parent);
    board::Force color = NextForce(b);
    b.PlayMove(board::Move(color, indx));
    HashKey key = b.HashKey();
    table_[prnt_key].children_key_[indx] = key;
    return key;
  } else {
    return chldrn_key;
  }
}

template<board::BoardLen BOARD_LEN>
bool Engine<BOARD_LEN>::HasNewChild(const board::FullBoard<BOARD_LEN> &node,
                                    board::PositionIndex *p_indx) const {
  board::Force color = NextForce(node);
  auto playable = PlayableIndexesConsideringKo(node, color);
  auto children =
      math::GetOnePositionIndexes<board::BoardLenSquare<BOARD_LEN>()>(playable);
  math::RandomizeVector(&children, math::GetTimeSeed());

  for (board::PositionIndex nexti : children) {
    HashKey ck = ChildKey(node, nexti);
    if (table_.find(ck) == table_.end()) {
      *p_indx = nexti;
      return true;
    }
  }

  return false;
}

template<board::BoardLen BOARD_LEN>
board::PositionIndex Engine<BOARD_LEN>::MaxUCBChild(
    const board::FullBoard<BOARD_LEN> &node) const {
  board::Force nextc = NextForce(node);
  auto playable = board::PlayableIndexesConsideringKo(node, nextc);
  auto playable_v = math::GetOnePositionIndexes<board::BoardLenSquare<BOARD_LEN>()>(playable);
  int32_t sum = 0;

  for (board::PositionIndex indx : playable_v) {
    sum += table_[ChildKey(node, indx)].visited_times_;
  }

  static const float INITIAL = -10000;
  float max_ucb(INITIAL);
  board::PositionIndex best(0);

  for (board::PositionIndex indx : playable_v) {
    auto item = table_[ChildKey(node, indx)];

    float ucb = item.avg_prft_ + sqrt(2 * log(sum) / item.visited_times_);
    if (ucb > max_ucb) {
      max_ucb = ucb;
      best = indx;
    }
  }

  return max_ucb > INITIAL ? best : -1;
}

template<board::BoardLen BOARD_LEN>
board::PositionIndex Engine<BOARD_LEN>::BestChild(
    const board::FullBoard<BOARD_LEN> &node) const {
  board::Force nextc = NextForce(node);
  auto playable = PlayableIndexesConsideringKo(node, nextc);
  auto playable_v = math::GetOnePositionIndexes<
      board::BoardLenSquare<BOARD_LEN>()>(playable);
  int most_visited = 0;
  board::PositionIndex result(-1);

  for (board::PositionIndex indx : playable_v) {
    auto item = table_[ChildKey(node, indx)];
    if (item.visited_times_ > most_visited) {
      most_visited = item.visited_times_;
      result = indx;
    }
  }

  return result;
}

}
}

#endif
