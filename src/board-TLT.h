#ifndef BOARD_TLT_H
#define BOARD_TLT_H

#include "board.h"

#include <cstring>


template <BoardLen BOARD_LEN>
void Board<BOARD_LEN>::Init()
{
    memset(points_, EMPTY_POINT, sizeof(points_));
}


template <BoardLen BOARD_LEN>
void Board<BOARD_LEN>::Copy(const Board<BOARD_LEN> &b)
{
    memcpy(points_, b.points_, sizeof(points_));
}


template <BoardLen BOARD_LEN>
INLINE Point Board<BOARD_LEN>::GetPoint(PointIndex index) const
{
    ASSERT(this->GetPosClcltr().IsInBoard(index));
    return points_[index];
}


template <BoardLen BOARD_LEN>
INLINE Point Board<BOARD_LEN>::GetPoint(const Position &pos) const
{
    PointIndex index = this->GetPosClcltr().GetIndex(pos);
    return this->GetPoint(index);
}


template <BoardLen BOARD_LEN>
INLINE void Board<BOARD_LEN>::SetPoint(PointIndex indx, Point point)
{
    ASSERT(this->GetPosClcltr().IsInBoard(indx));
    points_[indx] = point;
}


template <BoardLen BOARD_LEN>
INLINE void Board<BOARD_LEN>::SetPoint(const Position &pos, Point point)
{
    BoardLen index = this->GetPosClcltr().GetIndex(pos);
    this->SetPoint(index, point);
}


#endif
