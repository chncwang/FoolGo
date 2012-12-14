#ifndef ZOB_HASHER_H
#define ZOB_HASHER_H

#include <stdint.h>

#include "def.h"
#include "position.h"
#include "board_in_gm.h"



template <BoardLen BOARD_LEN>
class ZobHasher : public BrdInGmDlgt<BOARD_LEN>
{
public:
    DISALLOW_COPY_AND_ASSIGN(ZobHasher);
    static ZobHasher &Ins();
    
    HashKey GetHash(const BoardInGm<BOARD_LEN> &b) const;

#ifdef FOO_TEST
    static void TEST();
    void PRINT();
#endif

private:
    HashKey board_hash_[BLSq<BOARD_LEN>()][3];
    HashKey player_hash_[2];
    HashKey ko_hash_[BLSq<BOARD_LEN>()];
    HashKey noko_hash_;

    ZobHasher();
    ~ZobHasher() = default;
};



#include "zob_hasher-TLT.h"
#ifdef FOO_TEST
#include "zob_hasher_TEST.h"
#endif

#endif
