#ifndef FOOLGO_SRC_PLAYER_RANDOM_PLAYER_H_
#define FOOLGO_SRC_PLAYER_RANDOM_PLAYER_H_

#include <cassert>
#include <cstdint>

#include "../board/full_board.h"
#include "../board/position.h"
#include "../math/rand.h"
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
  board::PositionIndex NextMove(
      const board::FullBoard<BOARD_LEN> &full_board,
      const typename board::FullBoard<BOARD_LEN>::BitSet &playable_indexes);

 private:
  uint32_t seed_;
};

template<board::BoardLen BOARD_LEN>
board::PositionIndex RandomPlayer<BOARD_LEN>::NextMove(
    const board::FullBoard<BOARD_LEN>& full_board,
    const typename board::FullBoard<BOARD_LEN>::BitSet &playable_indexes) {
  assert(playable_indexes.count() > 0);

  board::PositionIndex rand = math::Rand(playable_indexes.count() - 1, seed_);
  return math::CalSpecifiedOneOccurrenceTimeIndex<
      board::BoardLenSquare<BOARD_LEN>()>(playable_indexes, rand);
}

} /* namespace player */
} /* namespace foolgo */

#endif
