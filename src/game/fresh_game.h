#ifndef FOOLGO_SRC_GAME_FRESH_GAME_H_
#define FOOLGO_SRC_GAME_FRESH_GAME_H_

#include <log4cplus/logger.h>
#include <cstdint>

#include "../board/position.h"
#include "../def.h"
#include "../player/player.h"
#include "../player/uct_player.h"
#include "../player/input_player.h"
#include "game.h"

namespace foolgo {

namespace player {
template<board::BoardLen BOARD_LEN> class Player;
} /* namespace player */

namespace game {

template<board::BoardLen BOARD_LEN>
class FreshGame : public Game<BOARD_LEN> {
 public:
  static FreshGame* BuildHumanVsAiGame(bool input_player_plays_black,
                                     uint32_t seed);
  ~FreshGame() = default;
  const log4cplus::Logger &GetLogger() const {
    return logger_;
  }
 protected:
  using Game<BOARD_LEN>::Game;

 private:
  static log4cplus::Logger logger_;

  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(FreshGame)
};

template<board::BoardLen BOARD_LEN>
log4cplus::Logger FreshGame<BOARD_LEN>::logger_ =
    log4cplus::Logger::getInstance("foolgo.game.FreshGame");

template<board::BoardLen BOARD_LEN>
FreshGame<BOARD_LEN>* FreshGame<BOARD_LEN>::BuildHumanVsAiGame(
    bool input_player_plays_black, uint32_t seed) {
  auto input_player = new player::InputPlayer<BOARD_LEN>;
  auto ai_player = new player::UctPlayer<BOARD_LEN>(seed);

  player::Player<BOARD_LEN> *black_player, *white_player;
  if (input_player_plays_black) {
    black_player = input_player;
    white_player = ai_player;
  } else {
    black_player = ai_player;
    white_player = input_player;
  }

  board::FullBoard<BOARD_LEN> full_board;
  full_board.Init();

  return new FreshGame(full_board, black_player, white_player);
}

}
}

#endif
