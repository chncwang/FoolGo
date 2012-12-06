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
    void Init();
    void Copy(const Board<BOARD_LEN> &b);

    inline Point GetPoint(PointIndex index) const;
    inline Point GetPoint(const Position &pos) const;

    inline void SetPoint(PointIndex index, Point point);
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

    DISALLOW_COPY_AND_ASSIGN(Board);
};



#include "board-TLT.h"

#ifdef FOO_TEST
#include "board_TEST.h"
#endif

#endif
