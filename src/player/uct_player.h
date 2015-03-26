#ifndef FOOLGO_SRC_PLAYER_UCT_PLAYER_H_
#define FOOLGO_SRC_PLAYER_UCT_PLAYER_H_

#include <log4cplus/logger.h>
#include <cassert>
#include <cmath>
#include <cstdint>

#include "../board/force.h"
#include "../board/full_board.h"
#include "../board/position.h"
#include "../game/monte_carlo_game.h"
#include "node_record.h"
#include "passable_player.h"
#include "transposition_table.h"

namespace foolgo {
namespace player {

template<board::BoardLen BOARD_LEN>
class UctPlayer : public PassablePlayer<BOARD_LEN> {
 public:
  UctPlayer(uint32_t seed, int mc_game_count_per_move = 10000);

 protected:
  board::PositionIndex NextMoveWithPlayableBoard(
      const board::FullBoard<BOARD_LEN> &full_board);

 private:
  int mc_game_count_per_move_;
  TranspositionTable<BOARD_LEN> transposition_table_;
  uint32_t seed_;
  static log4cplus::Logger logger_;

  board::PositionIndex MaxUcbChild(
      const board::FullBoard<BOARD_LEN> &full_board);
  float ModifyAverageProfitAndReturnNewProfit(
      board::FullBoard<BOARD_LEN> *full_board_ptr, int *mc_game_count_ptr);
  board::PositionIndex BestChild(const board::FullBoard<BOARD_LEN> &full_board);
};

template<board::BoardLen BOARD_LEN>
log4cplus::Logger UctPlayer<BOARD_LEN>::logger_ =
    log4cplus::Logger::getInstance("foolgo.player.UctPlayer");

namespace {

float Ucb(const NodeRecord &node_record, int visited_count_sum) {
  return node_record.GetAverageProfit()
      + sqrt(2 * log(visited_count_sum) / node_record.GetVisitedTime());
}

template<board::BoardLen BOARD_LEN>
float GetRegionRatio(const board::FullBoard<BOARD_LEN> &full_board,
                board::Force force) {
  int black_region = full_board.BlackRegion();
  float black_ratio = static_cast<float>(black_region)
      / board::BoardLenSquare<BOARD_LEN>();
  return force == board::Force::BLACK_FORCE ? black_ratio : 1 - black_ratio;
}

}

template<board::BoardLen BOARD_LEN>
UctPlayer<BOARD_LEN>::UctPlayer(uint32_t seed, int mc_game_count_per_move)
    : seed_(seed),
      mc_game_count_per_move_(mc_game_count_per_move) {}

template<board::BoardLen BOARD_LEN>
board::PositionIndex UctPlayer<BOARD_LEN>::NextMoveWithPlayableBoard(
      const board::FullBoard<BOARD_LEN> &full_board) {
  int current_mc_game_count = 0;

  while (current_mc_game_count < mc_game_count_per_move_) {
    board::PositionIndex max_ucb_index = MaxUcbChild(full_board);
    board::FullBoard<BOARD_LEN> max_ucb_child;
    max_ucb_child.Copy(full_board);
    board::Play(&max_ucb_child, max_ucb_index);
    ModifyAverageProfitAndReturnNewProfit(&max_ucb_child,
                                          &current_mc_game_count);
  }

  return BestChild(full_board);
}

template<board::BoardLen BOARD_LEN>
board::PositionIndex UctPlayer<BOARD_LEN>::MaxUcbChild(
    const board::FullBoard<BOARD_LEN> &full_board) {
  board::Force current_force = board::NextForce(full_board);
  auto playable_index_vector = full_board.PlayableIndexes(current_force);

  assert(!playable_index_vector.empty());

  int visited_count_sum = 0;

  for (board::PositionIndex position_index : playable_index_vector) {
    const NodeRecord *node_record_ptr = transposition_table_.GetChild(
        full_board, position_index);
    if (node_record_ptr == nullptr) {
      return position_index;
    }

    visited_count_sum += node_record_ptr->GetVisitedTime();
  }

  float max_ucb = -1.0f;
  board::PositionIndex max_ucb_index = board::POSITION_INDEX_PASS;

  for (board::PositionIndex position_index : playable_index_vector) {
    const NodeRecord *node_record_ptr = transposition_table_.GetChild(
        full_board, position_index);
    // It is guaranteed by the above loop that node_record_ptr is not nullptr.
    float ucb = Ucb(*node_record_ptr, visited_count_sum);
    if (ucb > max_ucb
        && !full_board.IsSuicide(
            board::Move(board::NextForce(full_board), position_index))) {
      max_ucb = ucb;
      max_ucb_index = position_index;
    }
  }

  return max_ucb_index;
}

template<board::BoardLen BOARD_LEN>
float UctPlayer<BOARD_LEN>::ModifyAverageProfitAndReturnNewProfit(
    board::FullBoard<BOARD_LEN> *full_board_ptr, int *mc_game_count_ptr) {
  float new_profit;
  NodeRecord *node_record_ptr = transposition_table_.Get(*full_board_ptr);

  if (node_record_ptr == nullptr) {
    game::MonteCarloGame<BOARD_LEN> monte_carlo_game(*full_board_ptr, seed_);
    if (!full_board_ptr->IsEnd()) {
      monte_carlo_game.Run();
    }
    ++(*mc_game_count_ptr);
    board::Force force = full_board_ptr->LastForce();
    new_profit = GetRegionRatio(monte_carlo_game.GetFullBoard(), force);
    player::NodeRecord node_record(1, new_profit);
    transposition_table_.Insert(*full_board_ptr, node_record);
  } else {
    if (full_board_ptr->IsEnd()) {
      ++(*mc_game_count_ptr);
      new_profit = node_record_ptr->GetAverageProfit();
    } else {
      if (full_board_ptr->PlayableIndexes(board::NextForce(*full_board_ptr))
          .empty()) {
        full_board_ptr->Pass(board::NextForce(*full_board_ptr));
      } else {
        board::PositionIndex max_ucb_index = MaxUcbChild(*full_board_ptr);
        LOG4CPLUS_DEBUG(
            logger_,
            "max_ucb_index:" <<
            board::PositionToString(board::PstionAndIndxCcltr<BOARD_LEN>::Ins().
                                    GetPosition(max_ucb_index)) <<
            "full_board:" << *full_board_ptr);
        board::Play(full_board_ptr, max_ucb_index);
      }
      new_profit = 1.0f
          - ModifyAverageProfitAndReturnNewProfit(full_board_ptr,
                                                  mc_game_count_ptr);
      float previous_profit = node_record_ptr->GetAverageProfit();
      float modified_profit = (previous_profit
          * node_record_ptr->GetVisitedTime() + new_profit)
          / (node_record_ptr->GetVisitedTime() + 1);
      node_record_ptr->SetAverageProfit(modified_profit);
    }

    node_record_ptr->SetVisitedTimes(node_record_ptr->GetVisitedTime() + 1);
  }

  return new_profit;
}

template<board::BoardLen BOARD_LEN>
board::PositionIndex UctPlayer<BOARD_LEN>::BestChild(
    const board::FullBoard<BOARD_LEN> &full_board) {
  board::Force force = board::NextForce(full_board);
  auto playable_index_vector = full_board.PlayableIndexes(force);
  int max_visited_count = -1;
  board::PositionIndex most_visited_index;

  for (board::PositionIndex index : playable_index_vector) {
    const NodeRecord *node_record = transposition_table_.GetChild(full_board,
                                                                  index);
    assert(node_record != nullptr);
    if (node_record->GetVisitedTime() > max_visited_count) {
      max_visited_count = node_record->GetVisitedTime();
      most_visited_index = index;
    }
  }

  return most_visited_index;
}

}
}

#endif /* FOOLGO_SRC_PLAYER_UCT_PLAYER_H_ */
