#ifndef FOOLGO_SRC_GAME_GAME_INFO_H_
#define FOOLGO_SRC_GAME_GAME_INFO_H_

#include <vector>

#include "board/full_board.h"

namespace foolgo {

struct GameInfo {
  std::vector<Move> moves;
};

}

#endif
