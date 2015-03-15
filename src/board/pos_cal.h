#ifndef POS_CAL_H
#define POS_CAL_H

#include <cassert>

#include "../def.h"
#include "position.h"

namespace foolgo {
namespace board {

template<BoardLen BOARD_LEN>
class PstionAndIndxCcltr {
 public:
  typedef enum {
    IN_CENTRAL = 0,
    IN_EDGE = 1,
    IN_CORNER = 2,
  } CentralEdgeCorner;
  static PstionAndIndxCcltr &Ins();

  inline const Position &GetPosition(PositionIndex index) const {
    assert(IsInBoard(index));
    return position_[index];
  }
  inline PositionIndex GetIndex(const Position &pos) const {
    assert(IsInBoard(pos));
    return indexes_[pos.y][pos.x];
  }

  inline bool IsInBoard(const Position &pos) const {
    return pos.x >= 0 && pos.x < BOARD_LEN && pos.y >= 0
        && pos.y < BOARD_LEN;
  }
  inline bool IsInBoard(PositionIndex indx) const {
    return indx >= 0 && indx < BoardLenSquare<BOARD_LEN>();
  }
  CentralEdgeCorner CentralOrEdgeOrCorner(const Position &pos);

 private:
  Position position_[BoardLenSquare<BOARD_LEN>()];
  PositionIndex indexes_[BOARD_LEN][BOARD_LEN];

  PstionAndIndxCcltr();
  ~PstionAndIndxCcltr() = default;
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(PstionAndIndxCcltr)
  void Init();
  CentralEdgeCorner CalPosStatus(BoardLen x, BoardLen y) const;
};

template<BoardLen BOARD_LEN>
inline PstionAndIndxCcltr<BOARD_LEN> &PstionAndIndxCcltr<BOARD_LEN>::Ins() {
  static PstionAndIndxCcltr<BOARD_LEN> ins;
  return ins;
}

template<BoardLen BOARD_LEN>
typename PstionAndIndxCcltr<BOARD_LEN>::CentralEdgeCorner PstionAndIndxCcltr<
    BOARD_LEN>::CentralOrEdgeOrCorner(const Position &pos) {
  if ((pos.x == 0 && pos.y == 0)
      || (pos.x == 0 && pos.y == BoardLenMinusOne<BOARD_LEN>())
      || (pos.x == BoardLenMinusOne<BOARD_LEN>() && pos.y == 0)
      || (pos.x == BoardLenMinusOne<BOARD_LEN>()
          && pos.y == BoardLenMinusOne<BOARD_LEN>())) {
    return IN_CORNER;
  } else if (pos.x > 0 && pos.x < BoardLenMinusOne<BOARD_LEN>() && pos.y > 0
      && pos.y < BoardLenMinusOne<BOARD_LEN>()) {
    return IN_CENTRAL;
  } else {
    return IN_EDGE;
  }
}

template<BoardLen BOARD_LEN>
PstionAndIndxCcltr<BOARD_LEN>::PstionAndIndxCcltr() {
  for (int y = 0; y < BOARD_LEN; ++y) {
    for (int x = 0; x < BOARD_LEN; ++x) {
      PositionIndex index = y * BOARD_LEN + x;
      indexes_[y][x] = index;
      position_[index].Set(x, y);
    }
  }
}

}
}

#endif
