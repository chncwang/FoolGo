#include "board_in_gm.h"


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::TEST()
{
    BoardInGm<9> brd;
    PlayerColor color = BLACK_PLAYER;
    while (true) {
        brd.board_.PRINT();
        FOO_PRINT_LINE(" ");
        if (color == BLACK_PLAYER) {
            printf("black: ");
        } else {
            printf("white: ");
        }
        int x, y;
        scanf("%d%d", &x, &y);
        brd.Move(color, Position(x, y));
        color = ~color & 1;
    }
}
