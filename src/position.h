#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

typedef char BoardLen;
typedef int16_t PointIndex;

struct Position {
    BoardLen x_, y_;

    Position() {x_ = 0; y_ = 0;}
    Position(BoardLen x, BoardLen y) {this->Set(x, y);}

    inline void Set(BoardLen x, BoardLen y) {x_ = x; y_ = y;}
};

#endif
