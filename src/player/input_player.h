#ifndef FOOLGO_SRC_PLAYER_INPUT_PLAYER_H_
#define FOOLGO_SRC_PLAYER_INPUT_PLAYER_H_

#include <iostream>

#include "../board/full_board.h"
#include "../board/pos_cal.h"
#include "../board/position.h"
#include "passable_player.h"

namespace foolgo {
namespace player {

namespace {
template<board::BoardLen BOARD_LEN>
bool IsPostionIndexLegalMove(const board::Position &position,
                             const board::FullBoard<BOARD_LEN> &full_board) {
  auto index_vector = full_board.PlayableIndexes(board::NextForce(full_board));
  board::PositionIndex index = board::PstionAndIndxCcltr<BOARD_LEN>::Ins()
      .GetIndex(position);
  for (board::PositionIndex indx : index_vector) {
    if (indx == index) {
      return true;
    }
  }

  return false;
}
}

template<board::BoardLen BOARD_LEN>
class InputPlayer : public PassablePlayer<BOARD_LEN> {
 public:
  InputPlayer() = default;

 protected:
  board::PositionIndex NextMoveWithPlayableBoard(
      const board::FullBoard<BOARD_LEN> &full_board);
};

template<board::BoardLen BOARD_LEN>
board::PositionIndex InputPlayer<BOARD_LEN>::NextMoveWithPlayableBoard(
    const board::FullBoard<BOARD_LEN> &full_board) {
  board::Position position;
  std::cout << "x, y: ";
  int x, y;
  std::cin >> x >> y;
  position = board::Position(x, y);

  return board::PstionAndIndxCcltr<BOARD_LEN>::Ins().GetIndex(position);
}

}
}

#endif /* FOOLGO_SRC_PLAYER_INPUT_PLAYER_H_ */
