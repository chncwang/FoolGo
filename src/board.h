#ifndef BOARD_H
#define BOARD_H

#include <def.h>
#include <pos_cal.h>
#include <position.h>
#include <types_and_constants.h>
#include <ostream>

inline PlayerColor OppstColor(PlayerColor color) {
  static const PlayerColor T[2] = { 1, 0 };
  return T[color];
}

template<BoardLen BOARD_LEN>
class Board {
 public:
  Board() = default;
  virtual ~Board() {
  }
  DISALLOW_COPY_AND_ASSIGN(Board)
  virtual void Init();
  virtual void Copy(const Board<BOARD_LEN> &b);

  Point GetPoint(PointIndex index) const;
  Point GetPoint(const Position &pos) const;

  void SetPoint(PointIndex index, Point point);
  void SetPoint(const Position &pos, Point point);

  inline PosCalculator<BOARD_LEN> &GetPosClcltr() const {
    return PosCalculator<BOARD_LEN>::Ins();
  }

 private:
  Point points_[BoardLenSquare<BOARD_LEN>()];
};

template<BoardLen BOARD_LEN>
std::ostream &operator <<(std::ostream &os, const Board<BOARD_LEN> &board);

#endif
