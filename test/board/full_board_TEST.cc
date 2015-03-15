#include "../../src/board/full_board.h"

#include <gtest/gtest.h>
#include <gtest/internal/gtest-internal.h>

#include "../def_for_test.h"
#include "../test.h"

namespace foolgo {
namespace board {

class BoardInGmTest : public Test {
};

TEST_F(BoardInGmTest, Init) {
  ZobHasher<DEFAULT_BOARD_LEN> zob_hasher(SEED);
  FullBoard<DEFAULT_BOARD_LEN> board(&zob_hasher);
  board.Init();

  for (int i = 0; i < BoardLenSquare<DEFAULT_BOARD_LEN>(); ++i) {
    EXPECT_EQ(board.GetPointState(i), EMPTY_POINT);
  }
}

}
}
