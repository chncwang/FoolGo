#ifndef FOOLGO_SRC_DEEP_LEARNING
#define FOOLGO_SRC_DEEP_LEARNING

#include "board/full_board.h"

namespace foolgo {

template<BoardLen BOARD_LEN>
struct Sample {
  FullBoard<BOARD_LEN> full_board;
  PositionIndex position_index;
};

}

#endif
