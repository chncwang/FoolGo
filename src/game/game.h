#ifndef FOOLGO_SRC_GAME_GAME_H_
#define FOOLGO_SRC_GAME_GAME_H_

#include <log4cplus/loggingmacros.h>
#include <array>
#include <iostream>

#include "../board/force.h"
#include "../board/full_board.h"
#include "../board/position.h"
#include "../def.h"
#include "../player/player.h"

namespace foolgo {
namespace game {

template <board::BoardLen BOARD_LEN>
class Game {
 public:
  virtual ~Game();

  const board::FullBoard<BOARD_LEN>& GetFullBoard() const {
    return full_board_;
  }
  void Run();

 protected:
  Game(const board::FullBoard<BOARD_LEN> &full_board,
       player::Player<BOARD_LEN> *black_player,
       player::Player<BOARD_LEN> *white_player);
  virtual const log4cplus::Logger &GetLogger() const = 0;
 private:
  board::FullBoard<BOARD_LEN> full_board_;
  std::array<player::Player<BOARD_LEN>*, 2> player_ptrs_;

  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(Game)
};

template<board::BoardLen BOARD_LEN>
Game<BOARD_LEN>::~Game() {
  for (auto ptr : player_ptrs_) {
    if (ptr != nullptr) {
      delete ptr;
    }
  }
}

template<board::BoardLen BOARD_LEN>
void Game<BOARD_LEN>::Run() {
  const log4cplus::Logger &logger = GetLogger();
  LOG4CPLUS_INFO(logger, "full_board_:" << full_board_.ToString(true));

  while (!full_board_.IsEnd()) {
    board::Force current_force = NextForce(full_board_);
    player::Player<BOARD_LEN> *current_player = player_ptrs_.at(current_force);
    board::PositionIndex next_index = current_player->NextMove(full_board_);
    if (next_index == board::POSITION_INDEX_PASS) {
      full_board_.Pass(current_force);
    } else {
      full_board_.PlayMove(board::Move(current_force, next_index));
    }

    LOG4CPLUS_INFO(logger,
                   "full_board_:" << full_board_.ToString(next_index, true));
  }
}

template<board::BoardLen BOARD_LEN>
Game<BOARD_LEN>::Game(const board::FullBoard<BOARD_LEN> &full_board,
                      player::Player<BOARD_LEN> *black_player,
                      player::Player<BOARD_LEN> *white_player)
    : player_ptrs_({black_player, white_player}) {
  full_board_.Copy(full_board);
}


} /* namespace player */
} /* namespace foolgo */

#endif /* SRC_GAME_GAME_H_ */
