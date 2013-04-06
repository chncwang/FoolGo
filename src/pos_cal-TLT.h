#ifndef POS_CAL_TLT_H
#define POS_CAL_TLT_H

#include "pos_cal.h"


template <BoardLen BOARD_LEN>
inline PosCalculator<BOARD_LEN> &PosCalculator<BOARD_LEN>::Ins()
{
    static PosCalculator<BOARD_LEN> ins;
    return ins;
}


template <BoardLen BOARD_LEN>
typename PosCalculator<BOARD_LEN>::PosStatus
PosCalculator<BOARD_LEN>::GetPosStatus(const Position &pos)
{
    if ((pos.x_ == 0 && pos.y_ == 0) ||
            (pos.x_ == 0 && pos.y_ == BoardLenMinusOne<BOARD_LEN>())
            || (pos.x_ == BoardLenMinusOne<BOARD_LEN>() && pos.y_ == 0)
            || (pos.x_ == BoardLenMinusOne<BOARD_LEN>() &&
                pos.y_ == BoardLenMinusOne<BOARD_LEN>())) {
        return IN_CORNER;
    } else if (pos.x_ > 0 && pos.x_ < BoardLenMinusOne<BOARD_LEN>() &&
            pos.y_ > 0 && pos.y_ < BoardLenMinusOne<BOARD_LEN>()) {
        return IN_CENTRAL;
    } else {
        return IN_EDGE;
    }
}


template <BoardLen BOARD_LEN>
PosCalculator<BOARD_LEN>::PosCalculator()
{
    for (int y=0; y<BOARD_LEN; ++y) {
        for (int x=0; x<BOARD_LEN; ++x) {
            PointIndex index = y*BOARD_LEN + x;
            indexes_[y][x] = index;
            position_[index].Set(x, y);
        }
    }
}


#endif
