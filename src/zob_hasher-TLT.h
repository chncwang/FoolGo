#ifndef ZOB_HASHER_TLT_H
#define ZOB_HASHER_TLT_H

#include "zob_hasher.h"

#include <functional>

#include "rand.h"


template <typename T, typename GetHash>
inline static HashKeyType
HashChange(const BrdChange::Change<T> &chng, const GetHash &get)
{
    return get(chng.origin_) ^ get(chng.now_);
}


template <BoardLen BOARD_LEN>
ZobHasher<BOARD_LEN> &ZobHasher<BOARD_LEN>::Ins()
{
    static ZobHasher<BOARD_LEN> ins;
    return ins;
}


template <BoardLen BOARD_LEN>
HashKeyType ZobHasher<BOARD_LEN>::GetHash(const BoardInGm<BOARD_LEN> &b) const
{
    HashKeyType result = 0;

    for (int i=0; i<BoardLenSquare<BOARD_LEN>(); ++i) {
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
HashKeyType
ZobHasher<BOARD_LEN>::GetHash(HashKeyType hash, const BrdChange &chng) const
{
    auto getko = [this](PointIndex ko_indx) {
        return (ko_indx == BoardInGm<BOARD_LEN>::NONE) ?
            noko_hash_ : ko_hash_[ko_indx];
    };
    auto getplayer = [this](PlayerColor color) {
        return player_hash_[color];
    };
    HashKeyType r = hash ^ HashChange(chng.KoChng(), getko) ^
                HashChange(chng.LastPlayerChng(), getplayer);

    for (auto pair : chng.PointsChng()) {
        PointIndex pair_indx = pair.indx_;
        auto getpnt = [this, pair_indx](Point pnt) {
            return board_hash_[pair_indx][pnt];
        };
        r ^= HashChange(pair.pnt_, getpnt);
    }

    return r;
}


template <BoardLen BOARD_LEN>
ZobHasher<BOARD_LEN>::ZobHasher()
{
    uint32_t max = ~0;
    auto LRand = std::bind(Rand, max);

    for (int i=0; i<BoardLenSquare<BOARD_LEN>(); ++i) {
        for (int j=0; j<3; ++j) board_hash_[i][j] = LRand();
    }

    for (int i=0; i<2; ++i) player_hash_[i] = LRand();
    for (int i=0; i<BoardLenSquare<BOARD_LEN>(); ++i) ko_hash_[i] = LRand();
    noko_hash_ = LRand();
}


#endif
