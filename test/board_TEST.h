#ifndef BOARD_TEST_TLT_H
#define BOARD_TEST_TLT_H

#include "board.h"


template<BoardLen BOARD_LEN>
void Board<BOARD_LEN>::PRINT() const
{
    printf("  ");
    for (int i=0; i<BOARD_LEN; ++i) printf("%d ", i);
    printf("\n");
    for (int y=0; y<BOARD_LEN; ++y) {
        printf("%d ", y);
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
    board.SetPoint(Position(3, 3), BLACK_POINT);
    board.SetPoint(Position(2, 4), WHITE_POINT);
    board.PRINT();
}


#endif
