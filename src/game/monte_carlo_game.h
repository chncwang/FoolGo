#ifndef FOOLGO_SRC_GAME_MONTE_CARLO_GAME_H_
#define FOOLGO_SRC_GAME_MONTE_CARLO_GAME_H_

#include "../board/position.h"
#include "../player/random_player.h"
#include "game.h"

namespace foolgo {
namespace game {

template<board::BoardLen BOARD_LEN>
class MonteCarloGame : public Game<BOARD_LEN> {
 public:
  MonteCarloGame(const board::FullBoard<BOARD_LEN> &full_board, uint32_t seed,
                 bool only_log_board = true);
  ~MonteCarloGame() = default;

  bool ShouldLog() const override {
    return false;
  }
};

template<board::BoardLen BOARD_LEN>
MonteCarloGame<BOARD_LEN>::MonteCarloGame(
    const board::FullBoard<BOARD_LEN> &full_board, uint32_t seed,
    bool only_log_board )
    : Game<BOARD_LEN>(full_board, new player::RandomPlayer<BOARD_LEN>(seed),
                      new player::RandomPlayer<BOARD_LEN>(seed),
                      only_log_board) {}

} /* namespace player */
} /* namespace foolgo */

#endif
