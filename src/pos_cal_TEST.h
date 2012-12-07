#ifndef POS_CAL_TEST_H
#define POS_CAL_TEST_H

#include "pos_cal.h"


template<BoardLen BOARD_LEN>
void PosCalculator<BOARD_LEN>::TEST()
{
    PosCalculator<BOARD_LEN> &c = PosCalculator<BOARD_LEN>::Ins();
    for (int y=0; y<BOARD_LEN; ++y) {
        for (int x=0; x<BOARD_LEN; ++x) {
            printf("(%d, %d) : %d, ", x, y, c.GetIndex(Position(x, y)));
        }
    }
    printf("\n");

    for (int i=0; i<BoardLenSquare<BOARD_LEN>(); ++i) {
        printf("%d: (%d, %d), ", i, c.GetPos(i).x_, c.GetPos(i).y_);
    }
    printf("\n");
}


#endif
