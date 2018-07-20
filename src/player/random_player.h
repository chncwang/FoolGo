#ifndef FOOLGO_SRC_PLAYER_RANDOM_PLAYER_H_
#define FOOLGO_SRC_PLAYER_RANDOM_PLAYER_H_

#include <cassert>
#include <cstdint>
#include <iostream>

#include "board/full_board.h"
#include "board/position.h"
#include "util/rand.h"
#include "passable_player.h"

namespace foolgo {

template<BoardLen BOARD_LEN> class FullBoard;
struct Move;

template <BoardLen BOARD_LEN>
class RandomPlayer : public PassablePlayer<BOARD_LEN> {
 public:
  explicit RandomPlayer(uint32_t seed) : seed_(seed) {}

 protected:
  PositionIndex NextMoveWithPlayableBoard(
      const FullBoard<BOARD_LEN> &full_board);

 private:
  uint32_t seed_;
};

template<BoardLen BOARD_LEN>
PositionIndex RandomPlayer<BOARD_LEN>::NextMoveWithPlayableBoard(
    const FullBoard<BOARD_LEN>& full_board) {
  auto playable_indexes = full_board.PlayableIndexes(NextForce(full_board));
  assert(!playable_indexes.empty());

  PositionIndex rand = Rand(playable_indexes.size() - 1, seed_);
  return playable_indexes.at(rand);
}

} /* namespace foolgo */

#endif
