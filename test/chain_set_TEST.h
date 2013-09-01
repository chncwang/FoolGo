#ifndef CHAIN_SET_TEST_H
#define CHAIN_SET_TEST_H

#include <cstring>

#include "chain_set.h"


template <BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::PRINT() const
{
    PosCalculator<BOARD_LEN> &ins = this->GetPosClcltr();
    bool lists_exist[BoardLenSquare<BOARD_LEN>()];
    memset(lists_exist, false, sizeof(lists_exist));

    for (int y=0; y<BOARD_LEN; ++y) {
        for (int x=0; x<BOARD_LEN; ++x) {
            int index = ins.GetIndex(Position(x, y));
            int n = nodes_[index].next_;
            const Position &np = ins.GetPos(n);
            int nx = np.x_;
            int ny = np.y_;
            printf("{n=%d,%d ", nx, ny);
            int h = nodes_[index].list_head_;
            if (h == ChainSet<BOARD_LEN>::NONE_LIST) {
                printf("h=x,x} ");
            } else {
                lists_exist[h] = true;
                const Position &hp = ins.GetPos(h);
                int hx = hp.x_;
                int hy = hp.y_;
                printf("h=%d,%d} ",hx, hy);
            }
        }

        printf("\n\n");
    }

    for (int i=0; i<BoardLenSquare<BOARD_LEN>(); ++i) {
        if (lists_exist[i]) {
            const Position &pos = ins.GetPos(i);
            const Position &tp = ins.GetPos(lists_[i].tail_);
            printf("(%d, %d): tail_ = (%d, %d), len_ = %d, aircount = %d\n",
                   pos.x_,
                   pos.y_,
                   tp.x_,
                   tp.y_,
                   lists_[i].len_,
                   (int)lists_[i].air_set_.count());
        }
    }
}


template <BoardLen BOARD_LEN>
void ChainSet<BOARD_LEN>::TEST()
{
//    ChainSet<9> s;

//    Position a1(1, 1);
//    ChainSet<9>::AirSet air1;
//    for (int i=0; i<4; ++i) {
//        Position n = a1.AdjcntPos(i);
//        SetAir<9>(&air1, n);
//    }
//    s.AddPiece(a1, air1);

//    Position a2(1, 0);
//    ChainSet<9>::AirSet air2;
//    SetAir<9>(&air2, a2.AdjcntPos(1));
//    SetAir<9>(&air2, a2.AdjcntPos(3));
//    s.AddPiece(a2, air2);

//    Position a3(0, 2);
//    ChainSet<9>::AirSet air3;
//    SetAir<9>(&air3, a3.AdjcntPos(0));
//    SetAir<9>(&air3, a3.AdjcntPos(1));
//    SetAir<9>(&air3, a3.AdjcntPos(2));
//    s.AddPiece(a3, air3);

//    Position a4(1, 2);
//    ChainSet<9>::AirSet air4;
//    SetAir<9>(&air4, a4.AdjcntPos(1));
//    SetAir<9>(&air4, a4.AdjcntPos(2));
//    s.AddPiece(a4, air4);

//    s.PRINT();
//    ChainSet<9> sc = s;
//    sc.PRINT();

//    PRINT_LINE("air count = %d.", s.GetAirCountByPiece(1));
//    PntIndxArray v = s.GetPieces(18);
//    PntIndxArray::iterator it;
//    for (it=v.begin(); it!=v.end(); ++it) {
//        printf("%d, ", *it);
//    }
//    PRINT_LINE(" ");
}


#endif
