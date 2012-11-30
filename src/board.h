#ifndef BOARD_H
#define BOARD_H

#include "def.h"
#include "position.h"
#include "pos_cal.h"

typedef char Point;

extern const Point EMPTY_POINT;
extern const Point BLACK_POINT;
extern const Point WHITE_POINT;



template <BoardLen BOARD_LEN>
class Board
{
public:
    explicit Board();
    ~Board();

    Point GetPoint(BoardLen index) const {return points_[(int)index];}
    void SetPoint(BoardLen index, Point point) {points_[index] = point;}
    inline Point GetPoint(const Position &pos) const;
    inline void SetPoint(const Position &pos, Point point);

#ifdef FOO_TEST
    void PRINT();
    static void TEST();
#endif

private:
    const Position &GetPosByIndex(PointIndex index) const {
        return PosCalculator<BOARD_LEN>::Ins().GetPos(index);
    }
    PointIndex GetIndexByPos(const Position &pos) const {
        return PosCalculator<BOARD_LEN>::Ins().GetIndex(pos);
    }

    Point points_[BOARD_LEN * BOARD_LEN];
};



#include "board-TLT.h"
#include "board_TEST.h"

#endif
