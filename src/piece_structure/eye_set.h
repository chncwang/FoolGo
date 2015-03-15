#ifndef FOOLGO_SRC_PIECE_STRUCTRUE_EYE_SET_H_
#define FOOLGO_SRC_PIECE_STRUCTRUE_EYE_SET_H_

#include <bitset>

#include "../board/position.h"
#include "../def.h"

namespace foolgo {
namespace piece_structure {

template<board::BoardLen BOARD_LEN>
class EyeSet {
 public:
  typedef std::bitset<board::BoardLenSquare<BOARD_LEN>()> BitSet;

  EyeSet() = default;
  ~EyeSet() = default;
  void Copy(const EyeSet &es);

  void SetEye(board::PositionIndex indx, bool v) {
    eyes_[indx] = v;
  }
  void SetRealEye(board::PositionIndex indx, bool v) {
    real_eyes_[indx] = v;
  }
  bool IsEye(board::PositionIndex indx) const {
    return eyes_[indx];
  }
  bool IsRealEye(board::PositionIndex indx) const {
    return real_eyes_[indx];
  }
  board::PositionIndex RealCount() const {
    return real_eyes_.count();
  }

  const BitSet &GetRealEyes() const {
    return real_eyes_;
  }

 private:
  BitSet eyes_;
  BitSet real_eyes_;

  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(EyeSet)
};

template<board::BoardLen BOARD_LEN>
bool IsFakeEye(const EyeSet<BOARD_LEN> &eyeset, board::PositionIndex indx) {
  return eyeset.IsEye(indx) && !eyeset.IsRealEye(indx);
}

template<board::BoardLen BOARD_LEN>
inline void EyeSet<BOARD_LEN>::Copy(const EyeSet &es) {
  eyes_ = es.eyes_;
  real_eyes_ = es.real_eyes_;
}

}
}

#endif
