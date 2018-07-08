#ifndef FOOLGO_SRC_PLAYER_PLAYER_H_
#define FOOLGO_SRC_PLAYER_PLAYER_H_

#include "../board/full_board.h"
#include "../board/position.h"
#include "../def.h"

namespace foolgo {

template<BoardLen BOARD_LEN> class FullBoard;

namespace player {

template<BoardLen BOARD_LEN>
class Player {
 public:
  Player() = default;
  virtual ~Player() = default;

  virtual PositionIndex NextMove(
      const FullBoard<BOARD_LEN> &full_board) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(Player)
};

} /* namespace player */
} /* namespace foolgo */

#endif /* SRC_PLAYER_PLAYER_H_ */
