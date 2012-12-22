#include "board_in_gm.h"

#include <cstdio>
#include <functional>
#include <vector>


static void PRINT_POS_VECTOR(const std::vector<Position> &v)
{
//    for (Position e : v) {
//        PRINT_LINE("%d %d", e.x_, e.y_);
//    }
}


template <BoardLen BOARD_LEN>
void BoardInGm<BOARD_LEN>::PRINT_EYES() const
{
    printf("  ");
    for (int i=0; i<BOARD_LEN; ++i) printf("%d  ", i);
    printf("\n");
    auto Print = [this](PointIndex indx) {
        if (this->GetPoint(indx) != EMPTY_POINT) printf("xx ");
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
void BoardInGm<BOARD_LEN>::PRINT_PLAYABLE() const
{
    printf("  ");
    for (int i=0; i<BOARD_LEN; ++i) printf("%d ", i);
    printf("\n");
    auto Print = [this](PointIndex indx) {
        Move bm(0, indx), wm(1, indx);
        bool bp = this->IsPlayable(bm);
        bool wp = this->IsPlayable(wm);
        if (bp && wp) printf("a ");
        else if (bp) printf("b ");
        else if (wp) printf("w ");
        else printf("x ");
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
    std::vector<Position> inputs;
    BoardInGm<9> brd;
//    auto &ins = brd.GetPosClcltr();
    brd.Init();
    PRINT_LINE("%d", (int)sizeof(brd));
    PlayerColor color = BLACK_PLAYER;
    while (true) {
        brd.PRINT();
//        PRINT_LINE(" ");
//        PRINT_LINE("black chain:\n");
//        brd.chain_sets_[0].PRINT();
//        PRINT_LINE("white chain:\n");
//        brd.chain_sets_[1].PRINT();
//        brd.PRINT_EYES();
//        for (int y=0; y<BOARD_LEN; ++y) {
//            for (int x=0; x<BOARD_LEN; ++x) {
//                PointIndex indx = ins.GetIndex(Position(x, y));
//                Point pnt = brd.board_.GetPoint(indx);
//                if (pnt != EMPTY_POINT) brd.UpdtAdjPlblIndxsOfChn(indx);
//            }
//        }
        brd.PRINT_PLAYABLE();
//        brd.PRINT_EYES();
        if (color == BLACK_PLAYER) {
            printf("black: ");
        } else {
            printf("white: ");
        }
        int x, y;
        scanf("%d%d", &x, &y);
        if (x == -1) {
            PRINT_POS_VECTOR(inputs);
            return;
        }
        inputs.push_back(Position(x, y));
        PointIndex index = brd.GetPosClcltr().GetIndex(Position(x, y));
        Move move = {color, index};
        if (brd.IsSuiside(move)) {
            printf("suiside move.\n");
        } else {
            printf("legal move.\n");
            brd.PlayMove(move);
        color = ~color & 1;
        }
    }
}
