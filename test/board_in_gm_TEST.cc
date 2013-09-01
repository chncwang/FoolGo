#include "board_in_gm.h"

#include "gtest/gtest.h"



class BoardInGmTest
{
protected:
    virtual void SetUp() {
        board_.Init();
    }

    BoardInGm<9> board_;
};



TEST(BoardInGmTest, Init)
{
    BoardInGm<9> board;
    board.Init();

    for (int i=0; i<BoardLenSquare<9>(); ++i) {
        EXPECT_EQ(board.GetPoint(i), EMPTY_POINT);
    }
}
