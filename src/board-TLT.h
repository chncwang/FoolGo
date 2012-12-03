#ifndef BOARD_TLT_H
#define BOARD_TLT_H

#include "board.h"

#include <cstring>


template <BoardLen BOARD_LEN>
Board<BOARD_LEN>::Board()
{
    memset(points_, EMPTY_POINT, sizeof(points_));
}


template <BoardLen BOARD_LEN>
Point Board<BOARD_LEN>::GetPoint(const Position &pos) const
{
    BoardLen index = this->GetPosClcltr().GetIndex(pos);
    return this->GetPoint(index);
}


template <BoardLen BOARD_LEN>
void Board<BOARD_LEN>::SetPoint(const Position &pos, Point point)
{
    BoardLen index = this->GetPosClcltr().GetIndex(pos);
    this->SetPoint(index, point);
}


#endif
