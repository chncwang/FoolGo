#ifndef FOOLGO_SRC_PLAYER_SGF_PLAYER_H_
#define FOOLGO_SRC_PLAYER_SGF_PLAYER_H_

#include <cassert>
#include <cstdint>

#include "board/full_board.h"
#include "board/position.h"
#include "game/game_info.h"
#include "passable_player.h"

namespace foolgo {

template <BoardLen BOARD_LEN>
class SgfPlayer : public PassablePlayer<BOARD_LEN> {
 public:
  SgfPlayer() = default;

  SgfPlayer(const GameInfo &game_info) : game_info_(&game_info) {}

 protected:
  PositionIndex NextMoveWithPlayableBoard(
      const FullBoard<BOARD_LEN> &full_board);

 private:
  const GameInfo *game_info_ = nullptr;
};

template<BoardLen BOARD_LEN>
PositionIndex SgfPlayer<BOARD_LEN>::NextMoveWithPlayableBoard(
    const FullBoard<BOARD_LEN>& full_board) {
  auto playable_indexes =
    full_board.PlayableIndexes(NextForce(full_board));
  assert(!playable_indexes.empty());

  int index = full_board.MoveCount();
  assert(index <= game_info_->moves.size());
  if (index == game_info_->moves.size()) {
    return POSITION_INDEX_END;
  }

  Move move = game_info_->moves.at(index);
  if (index > 0 && move.force != OppositeForce(full_board.LastForce())) {
    std::cerr << "index:" << index << " last force:" <<
      ForceToString(full_board.LastForce()) << " current force:" <<
          ForceToString(move.force) << std::endl;
    abort();
  }
  return move.position_index;
}

} /* namespace foolgo */

#endif
