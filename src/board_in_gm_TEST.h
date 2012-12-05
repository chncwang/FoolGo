#include "board_in_gm.h"

#include <cstdio>


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::TEST()
{
    BoardInGm<3> brd;
    PlayerColor color = BLACK_PLAYER;
    while (true) {
        brd.board_.PRINT();
        FOO_PRINT_LINE(" ");
        FOO_PRINT_LINE("black chain:\n");
        brd.chain_sets_[0].PRINT();
        FOO_PRINT_LINE("white chain:\n");
        brd.chain_sets_[1].PRINT();
        if (color == BLACK_PLAYER) {
            printf("black: ");
        } else {
            printf("white: ");
        }
        int x, y;
        scanf("%d%d", &x, &y);
        if (brd.IsMoveSuiside(color, Position(x, y))) {
            printf("suiside move.\n");
        } else {
            printf("legal move.\n");
            brd.Move(color, Position(x, y));
        color = ~color & 1;
        }
    }
}
