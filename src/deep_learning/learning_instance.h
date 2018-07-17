#ifndef FOOLGO_SRC_DEEP_LEARNING
#define FOOLGO_SRC_DEEP_LEARNING

#include "full_board.h"

namespace foolgo {

struct LearningInstance {
  FullBoard full_board;
  PositionIndex move;
};

}

#endif
