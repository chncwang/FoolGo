#include "../../src/game/monte_carlo_game.h"

#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>

#include "../../src/board/full_board.h"
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

    ZobHasher<DEFAULT_BOARD_LEN>::Init(SEED);
    FullBoard<DEFAULT_BOARD_LEN> full_board_;
    full_board_.Init();

    game_ = new MonteCarloGame<DEFAULT_BOARD_LEN>(full_board_, SEED, false);
  }

  virtual void TearDown() {
    delete game_;
  }

  MonteCarloGame<DEFAULT_BOARD_LEN> *game_ = nullptr;
};

TEST_F(MonteCarloGameTest, Run) {
  game_->Run();
}

}
}
