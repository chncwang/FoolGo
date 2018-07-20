#ifndef FOOLGO_SRC_BOARD_BOARD_DIFFERENCE_H_
#define FOOLGO_SRC_BOARD_BOARD_DIFFERENCE_H_

#include <vector>

#include "../def.h"
#include "force.h"
#include "position.h"

namespace foolgo {

class BoardDifference {
 public:
  template<typename T>
  struct Difference {
    T old_state, current_state;
  };

  struct DifferenceWithIndex {
    PositionIndex position_index;
    Difference<PointState> difference;

    inline void Init(PositionIndex position_index, PointState old_state,
                     PointState current_state) {
      this->position_index = position_index;
      difference.old_state = old_state;
      difference.current_state = current_state;
    }
  };

  BoardDifference() = default;
  ~BoardDifference() = default;
  void Init(Force last_force, PositionIndex ko_position_index);
  void ModifyToCurrentState(
      PositionIndex ko_position_index, PositionIndex move_position_index,
      bool is_last_move_suicide,
      const std::vector<PositionIndex> &removed_pieces_index);

  const Difference<PositionIndex> &KoChng() const {
    return ko_index_difference_;
  }
  const Difference<Force> &LastForceChng() const {
    return last_force_difference_;
  }
  const std::vector<DifferenceWithIndex> &PointsChng() const {
    return indexes_difference_;
  }

 private:
  Difference<PositionIndex> ko_index_difference_;
  Difference<Force> last_force_difference_;
  std::vector<DifferenceWithIndex> indexes_difference_;

  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(BoardDifference)
};

inline void BoardDifference::Init(Force last_force,
                                  PositionIndex ko_position_index) {
  ko_index_difference_.old_state = ko_position_index;
  last_force_difference_.old_state = last_force;
}

}

#endif
