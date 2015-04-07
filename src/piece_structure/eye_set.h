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

  std::vector<board::PositionIndex> GetRealEyes() const {
    return math::GetOnePositionIndexes<board::BoardLenSquare<BOARD_LEN>()>(
        real_eyes_);
  }

 private:
  board::BitSet<BOARD_LEN> eyes_;
  board::BitSet<BOARD_LEN> real_eyes_;

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
