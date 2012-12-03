#ifndef BOARD_H
#define BOARD_H

#include "def.h"
#include "position.h"
#include "pos_cal.h"

typedef char Point;

const Point EMPTY_POINT = 0;
const Point BLACK_POINT = 1;
const Point WHITE_POINT = 2;



template <BoardLen BOARD_LEN>
class Board
{
public:
    explicit Board();

    Point GetPoint(BoardLen index) const {return points_[(int)index];}
    inline Point GetPoint(const Position &pos) const;

    void SetPoint(BoardLen index, Point point) {points_[(int)index] = point;}
    inline void SetPoint(const Position &pos, Point point);

#ifdef FOO_TEST
    void PRINT() const;
    static void TEST();
#endif

private:
    const Position &GetPosByIndex(PointIndex index) const {
        return PosCalculator<BOARD_LEN>::Ins().GetPos(index);
    }
    PointIndex GetIndexByPos(const Position &pos) const {
        return PosCalculator<BOARD_LEN>::Ins().GetIndex(pos);
    }

    Point points_[FOO_SQUARE(BOARD_LEN)];
};



#include "board-TLT.h"

#ifdef FOO_TEST
#include "board_TEST.h"
#endif

#endif
