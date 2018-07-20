#ifndef FOOLGO_SRC_PLAYER_PASSABLE_PLAYER_H_
#define FOOLGO_SRC_PLAYER_PASSABLE_PLAYER_H_

#include "../board/full_board.h"
#include "../board/position.h"
#include "../def.h"
#include "player.h"

namespace foolgo {

template<BoardLen BOARD_LEN>
class PassablePlayer : public Player<BOARD_LEN> {
 public:
  virtual ~PassablePlayer() = default;
  PositionIndex NextMove(
      const FullBoard<BOARD_LEN> &full_board);
 protected:
  virtual PositionIndex NextMoveWithPlayableBoard(
      const FullBoard<BOARD_LEN> &full_board) = 0;
};

template<BoardLen BOARD_LEN>
PositionIndex PassablePlayer<BOARD_LEN>::NextMove(
    const FullBoard<BOARD_LEN> &full_board) {
  Force current_force = NextForce(full_board);
  auto playable_indexes = full_board.PlayableIndexes(current_force);
  if (playable_indexes.empty()) {
    return POSITION_INDEX_PASS;
  } else {
    return NextMoveWithPlayableBoard(full_board);
  }
}

}

#endif
