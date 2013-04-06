#ifndef EYE_SET_TLT_H
#define EYE_SET_TLT_H

#include "eye_set.h"


template <BoardLen BOARD_LEN>
inline void EyeSet<BOARD_LEN>::Copy(const EyeSet &es)
{
    eyes_ = es.eyes_;
    real_eyes_ = es.real_eyes_;
}


#endif
