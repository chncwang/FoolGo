#ifndef FOOLGO_SRC_BOARD_BOARD_HASHER_H_
#define FOOLGO_SRC_BOARD_BOARD_HASHER_H_

#include "../def.h"
#include "full_board.h"
#include "position.h"

namespace foolgo {
namespace board {

class BoardDifference;
template<BoardLen BOARD_LEN> class FullBoard;

template<BoardLen BOARD_LEN>
class FullBoardHasher {
 public:
  virtual ~FullBoardHasher() = default;
  virtual HashKey GetHash(const FullBoard<BOARD_LEN> &b) const = 0;
  virtual HashKey GetHash(HashKey hash, const BoardDifference &change) const = 0;
};

}
}

#endif
