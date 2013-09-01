#include "board.h"

#include <iostream>

#include "gtest/gtest.h"

using namespace std;



class BoardTest : public ::testing::Test
{
protected:
    virtual void SetUp() {
        board_.Init();
        board_.SetPoint(Position(3, 3), BLACK_POINT);
        board_.SetPoint(Position(2, 4), WHITE_POINT);
    }

    Board<9> board_;
};



TEST_F(BoardTest, Print)
{
    cout << board_;
}


TEST_F(BoardTest, Init)
{
    Board<9> board;
    board.Init();

    for (int i=0; i<BoardLenSquare<9>(); ++i) {
        EXPECT_EQ(board.GetPoint(i), EMPTY_POINT);
    }
}


TEST_F(BoardTest, Copy)
{
    Board<9> board;
    board_.Copy(board);

    for (int i=0; i<BoardLenSquare<9>(); ++i) {
        EXPECT_EQ(board.GetPoint(i), board_.GetPoint(i));
    }
}
