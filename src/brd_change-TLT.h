#ifndef BRD_CHANGE_TLT_H
#define BRD_CHANGE_TLT_H

#include "brd_change.h"


inline void BrdChange::Init(PlayerColor color, PointIndex ko)
{
    ko_indx_.origin_ = ko;
    last_player_.origin_ = color;
}


#endif
