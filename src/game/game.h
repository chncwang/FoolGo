#ifndef FOOLGO_SRC_GAME_GAME_H_
#define FOOLGO_SRC_GAME_GAME_H_

#include <array>
#include <iostream>

#include "../board/force.h"
#include "../board/full_board.h"
#include "../board/position.h"
#include "../def.h"
#include "../player/player.h"

namespace foolgo {
namespace game {

template <BoardLen BOARD_LEN>
class Game {
 public:
  virtual ~Game();

  const FullBoard<BOARD_LEN>& GetFullBoard() const {
    return full_board_;
  }
  void Run();

 protected:
  Game(const FullBoard<BOARD_LEN> &full_board,
       player::Player<BOARD_LEN> *black_player,
       player::Player<BOARD_LEN> *white_player,
       bool only_log_board = true);
  virtual bool ShouldLog() const {
    return false;
  }

 private:
  FullBoard<BOARD_LEN> full_board_;
  std::array<player::Player<BOARD_LEN>*, 2> player_ptrs_;
  bool only_log_board_;

  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(Game)
};

template<BoardLen BOARD_LEN>
Game<BOARD_LEN>::~Game() {
  for (auto ptr : player_ptrs_) {
    if (ptr != nullptr) {
      delete ptr;
    }
  }
}

template<BoardLen BOARD_LEN>
void Game<BOARD_LEN>::Run() {
  if (ShouldLog()) {
    std::cout << full_board_.ToString(only_log_board_) << std::endl;
  }

  while (!full_board_.IsEnd()) {
    Force current_force = NextForce(full_board_);
    player::Player<BOARD_LEN> *current_player = player_ptrs_.at(current_force);
    PositionIndex next_index = current_player->NextMove(full_board_);
    Play(&full_board_, next_index);

    if (ShouldLog()) {
      std::cout << full_board_.ToString(next_index, only_log_board_) <<
        std::endl;
    }
  }
}

template<BoardLen BOARD_LEN>
Game<BOARD_LEN>::Game(const FullBoard<BOARD_LEN> &full_board,
                      player::Player<BOARD_LEN> *black_player,
                      player::Player<BOARD_LEN> *white_player,
                      bool only_log_board)
    : player_ptrs_( { black_player, white_player }),
      only_log_board_(only_log_board) {
  full_board_.Copy(full_board);
}


} /* namespace player */
} /* namespace foolgo */

#endif /* SRC_GAME_GAME_H_ */
