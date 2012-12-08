#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

typedef char BoardLen;
typedef int16_t PointIndex;

struct Position {
    static const BoardLen STRAIGHT_ORNTTIONS[4][2], OBLIQUE_ORNTTIONS[4][2];

    BoardLen x_, y_;

    Position() : x_(0), y_(0) {}
    Position(BoardLen x, BoardLen y) : x_(x), y_(y) {}

    inline void Set(BoardLen x, BoardLen y) {x_ = x; y_ = y;}
    Position AdjcntPos(int i) const {
        return Position(x_ + STRAIGHT_ORNTTIONS[i][0],
                        y_ + STRAIGHT_ORNTTIONS[i][1]);
    }
    Position ObliquePos(int i) const {
        return Position(x_ + OBLIQUE_ORNTTIONS[i][0],
                        y_ + OBLIQUE_ORNTTIONS[i][1]);
    }
};

const BoardLen Position::STRAIGHT_ORNTTIONS[4][2] = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}
};

const BoardLen Position::OBLIQUE_ORNTTIONS[4][2] = {
    {1, -1},
    {1, 1},
    {-1, 1},
    {-1, -1}
};

template <BoardLen BOARD_LEN>
constexpr inline PointIndex BoardLenSquare()
{
    return BOARD_LEN * BOARD_LEN;
}

template <BoardLen BOARD_LEN>
constexpr inline BoardLen BoardLenMinusOne()
{
    return BOARD_LEN - 1;
}

#endif
