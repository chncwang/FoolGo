#include "board_in_gm.h"

#include <cstdio>
#include <functional>

using namespace std;


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::PRINT_EYES() const
{
    printf("  ");
    for (int i=0; i<BOARD_LEN; ++i) printf("%d  ", i);
    printf("\n");
    function<void(PointIndex)> Print = [this](PointIndex indx) {
        if (board_.GetPoint(indx) != EMPTY_POINT) printf("xx ");
        else if (this->IsRealEye(Move(0, indx))) printf("br ");
        else if (this->IsEye(Move(0, indx))) printf("be ");
        else if (this->IsRealEye(Move(1, indx))) printf("wr ");
        else if (this->IsEye(Move(1, indx))) printf("we ");
        else printf("xx ");
    };

    for (int y=0; y<BOARD_LEN; ++y) {
        printf("%d ", y);

        for (int x=0; x<BOARD_LEN; ++x) {
            PointIndex indx = this->GetPosClcltr().GetIndex(Position(x, y));
            Print(indx);
        }

        printf("\n");
    }
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::TEST()
{
    BoardInGm<9> brd;
    brd.Init();
    FOO_PRINT_LINE("%d", (int)sizeof(brd));
    PlayerColor color = BLACK_PLAYER;
    while (true) {
        brd.board_.PRINT();
//        FOO_PRINT_LINE(" ");
//        FOO_PRINT_LINE("black chain:\n");
//        brd.chain_sets_[0].PRINT();
//        FOO_PRINT_LINE("white chain:\n");
//        brd.chain_sets_[1].PRINT();
        brd.PRINT_EYES();
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
