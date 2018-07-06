#include "../../src/game/monte_carlo_game.h"

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "../../src/board/full_board.h"
#include "../../src/board/zob_hasher.h"
#include "../../src/math/rand.h"
#include "../def_for_test.h"
#include "../test.h"

namespace foolgo {
namespace game {

using board::FullBoard;
using board::ZobHasher;

class MonteCarloGameTest : public Test {
 protected:
  virtual void SetUp() {
    Test::SetUp();

    uint32_t seed = math::GetTimeSeed();
    logger_->info("seed:{}", seed);

    ZobHasher<DEFAULT_BOARD_LEN>::Init(seed);
    FullBoard<DEFAULT_BOARD_LEN> full_board_;
    full_board_.Init();

    game_ = new MonteCarloGame<DEFAULT_BOARD_LEN>(full_board_, seed, false);
  }

  virtual void TearDown() {
    delete game_;
  }

  MonteCarloGame<DEFAULT_BOARD_LEN> *game_ = nullptr;

  static std::shared_ptr<spdlog::logger> logger_;
};

std::shared_ptr<spdlog::logger> MonteCarloGameTest::logger_ =
    spdlog::stdout_logger_st("foolgo.game.MonteCarloGameTest");

TEST_F(MonteCarloGameTest, Run) {
  game_->Run();
}

}
}
