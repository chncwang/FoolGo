#ifndef FOOLGO_SRC_GAME_MONTE_CARLO_GAME_H_
#define FOOLGO_SRC_GAME_MONTE_CARLO_GAME_H_

#include <spdlog/spdlog.h>

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
  spdlog::logger &GetLogger() const override {
    return *logger_;
  }
private:
  static std::shared_ptr<spdlog::logger> logger_;
  static std::shared_ptr<spdlog::logger> InitLogger() {
    std::shared_ptr<spdlog::logger> logger =
      spdlog::stdout_logger_st("foolgo.game.MonteCarloGame");
    logger->set_level(spdlog::level::off);
    return logger;
  }
};

template<board::BoardLen BOARD_LEN>
std::shared_ptr<spdlog::logger> MonteCarloGame<BOARD_LEN>::logger_ =
MonteCarloGame<BOARD_LEN>::InitLogger();

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
