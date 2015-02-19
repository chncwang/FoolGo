#ifndef ZOB_HASHER_H
#define ZOB_HASHER_H

#include <board_in_gm.h>
#include <board-TLT.h>
#include <def.h>
#include <position.h>
#include <types_and_constants.h>

template<BoardLen BOARD_LEN>
class ZobHasher : public BrdInGmDlgt<BOARD_LEN> {
 public:
  DISALLOW_COPY_AND_ASSIGN(ZobHasher)
  ;
  static ZobHasher &Ins();

  HashKeyType GetHash(const BoardInGm<BOARD_LEN> &b) const;HashKeyType GetHash(
      HashKeyType hash, const BrdChange &chng) const;

 private:
  HashKeyType board_hash_[BoardLenSquare<BOARD_LEN>()][3];
  HashKeyType player_hash_[2];
  HashKeyType ko_hash_[BoardLenSquare<BOARD_LEN>()];
  HashKeyType noko_hash_;

  ZobHasher();
  ~ZobHasher() = default;
};

#endif
