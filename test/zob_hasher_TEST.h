#ifndef ZOB_HASHER_TEST_H
#define ZOB_HASHER_TEST_H

#include "zob_hasher.h"


template <BoardLen BOARD_LEN>
void ZobHasher<BOARD_LEN>::TEST()
{
    auto &ins = ZobHasher<BOARD_LEN>::Ins();
    ins.PRINT();
}


template <BoardLen BOARD_LEN>
void ZobHasher<BOARD_LEN>::PRINT()
{
    for (int i=0; i<BoardLenSquare<BOARD_LEN>(); ++i) {
        for (int j=0; j<3; ++j) {
            PRINT_LINE("i=%d, j=%d, hash= %u", i, j, board_hash_[i][j]);
        }
    }

    for (int i=0; i<2; ++i) PRINT_LINE("player hash = %u", player_hash_[i]);

    for (int i=0; i<BoardLenSquare<BOARD_LEN>(); ++i) {
        PRINT_LINE("i=%d, ko=%u", i, ko_hash_[i]);
    }

    PRINT_LINE("noko = %u", noko_hash_);
}


#endif
