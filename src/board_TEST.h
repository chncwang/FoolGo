#ifndef BOARD_TEST_TLT_H
#define BOARD_TEST_TLT_H

#include "board.h"

#ifdef FOO_TEST


template<BoardLen BOARD_LEN>
void Board<BOARD_LEN>::PRINT()
{
    for (int y=0; y<BOARD_LEN; ++y) {
        for (int x=0; x<BOARD_LEN; ++x) {
            Point point = this->GetPoint(Position(x, y));
            if (point == EMPTY_POINT) {
                printf(". ");
            } else if (point == BLACK_POINT) {
                printf("x ");
            } else if (point == WHITE_POINT) {
                printf("o ");
            } else {
                CANNOT_RUN_HERE();
            }
        }

        printf("\n");
    }
}


template <BoardLen BOARD_LEN>
void Board<BOARD_LEN>::TEST()
{
    Board<9> board;
    board.PRINT();
}


#endif

#endif
