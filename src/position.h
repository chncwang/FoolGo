#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

typedef char BoardLen;
typedef int16_t PointIndex;

struct Position {
    static const BoardLen STRAIGHT_ORNTTIONS[4][2];

    BoardLen x_, y_;

    Position() {x_ = 0; y_ = 0;}
    Position(BoardLen x, BoardLen y) {this->Set(x, y);}

    inline void Set(BoardLen x, BoardLen y) {x_ = x; y_ = y;}
    Position AdjcntPos(int i) const {
        return Position(x_ + STRAIGHT_ORNTTIONS[i][0],
                        y_ + STRAIGHT_ORNTTIONS[i][1]);
    }
};

const BoardLen Position::STRAIGHT_ORNTTIONS[4][2] = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}
};


template <BoardLen BOARD_LEN>
inline PointIndex BoardLenSquare()
{
    return BOARD_LEN * BOARD_LEN;
}


#endif
