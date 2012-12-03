#ifndef BOARD_H
#define BOARD_H

#include "def.h"
#include "position.h"
#include "pos_cal.h"

typedef char Point;

const Point BLACK_POINT = 0;
const Point WHITE_POINT = 1;
const Point EMPTY_POINT = 2;



template <BoardLen BOARD_LEN>
class Board
{
public:
    explicit Board();

    Point GetPoint(BoardLen index) const {return points_[(int)index];}
    inline Point GetPoint(const Position &pos) const;

    void SetPoint(BoardLen index, Point point) {points_[(int)index] = point;}
    inline void SetPoint(const Position &pos, Point point);

    PosCalculator<BOARD_LEN> &GetPosClcltr() const {
        return PosCalculator<BOARD_LEN>::Ins();
    }

#ifdef FOO_TEST
    void PRINT() const;
    static void TEST();
#endif

private:
    Point points_[FOO_SQUARE(BOARD_LEN)];
};



#include "board-TLT.h"

#ifdef FOO_TEST
#include "board_TEST.h"
#endif

#endif
