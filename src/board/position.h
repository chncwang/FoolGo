#ifndef FOOLGO_SRC_BOARD_POSITION_H_
#define FOOLGO_SRC_BOARD_POSITION_H_

#include <stdint.h>
#include <bitset>
#include <iostream>

namespace foolgo {
namespace board {

typedef char BoardLen;
typedef int16_t PositionIndex;

struct Position {
  static const BoardLen STRAIGHT_ORNTTIONS[4][2], OBLIQUE_ORNTTIONS[4][2];

  BoardLen x, y;

  Position() : x(0), y(0) {}
  Position(BoardLen x, BoardLen y) : x(x), y(y) {}

  void Set(BoardLen x, BoardLen y) {
    this->x = x;
    this->y = y;
  }
};

std::string PositionToString(const Position &position);

std::ostream &operator<<(std::ostream &os, const Position &position);

Position AdjacentPosition(const Position & position, int i);

Position ObliquePosition(const Position &position, int i);

template<BoardLen BOARD_LEN>
constexpr PositionIndex BoardLenSquare() {
  return BOARD_LEN * BOARD_LEN;
}

template<BoardLen BOARD_LEN>
using BitSet = std::bitset<BoardLenSquare<BOARD_LEN>()>;

template<BoardLen BOARD_LEN>
constexpr BoardLen BoardLenMinusOne() {
  return BOARD_LEN - 1;
}

template<BoardLen BOARD_LEN>
constexpr int16_t MoveCountUpperBoundPerGame() {
  return BoardLenSquare<BOARD_LEN>() * 4;
}

}
}

#endif
