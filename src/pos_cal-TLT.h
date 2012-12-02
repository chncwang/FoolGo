#ifndef POS_CAL_TLT_H
#define POS_CAL_TLT_H

#include "pos_cal.h"


template <BoardLen BOARD_LEN>
PosCalculator<BOARD_LEN> &PosCalculator<BOARD_LEN>::Ins()
{
    static PosCalculator<BOARD_LEN> ins;
    return ins;
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
