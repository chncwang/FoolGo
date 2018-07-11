#ifndef FOOLGO_SRC_GAME_SGF_GAME_H_
#define FOOLGO_SRC_GAME_SGF_GAME_H_

#include <cstdint>
#include <memory>

#include "../board/position.h"
#include "../def.h"
#include "../player/player.h"
#include "../player/sgf_player.h"
#include "game.h"

namespace foolgo {

template<BoardLen BOARD_LEN>
class SgfGame : public Game<BOARD_LEN> {
 public:
  SgfGame(const GameInfo &game_info) {
    game_info_ = game_info;
  }

  ~SgfGame() = default;

  static std::unique_ptr<SgfGame> BuildSgfGame(const GameInfo &game_info) {
    SgfPlayer<BOARD_LEN> *black_player = new SgfPlayer<BOARD_LEN>(game_info);
    SgfPlayer<BOARD_LEN> *white_player = new SgfPlayer<BOARD_LEN>(game_info);
    FullBoard<BOARD_LEN> full_board;
    full_board.Init();

    return std::unique_ptr<SgfGame>(new SgfGame<BOARD_LEN>(
          full_board, black_player, white_player, game_info));
  }

 protected:
  SgfGame(const FullBoard<BOARD_LEN> &full_board,
       Player<BOARD_LEN> *black_player,
       Player<BOARD_LEN> *white_player,
       const GameInfo &game_info,
       bool only_log_board = true) : Game<BOARD_LEN>(full_board, black_player,
           white_player, only_log_board), game_info_(game_info) {}

  bool ShouldLog() const override {
    return true;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(SgfGame);
  GameInfo game_info_;
};

}

#endif
