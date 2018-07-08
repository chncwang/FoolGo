#ifndef FOOLGO_SRC_PIECE_STRUCTRUE_EYE_SET_H_
#define FOOLGO_SRC_PIECE_STRUCTRUE_EYE_SET_H_

#include <bitset>

#include "../board/position.h"
#include "../def.h"

namespace foolgo {
namespace piece_structure {

template<BoardLen BOARD_LEN>
class EyeSet {
 public:
  EyeSet() = default;
  ~EyeSet() = default;
  void Copy(const EyeSet &es);

  void SetEye(PositionIndex indx, bool v) {
    eyes_[indx] = v;
  }
  void SetRealEye(PositionIndex indx, bool v) {
    real_eyes_[indx] = v;
  }
  bool IsEye(PositionIndex indx) const {
    return eyes_[indx];
  }
  bool IsRealEye(PositionIndex indx) const {
    return real_eyes_[indx];
  }
  PositionIndex RealCount() const {
    return real_eyes_.count();
  }

  std::vector<PositionIndex> GetRealEyes() const {
    return util::GetOnePositionIndexes<BoardLenSquare<BOARD_LEN>()>(
        real_eyes_);
  }

 private:
  BitSet<BOARD_LEN> eyes_;
  BitSet<BOARD_LEN> real_eyes_;
};

template<BoardLen BOARD_LEN>
bool IsFakeEye(const EyeSet<BOARD_LEN> &eyeset, PositionIndex indx) {
  return eyeset.IsEye(indx) && !eyeset.IsRealEye(indx);
}

template<BoardLen BOARD_LEN>
inline void EyeSet<BOARD_LEN>::Copy(const EyeSet &es) {
  eyes_ = es.eyes_;
  real_eyes_ = es.real_eyes_;
}

}
}

#endif
