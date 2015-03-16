#ifndef FOOLGO_SRC_PLAYER_PASSABLE_PLAYER_H_
#define FOOLGO_SRC_PLAYER_PASSABLE_PLAYER_H_

#include "../board/full_board.h"
#include "../board/position.h"
#include "../def.h"
#include "player.h"

namespace foolgo {
namespace player {

template<board::BoardLen BOARD_LEN>
class PassablePlayer : public Player<BOARD_LEN> {
 public:
  virtual ~PassablePlayer() = default;
  board::PositionIndex NextMove(
      const board::FullBoard<BOARD_LEN> &full_board);
 protected:
  virtual board::PositionIndex NextMoveWithPlayableBoard(
      const board::FullBoard<BOARD_LEN> &full_board) = 0;
};

template<board::BoardLen BOARD_LEN>
board::PositionIndex PassablePlayer<BOARD_LEN>::NextMove(
    const board::FullBoard<BOARD_LEN> &full_board) {
  board::Force current_force = board::NextForce(full_board);
  auto playable_indexes = full_board.PlayableIndexes(current_force);
  if (playable_indexes.empty()) {
    return board::POSITION_INDEX_PASS;
  } else {
    return NextMoveWithPlayableBoard(full_board);
  }
}

}
}

#endif
