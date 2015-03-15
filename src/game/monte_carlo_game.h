#ifndef FOOLGO_SRC_GAME_MONTE_CARLO_GAME_H_
#define FOOLGO_SRC_GAME_MONTE_CARLO_GAME_H_

#include <log4cplus/logger.h>

#include "../board/position.h"
#include "../player/random_player.h"
#include "game.h"

namespace foolgo {
namespace player {
template<board::BoardLen BOARD_LEN> class RandomPlayer;
} /* namespace player */
} /* namespace foolgo */

namespace foolgo {
namespace game {

template<board::BoardLen BOARD_LEN>
class MonteCarloGame : public Game<BOARD_LEN> {
 public:
  MonteCarloGame(const board::FullBoard<BOARD_LEN> &full_board, uint32_t seed);
  ~MonteCarloGame() = default;
  const log4cplus::Logger &GetLogger() const {
    return logger_;
  }
private:
  static log4cplus::Logger logger_;
};

template<board::BoardLen BOARD_LEN>
log4cplus::Logger MonteCarloGame<BOARD_LEN>::logger_ =
    log4cplus::Logger::getInstance("foolgo.game.MonteCarloGame");

template<board::BoardLen BOARD_LEN>
MonteCarloGame<BOARD_LEN>::MonteCarloGame(
    const board::FullBoard<BOARD_LEN> &full_board, uint32_t seed)
    : Game<BOARD_LEN>(full_board, new player::RandomPlayer<BOARD_LEN>(seed),
                      new player::RandomPlayer<BOARD_LEN>(seed)) {}

} /* namespace player */
} /* namespace foolgo */

#endif
