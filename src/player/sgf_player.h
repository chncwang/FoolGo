#ifndef FOOLGO_SRC_PLAYER_SGF_PLAYER_H_
#define FOOLGO_SRC_PLAYER_SGF_PLAYER_H_

#include <cassert>
#include <cstdint>

#include "board/full_board.h"
#include "board/position.h"
#include "passable_player.h"

namespace foolgo {

template<BoardLen BOARD_LEN> class FullBoard;
struct Move;

namespace player {

template <BoardLen BOARD_LEN>
class SgfPlayer : public PassablePlayer<BOARD_LEN> {
 public:
  explicit SgfPlayer() = default;

 protected:
  PositionIndex NextMoveWithPlayableBoard(
      const FullBoard<BOARD_LEN> &full_board);

 private:
};

template<BoardLen BOARD_LEN>
PositionIndex SgfPlayer<BOARD_LEN>::NextMoveWithPlayableBoard(
    const FullBoard<BOARD_LEN>& full_board) {
  auto playable_indexes =
    full_board.PlayableIndexes(NextForce(full_board));
  assert(!playable_indexes.empty());
}

} /* namespace player */
} /* namespace foolgo */

#endif
