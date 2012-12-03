#ifndef AIRSET_UTIL_H
#define AIRSET_UTIL_H

#include <bitset>

#include "position.h"
#include "pos_cal.h"


template <BoardLen BOARD_LEN>
void SetAir(std::bitset<FOO_SQUARE(BOARD_LEN)> *p_arst, const Position &pos)
{
    PointIndex indx = PosCalculator<BOARD_LEN>::Ins().GetIndex(pos);
    p_arst->set(indx);
}


#endif
