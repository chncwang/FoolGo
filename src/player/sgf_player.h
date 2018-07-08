#ifndef FOOLGO_SRC_PLAYER_SGF_PLAYER_H_
#define FOOLGO_SRC_PLAYER_SGF_PLAYER_H_

#include <cassert>
#include <cstdint>

#include "board/full_board.h"
#include "board/position.h"
#include "passable_player.h"

namespace foolgo {

namespace board {
template<board::BoardLen BOARD_LEN> class FullBoard;
struct Move;
} /* namespace board */

namespace player {

template <board::BoardLen BOARD_LEN>
class SgfPlayer : public PassablePlayer<BOARD_LEN> {
 public:
  explicit SgfPlayer() = default;

 protected:
  board::PositionIndex NextMoveWithPlayableBoard(
      const board::FullBoard<BOARD_LEN> &full_board);

 private:
};

template<board::BoardLen BOARD_LEN>
board::PositionIndex SgfPlayer<BOARD_LEN>::NextMoveWithPlayableBoard(
    const board::FullBoard<BOARD_LEN>& full_board) {
  auto playable_indexes =
    full_board.PlayableIndexes(board::NextForce(full_board));
  assert(!playable_indexes.empty());
}

} /* namespace player */
} /* namespace foolgo */

#endif
