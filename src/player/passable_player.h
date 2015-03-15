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
  virtual board::PositionIndex NextMove(
      const board::FullBoard<BOARD_LEN> &full_board,
      const typename board::FullBoard<BOARD_LEN>::BitSet &playable_indexes) = 0;
};

template<board::BoardLen BOARD_LEN>
board::PositionIndex PassablePlayer<BOARD_LEN>::NextMove(
    const board::FullBoard<BOARD_LEN> &full_board) {
  board::Force current_force = board::NextForce(full_board);
  auto playable_indexes = board::PlayableIndexesConsideringKo(full_board,
                                                              current_force);
  int count = playable_indexes.count();
  if (count == 0) {
    return board::POSITION_INDEX_PASS;
  } else {
    return NextMove(full_board, playable_indexes);
  }
}

}
}

#endif
