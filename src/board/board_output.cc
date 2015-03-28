#include "board_output.h"

#include <cassert>

#include "../def.h"

namespace foolgo {
namespace board {

using std::string;

string GetPointStateOutput(PointState point_state, bool is_last_move) {
  char output = EMPTY_OUTPUT;

  if (point_state == BLACK_POINT) {
    output = is_last_move ? LAST_MOVE_BLACK_OUTPUT : BLACK_OUTPUT;
  } else if (point_state == WHITE_POINT) {
    output = is_last_move ? LAST_MOVE_WHITE_OUTPUT : WHITE_OUTPUT;
  } else if (point_state == EMPTY_POINT) {
    output = EMPTY_OUTPUT;
  } else {
    assert(false);
  }

  return string(1, output);
}

}
}
