#include "../../src/game/monte_carlo_game.h"

#include <gtest/gtest.h>

#include "../../src/board/full_board.h"
#include "../../src/board/zob_hasher.h"
#include "../../src/math/rand.h"
#include "../def_for_test.h"
#include "../test.h"

namespace foolgo {
namespace game {

using board::FullBoard;
using board::ZobHasher;
using log4cplus::Logger;

class MonteCarloGameTest : public Test {
 protected:
  virtual void SetUp() {
    Test::SetUp();

//    uint32_t seed = math::GetTimeSeed();
    uint32_t seed = 657062556;
    LOG4CPLUS_INFO(logger_, "seed:" << seed);

    ZobHasher<DEFAULT_BOARD_LEN>::Init(seed);
    FullBoard<DEFAULT_BOARD_LEN> full_board_;
    full_board_.Init();

    game_ = new MonteCarloGame<DEFAULT_BOARD_LEN>(full_board_, seed, false);
  }

  virtual void TearDown() {
    delete game_;
  }

  MonteCarloGame<DEFAULT_BOARD_LEN> *game_ = nullptr;

  static Logger logger_;
};

Logger MonteCarloGameTest::logger_ =
    Logger::getInstance("foolgo.game.MonteCarloGameTest");

TEST_F(MonteCarloGameTest, Run) {
  game_->Run();
}

}
}
