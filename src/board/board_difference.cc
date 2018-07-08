#include "board_difference.h"

namespace foolgo {

void BoardDifference::ModifyToCurrentState(
    PositionIndex ko_position_index, PositionIndex move_position_index,
    bool is_last_move_suicide,
    const std::vector<PositionIndex> &removed_pieces_index) {
  last_force_difference_.current_state = OppositeForce(
      last_force_difference_.old_state);
  ko_index_difference_.current_state = ko_position_index;
  int removed_pieces_count = removed_pieces_index.size();
  indexes_difference_.resize(removed_pieces_count + 1);

  PointState removed_pieces_color =
      is_last_move_suicide ?
          last_force_difference_.current_state :
          last_force_difference_.old_state;

  for (int i = 0; i < removed_pieces_count; ++i) {
    PositionIndex removed_index = removed_pieces_index[i];
    indexes_difference_[i].Init(removed_index, removed_pieces_color,
                                EMPTY_POINT);
  }

  indexes_difference_[removed_pieces_count].Init(
      move_position_index, EMPTY_POINT, last_force_difference_.current_state);
}

}
