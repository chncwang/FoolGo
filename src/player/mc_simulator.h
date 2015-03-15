#ifndef FOOLGO_SRC_PLAYER_MC_SIMULATOR_H_
#define FOOLGO_SRC_PLAYER_MC_SIMULATOR_H_

#include "../board/board.h"
#include "../board/full_board.h"
#include "../board/position.h"
#include "../def.h"
#include "../math/rand.h"

namespace foolgo {

namespace board {

template<char BOARD_LEN> class FullBoard;

}

namespace player {

template<board::BoardLen BOARD_LEN>
class MCSimulator {
 public:
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(MCSimulator)
  static MCSimulator &Ins();

  board::PositionIndex Simulate(
      const board::FullBoard<BOARD_LEN> &input_board) const;

 private:
  MCSimulator() = default;
};

template<board::BoardLen BOARD_LEN>
inline MCSimulator<BOARD_LEN> &MCSimulator<BOARD_LEN>::Ins() {
  static MCSimulator<BOARD_LEN> ins;
  return ins;
}

template<board::BoardLen BOARD_LEN>
board::PositionIndex MCSimulator<BOARD_LEN>::Simulate(
    const board::FullBoard<BOARD_LEN> &input_board) const {
  board::FullBoard<BOARD_LEN> bingm(nullptr);
  bingm.Copy(input_board);
  int count = 0;

  while (!IsEnd(bingm)) {
    if (++count > board::MoveCountUpperBoundPerGame<BOARD_LEN>()) {
      break;
    }
    board::Force last_player = bingm.LastForce();
    board::Force cur_player = board::OppositeForce(last_player);
    auto noko_plbl = board::PlayableIndexesConsideringKo(bingm, cur_player);

    board::PositionIndex play_c = noko_plbl.count();
    if (play_c > 0) {
      board::PositionIndex rand = math::Rand(play_c - 1, math::GetTimeSeed());
      board::PositionIndex cur_indx = math::CalSpecifiedOneOccurrenceTimeIndex<
          board::BoardLenSquare<BOARD_LEN>()>(noko_plbl, rand);
      bingm.PlayMove(board::Move(cur_player, cur_indx));
    } else {
      bingm.Pass(cur_player);
    }
  }

  return bingm.BlackRegion();
}

}
}

#endif
