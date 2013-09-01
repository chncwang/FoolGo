#ifndef ENGINE_TEST_H
#define ENGINE_TEST_H

#include "engine.h"

#include <ctime>
#include <iostream>


template <BoardLen BOARD_LEN>
void Engine<BOARD_LEN>::TEST()
{
    Engine<BOARD_LEN> engine;
    engine.Init();

    while (true) {
        std::cout << engine.brdingm_;
        unsigned begin = clock();
        PointIndex nexti = engine.NextMove();
        unsigned end = clock();
        printf("time: %f\n\n", (float)(end - begin) / 1000000);
//        engine.PRINT_PROFITS(engine.brdingm_);
        engine.Play(nexti);
        std::cout << engine.brdingm_;
        int x, y;
        scanf("%d%d", &x, &y);
        engine.Play(engine.GetPosClcltr().GetIndex(Position(x, y)));
    }
}


template <BoardLen BOARD_LEN>
void Engine<BOARD_LEN>::PRINT_PROFITS(const BoardInGm<BOARD_LEN> &b) const
{
    HashKeyType bh = b.HashKey();
    auto item = table_[bh];
    for (int y=0; y<BOARD_LEN; ++y) {
        for (int x=0; x<BOARD_LEN; ++x) {
            PointIndex indx = this->GetPosClcltr().GetIndex(Position(x, y));
            Point p = b.GetPoint(indx);
            if (p == BLACK_POINT) printf("x  ");
            else if (p == WHITE_POINT) printf("o  ");
            else if (item.children_key_[indx] != 0)
                printf("%2.f ", table_[item.children_key_[indx]].avg_prft_ * 100);
            else printf(".  ");
        }
        printf("\n");
    }
    printf("\n");
}


#endif
