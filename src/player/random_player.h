#ifndef FOOLGO_SRC_PLAYER_RANDOM_PLAYER_H_
#define FOOLGO_SRC_PLAYER_RANDOM_PLAYER_H_

#include <cassert>
#include <cstdint>

#include "board/full_board.h"
#include "board/position.h"
#include "util/rand.h"
#include "passable_player.h"

namespace foolgo {

namespace board {
template<board::BoardLen BOARD_LEN> class FullBoard;
struct Move;
} /* namespace board */

namespace player {

template <board::BoardLen BOARD_LEN>
class RandomPlayer : public PassablePlayer<BOARD_LEN> {
 public:
  explicit RandomPlayer(uint32_t seed) : seed_(seed) {}

 protected:
  board::PositionIndex NextMoveWithPlayableBoard(
      const board::FullBoard<BOARD_LEN> &full_board);

 private:
  uint32_t seed_;
};

template<board::BoardLen BOARD_LEN>
board::PositionIndex RandomPlayer<BOARD_LEN>::NextMoveWithPlayableBoard(
    const board::FullBoard<BOARD_LEN>& full_board) {
  auto playable_indexes = full_board.PlayableIndexes(board::NextForce(full_board));
  assert(!playable_indexes.empty());

  board::PositionIndex rand = util::Rand(playable_indexes.size() - 1, seed_);
  return playable_indexes.at(rand);
}

} /* namespace player */
} /* namespace foolgo */

#endif
