#ifndef POS_CAL_H
#define POS_CAL_H

#include "def.h"
#include "position.h"



template <BoardLen BOARD_LEN>
class PosCalculator
{
public:
    inline static PosCalculator &Ins();

    const Position &GetPos(PointIndex index) const {return position_[index];}
    PointIndex GetIndex(const Position &pos) const {
        return indexes_[(int)pos.y_][(int)pos.x_];
    }

#ifdef FOO_TEST
    static void TEST();
#endif

protected:

private:
    explicit PosCalculator();
    ~PosCalculator();

    Position position_[BOARD_LEN * BOARD_LEN];
    PointIndex indexes_[BOARD_LEN][BOARD_LEN];
};



#include "pos_cal-TLT.h"
#include "pos_cal_TEST.h"

#endif
