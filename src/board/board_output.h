#ifndef FOOLGO_SRC_BOARD_BOARD_OUTPUT_H_
#define FOOLGO_SRC_BOARD_BOARD_OUTPUT_H_

#include <iostream>

#include "../def.h"

namespace foolgo {

const char BLACK_OUTPUT = 'x';
const char LAST_MOVE_BLACK_OUTPUT = 'B';
const char WHITE_OUTPUT = 'o';
const char LAST_MOVE_WHITE_OUTPUT = 'W';
const char EMPTY_OUTPUT = '.';
const char LAST_MOVE_EMPTY_OUTPUT = '*';

std::string GetPointStateOutput(PointState point_state, bool is_last_move);

}

#endif /* FOOLGO_SRC_BOARD_BOARD_OUTPUT_H_ */
