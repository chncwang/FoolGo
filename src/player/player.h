#ifndef FOOLGO_SRC_PLAYER_PLAYER_H_
#define FOOLGO_SRC_PLAYER_PLAYER_H_

#include "../board/full_board.h"
#include "../board/position.h"
#include "../def.h"

namespace foolgo {

namespace board {
template<board::BoardLen BOARD_LEN> class FullBoard;
} /* namespace board */

namespace player {

template<board::BoardLen BOARD_LEN>
class Player {
 public:
  Player() = default;
  virtual ~Player() = default;

  virtual board::PositionIndex NextMove(
      const board::FullBoard<BOARD_LEN> &full_board) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(Player)
};

} /* namespace player */
} /* namespace foolgo */

#endif /* SRC_PLAYER_PLAYER_H_ */
