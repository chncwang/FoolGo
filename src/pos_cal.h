#ifndef POS_CAL_H
#define POS_CAL_H

#include "def.h"
#include "position.h"



template <BoardLen BOARD_LEN>
class PosCalculator
{
public:
    inline static PosCalculator &Ins();

    const Position &GetPos(PointIndex index) const {
        FOO_ASSERT(this->IsInBoard(index));
        return position_[index];
    }
    PointIndex GetIndex(const Position &pos) const {
        FOO_ASSERT(this->IsInBoard(pos));
        return indexes_[(int)pos.y_][(int)pos.x_];
    }

    bool IsInBoard(const Position &pos) const {
        return pos.x_ >= 0 && pos.x_ < BOARD_LEN
            && pos.y_ >= 0 && pos.y_ < BOARD_LEN;
    }
    bool IsInBoard(PointIndex indx) const {
        return indx >= 0 && indx < FOO_SQUARE(BOARD_LEN);
    }

#ifdef FOO_TEST
    static void TEST();
#endif

private:
    explicit PosCalculator();
    ~PosCalculator() {}

    Position position_[FOO_SQUARE(BOARD_LEN)];
    PointIndex indexes_[BOARD_LEN][BOARD_LEN];
};



#include "pos_cal-TLT.h"

#ifdef FOO_TEST
#include "pos_cal_TEST.h"
#endif

#endif
