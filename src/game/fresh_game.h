#ifndef FOOLGO_SRC_GAME_FRESH_GAME_H_
#define FOOLGO_SRC_GAME_FRESH_GAME_H_

#include <cstdint>
#include <memory>

#include "board/position.h"
#include "board/full_board.h"
#include "def.h"
#include "player/player.h"
#include "player/uct_player.h"
#include "player/input_player.h"
#include "game.h"

namespace foolgo {

template<BoardLen BOARD_LEN>
class FreshGame : public Game<BOARD_LEN> {
 public:
  static std::unique_ptr<FreshGame<BOARD_LEN>> BuildHumanVsAiGame(
      bool input_player_plays_black, uint32_t seed, int mc_game_count,
      int thread_count) {
    auto input_player = new InputPlayer<BOARD_LEN>;
    auto ai_player = new UctPlayer<BOARD_LEN>(seed, mc_game_count,
        thread_count);

    Player<BOARD_LEN> *black_player, *white_player;
    if (input_player_plays_black) {
      black_player = input_player;
      white_player = ai_player;
    } else {
      black_player = ai_player;
      white_player = input_player;
    }

    FullBoard<BOARD_LEN> full_board;
    full_board.Init();

    return std::unique_ptr<FreshGame<BOARD_LEN>>(new FreshGame<BOARD_LEN>(
          full_board, black_player, white_player));
  }

  static std::unique_ptr<FreshGame<BOARD_LEN>> BuildHumanVsHumanGame(
      bool only_log_board = true) {
    auto black_player = new InputPlayer<BOARD_LEN>;
    auto white_player = new InputPlayer<BOARD_LEN>;

    FullBoard<BOARD_LEN> full_board;
    full_board.Init();

    return std::unique_ptr<FreshGame<BOARD_LEN>>(full_board, black_player,
        white_player, only_log_board);
  }

  static std::unique_ptr<FreshGame<BOARD_LEN>> BuildAiVsAiGame(
      uint32_t seed, int mc_game_count, int thread_couunt,
      bool only_log_board = true) {
    auto black_player = new UctPlayer<BOARD_LEN>(seed, mc_game_count,
        thread_couunt);
    auto white_player = new UctPlayer<BOARD_LEN>(seed, mc_game_count,
        thread_couunt);

    FullBoard<BOARD_LEN> full_board;
    full_board.Init();

    return new FreshGame(full_board, black_player, white_player,
        only_log_board);
  }

  static std::unique_ptr<FreshGame<BOARD_LEN>> BuildFreshGame(
      Player<BOARD_LEN> *black_player,
      Player<BOARD_LEN> *white_player) {
    FullBoard<BOARD_LEN> full_board;
    full_board.Init();
    return std::unique_ptr<FreshGame<BOARD_LEN>>(
        new FreshGame<BOARD_LEN>(full_board, black_player,
          white_player));
  }
  ~FreshGame() = default;
  bool ShouldLog() const override {
    return true;
  }
 protected:
  using Game<BOARD_LEN>::Game;

 private:
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(FreshGame)
};

}

#endif
