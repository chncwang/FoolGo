#ifndef FOOLGO_SRC_BOARD_BOARD_OUTPUT_H_
#define FOOLGO_SRC_BOARD_BOARD_OUTPUT_H_

#include "../def.h"

namespace foolgo {
namespace board {

const char BLACK_OUTPUT = 'x';
const char LAST_MOVE_BLACK_OUTPUT = 'X';
const char WHITE_OUTPUT = 'o';
const char LAST_MOVE_WHITE_OUTPUT = 'O';
const char EMPTY_OUTPUT = '.';

char GetPointStateOutput(PointState point_state, bool is_last_move);

}
}

#endif /* FOOLGO_SRC_BOARD_BOARD_OUTPUT_H_ */
