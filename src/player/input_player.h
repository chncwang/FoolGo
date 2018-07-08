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
template<BoardLen BOARD_LEN>
bool IsPostionIndexLegalMove(const Position &position,
                             const FullBoard<BOARD_LEN> &full_board) {
  auto index_vector = full_board.PlayableIndexes(NextForce(full_board));
  PositionIndex index = PstionAndIndxCcltr<BOARD_LEN>::Ins()
      .GetIndex(position);
  for (PositionIndex indx : index_vector) {
    if (indx == index) {
      return true;
    }
  }

  return false;
}
}

template<BoardLen BOARD_LEN>
class InputPlayer : public PassablePlayer<BOARD_LEN> {
 public:
  InputPlayer() = default;

 protected:
  PositionIndex NextMoveWithPlayableBoard(
      const FullBoard<BOARD_LEN> &full_board);
};

template<BoardLen BOARD_LEN>
PositionIndex InputPlayer<BOARD_LEN>::NextMoveWithPlayableBoard(
    const FullBoard<BOARD_LEN> &full_board) {
  Position position;
  std::cout << "x, y: ";
  int x, y;
  std::cin >> x >> y;
  position = Position(x, y);

  return PstionAndIndxCcltr<BOARD_LEN>::Ins().GetIndex(position);
}

}
}

#endif /* FOOLGO_SRC_PLAYER_INPUT_PLAYER_H_ */
