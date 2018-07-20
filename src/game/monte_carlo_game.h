#ifndef FOOLGO_SRC_GAME_MONTE_CARLO_GAME_H_
#define FOOLGO_SRC_GAME_MONTE_CARLO_GAME_H_

#include "../board/position.h"
#include "../player/random_player.h"
#include "game.h"

namespace foolgo {

template<BoardLen BOARD_LEN>
class MonteCarloGame : public Game<BOARD_LEN> {
 public:
  MonteCarloGame(const FullBoard<BOARD_LEN> &full_board, uint32_t seed,
                 bool only_log_board = true);
  ~MonteCarloGame() = default;

  bool ShouldLog() const override {
    return false;
  }
};

template<BoardLen BOARD_LEN>
MonteCarloGame<BOARD_LEN>::MonteCarloGame(
    const FullBoard<BOARD_LEN> &full_board, uint32_t seed,
    bool only_log_board )
    : Game<BOARD_LEN>(full_board, new RandomPlayer<BOARD_LEN>(seed),
                      new RandomPlayer<BOARD_LEN>(seed),
                      only_log_board) {}

} /* namespace foolgo */

#endif
