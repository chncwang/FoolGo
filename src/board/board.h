#ifndef FOOLGO_SRC_BOARD_BOARD_H_
#define FOOLGO_SRC_BOARD_BOARD_H_

#include <boost/format.hpp>
#include <functional>
#include <cassert>
#include <cstring>
#include <ostream>
#include <string>

#include "../def.h"
#include "board_output.h"
#include "pos_cal.h"
#include "position.h"

namespace foolgo {
namespace board {

template<BoardLen BOARD_LEN>
class Board {
 public:
  Board() = default;
  virtual ~Board() {}
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(Board)
  virtual void Init();
  virtual void Copy(const Board<BOARD_LEN> &b);

  PointState GetPoint(PositionIndex index) const;
  PointState GetPoint(const Position &pos) const;

  void SetPoint(PositionIndex index, PointState point);
  void SetPoint(const Position &pos, PointState point);
 private:
  PointState points_[BoardLenSquare<BOARD_LEN>()];
};

namespace {
const char BLANK = ' ';
const char RETURN = '\n';
}

template<BoardLen BOARD_LEN>
inline void Board<BOARD_LEN>::Init() {
  memset(points_, EMPTY_POINT, sizeof(points_));
}

template<BoardLen BOARD_LEN>
inline void Board<BOARD_LEN>::Copy(const Board<BOARD_LEN> &b) {
  memcpy(points_, b.points_, sizeof(points_));
}

template<BoardLen BOARD_LEN>
inline PointState Board<BOARD_LEN>::GetPoint(PositionIndex index) const {
  assert(PstionAndIndxCcltr<BOARD_LEN>::Ins().IsInBoard(index));
  return points_[index];
}

template<BoardLen BOARD_LEN>
inline PointState Board<BOARD_LEN>::GetPoint(const Position &pos) const {
  PositionIndex index = PstionAndIndxCcltr<BOARD_LEN>::Ins().GetIndex(pos);
  return GetPoint(index);
}

template<BoardLen BOARD_LEN>
inline void Board<BOARD_LEN>::SetPoint(PositionIndex indx, PointState point) {
  assert(PstionAndIndxCcltr<BOARD_LEN>::Ins().IsInBoard(indx));
  points_[indx] = point;
}

template<BoardLen BOARD_LEN>
inline void Board<BOARD_LEN>::SetPoint(const Position &pos, PointState point) {
  BoardLen index = PstionAndIndxCcltr<BOARD_LEN>::Ins().GetIndex(pos);
  SetPoint(index, point);
}

template<BoardLen BOARD_LEN>
std::string ToString(std::function<std::string(PositionIndex)> get_output,
                     int width = 1) {
  int final_width = width % 2 == 1 ? width + 1 : width + 2;
  int blank_line_count = final_width / 2 - 1;

  std::string result = RETURN + std::string(2, BLANK);

  for (int i=0; i<BOARD_LEN; ++i) {
    result += (boost::format("%1% ") % i).str();
  }

  result += RETURN;

  for (int y=0; y<BOARD_LEN; ++y) {
    result += (boost::format("%1% ") % y).str();

    for (int x=0; x<BOARD_LEN; ++x) {
      PositionIndex position_index = PstionAndIndxCcltr<BOARD_LEN>::Ins()
          .GetIndex(Position(x, y));
      result += (boost::format("%1% ") % get_output(position_index)).str();
    }

    result += std::string(blank_line_count + 1, RETURN);
  }

  return result;
}

template<BoardLen BOARD_LEN>
std::string ToString(const Board<BOARD_LEN> &board) {
  static auto get_output = [&board](PositionIndex position_index) {
    return GetPointStateOutput(board.GetPoint(position_index), false);
  };
  return ToString<BOARD_LEN>(get_output);
}

template<BoardLen BOARD_LEN>
std::ostream &operator <<(std::ostream &os, const Board<BOARD_LEN> &board) {
  return os << ToString(board);
}

}
}

#endif
