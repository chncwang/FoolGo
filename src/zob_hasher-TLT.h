#ifndef ZOB_HASHER_TLT_H
#define ZOB_HASHER_TLT_H

#include "zob_hasher.h"

#include <functional>

#include "rand.h"


template <BoardLen BOARD_LEN>
ZobHasher<BOARD_LEN> &ZobHasher<BOARD_LEN>::Ins()
{
    static ZobHasher<BOARD_LEN> ins;
    return ins;
}


template <BoardLen BOARD_LEN>
HashKey ZobHasher<BOARD_LEN>::GetHash(const BoardInGm<BOARD_LEN> &b) const
{
    HashKey result = 0;

    for (int i=0; i<BLSq<BOARD_LEN>(); ++i) {
        Point point = b.GetPoint(i);
        result ^= board_hash_[i][point];
    }

    PlayerColor last = b.LastPlayer();
    result ^= player_hash_[last];
    PointIndex ko_indx = b.KoIndex();
    result ^= (ko_indx == BoardInGm<BOARD_LEN>::NONE) ?
        noko_hash_ : ko_hash_[ko_indx];

    return result;
}


template <BoardLen BOARD_LEN>
ZobHasher<BOARD_LEN>::ZobHasher()
{
    uint32_t max = ~0;
    auto Rand = std::bind(g_Rand, max);

    for (int i=0; i<BLSq<BOARD_LEN>(); ++i) {
        for (int j=0; j<3; ++j) board_hash_[i][j] = Rand();
    }

    for (int i=0; i<2; ++i) player_hash_[i] = Rand();
    for (int i=0; i<BLSq<BOARD_LEN>(); ++i) ko_hash_[i] = Rand();
    noko_hash_ = Rand();
}


#endif
