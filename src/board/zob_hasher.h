#ifndef FOOLGO_SRC_BOARD_ZOB_HASHER_H_
#define FOOLGO_SRC_BOARD_ZOB_HASHER_H_

#include <functional>

#include "def.h"
#include "util/rand.h"
#include "board_difference.h"
#include "full_board_hasher.h"
#include "position.h"

namespace foolgo {

class BoardDifference;

template<BoardLen BOARD_LEN>
class ZobHasher : public FullBoardHasher<BOARD_LEN> {
 public:
  static void Init(uint32_t seed);
  static ZobHasher<BOARD_LEN> *InstancePtr() {
    return zob_hasher_ptr_;
  }

  ~ZobHasher() = default;

  HashKey GetHash(const FullBoard<BOARD_LEN> &b) const;
  HashKey GetHash(HashKey hash, const BoardDifference &chng) const;

 private:
  HashKey board_hash_[BoardLenSquare<BOARD_LEN>()][3];
  HashKey player_hash_[2];
  HashKey ko_hash_[BoardLenSquare<BOARD_LEN>()];
  HashKey noko_hash_;

  static ZobHasher<BOARD_LEN> *zob_hasher_ptr_;

  ZobHasher(uint32_t seed);

  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(ZobHasher)
};

template<BoardLen BOARD_LEN>
ZobHasher<BOARD_LEN> *ZobHasher<BOARD_LEN>::zob_hasher_ptr_ = nullptr;

template<BoardLen BOARD_LEN>
void ZobHasher<BOARD_LEN>::Init(uint32_t seed) {
  zob_hasher_ptr_ = new ZobHasher<BOARD_LEN>(seed);
}

template<BoardLen BOARD_LEN>
ZobHasher<BOARD_LEN>::ZobHasher(uint32_t seed) {
  uint32_t max = ~0 - 1;

  for (int i = 0; i < BoardLenSquare<BOARD_LEN>(); ++i) {
    for (int j = 0; j < 3; ++j) {
      board_hash_[i][j] = Rand(max, seed);
    }
  }

  for (int i = 0; i < 2; ++i) {
    player_hash_[i] = Rand(max, seed);
  }

  for (int i = 0; i < BoardLenSquare<BOARD_LEN>(); ++i) {
    ko_hash_[i] = Rand(max, seed);
  }

  noko_hash_ = Rand(max, seed);
}

template<typename T, typename GetHash>
inline static HashKey HashChange(const BoardDifference::Difference<T> &chng,
                                 const GetHash &get) {
  return get(chng.old_state) ^ get(chng.current_state);
}

template<BoardLen BOARD_LEN>
HashKey ZobHasher<BOARD_LEN>::GetHash(const FullBoard<BOARD_LEN> &b) const {
  HashKey result = 0;

  for (int i = 0; i < BoardLenSquare<BOARD_LEN>(); ++i) {
    PointState point = b.GetPointState(i);
    result ^= board_hash_[i][point];
  }

  Force last = b.LastForce();
  result ^= player_hash_[last];
  PositionIndex ko_indx = b.KoIndex();
  result ^=
      (ko_indx == FullBoard<BOARD_LEN>::NONE) ? noko_hash_ : ko_hash_[ko_indx];

  return result;
}

template<BoardLen BOARD_LEN>
HashKey ZobHasher<BOARD_LEN>::GetHash(HashKey hash,
                                      const BoardDifference &chng) const {
  auto getko = [this](PositionIndex ko_indx) {
    return (ko_indx == FullBoard<BOARD_LEN>::NONE) ?
    noko_hash_ : ko_hash_[ko_indx];
  };
  auto getplayer = [this](Force color) {
    return player_hash_[color];
  };
  HashKey r = hash ^ HashChange(chng.KoChng(), getko)
      ^ HashChange(chng.LastForceChng(), getplayer);

  for (const auto &pair : chng.PointsChng()) {
    PositionIndex pair_indx = pair.position_index;
    auto getpnt = [this, pair_indx](PointState pnt) {
      return board_hash_[pair_indx][pnt];
    };
    r ^= HashChange(pair.difference, getpnt);
  }

  return r;
}

}

#endif
