#include "board_in_gm.h"

#include <cstdio>


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::TEST()
{
    BoardInGm<3> brd;
    brd.Init();
    FOO_PRINT_LINE("%d", (int)sizeof(brd));
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
        PointIndex index = brd.GetPosClcltr().GetIndex(Position(x, y));
        Move move = {color, index};
        if (brd.IsMoveSuiside(move)) {
            printf("suiside move.\n");
        } else {
            printf("legal move.\n");
            brd.PlayMove(move);
        color = ~color & 1;
        }
    }
}
