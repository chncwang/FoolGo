#ifndef BOARD_TLT_H
#define BOARD_TLT_H

#include "board.h"

#include <cstring>

namespace {
    const char BLANK = ' ';
    const char BLACK = 'x';
    const char WHITE = 'o';
    const char EMPTY = '.';
}


template <BoardLen BOARD_LEN>
inline void Board<BOARD_LEN>::Init()
{
    memset(points_, EMPTY_POINT, sizeof(points_));
}


template <BoardLen BOARD_LEN>
inline void Board<BOARD_LEN>::Copy(const Board<BOARD_LEN> &b)
{
    memcpy(points_, b.points_, sizeof(points_));
}


template <BoardLen BOARD_LEN>
inline Point Board<BOARD_LEN>::GetPoint(PointIndex index) const
{
    ASSERT(this->GetPosClcltr().IsInBoard(index));
    return points_[index];
}


template <BoardLen BOARD_LEN>
inline Point Board<BOARD_LEN>::GetPoint(const Position &pos) const
{
    PointIndex index = this->GetPosClcltr().GetIndex(pos);
    return this->GetPoint(index);
}


template <BoardLen BOARD_LEN>
inline void Board<BOARD_LEN>::SetPoint(PointIndex indx, Point point)
{
    ASSERT(this->GetPosClcltr().IsInBoard(indx));
    points_[indx] = point;
}


template <BoardLen BOARD_LEN>
inline void Board<BOARD_LEN>::SetPoint(const Position &pos, Point point)
{
    BoardLen index = this->GetPosClcltr().GetIndex(pos);
    this->SetPoint(index, point);
}


template<BoardLen BOARD_LEN>
std::ostream &operator <<(std::ostream &os, const Board<BOARD_LEN> &board)
{
    os << std::string(2, BLANK);
    for (int i=0; i<BOARD_LEN; ++i) os << i << BLANK;
    os << std::endl;

    for (int y=0; y<BOARD_LEN; ++y) {
        os << y << BLANK;
        for (int x=0; x<BOARD_LEN; ++x) {
            Point point = board.GetPoint(Position(x, y));
            if (point == EMPTY_POINT) {
                os << EMPTY << BLANK;
            } else if (point == BLACK_POINT) {
                os << BLACK << BLANK;
            } else if (point == WHITE_POINT) {
                os << WHITE << BLANK;
            } else {
                os << "point: " << point << std::endl;
                CANNOT_RUN_HERE();
            }
        }

        os << std::endl;
    }

    return os;
}


#endif
