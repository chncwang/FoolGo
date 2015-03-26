#include "position.h"

#include <boost/format.hpp>

namespace foolgo {
namespace board {

using boost::format;
using std::string;

const BoardLen Position::STRAIGHT_ORNTTIONS[4][2] = { { 0, -1 }, { 1, 0 }, { 0,
    1 }, { -1, 0 } };

const BoardLen Position::OBLIQUE_ORNTTIONS[4][2] = { { 1, -1 }, { 1, 1 }, { -1,
    1 }, { -1, -1 } };

string PositionToString(const Position &position) {
  return (boost::format("{%1%, %2%}") % static_cast<int>(position.x)
      % static_cast<int>(position.y)).str();
}

std::ostream &operator<<(std::ostream &os, const Position &position) {
  return os << PositionToString(position);
}

Position AdjacentPosition(const Position & position, int i) {
  return Position(position.x + Position::STRAIGHT_ORNTTIONS[i][0],
                  position.y + Position::STRAIGHT_ORNTTIONS[i][1]);
}

Position ObliquePosition(const Position &position, int i) {
  return Position(position.x + Position::OBLIQUE_ORNTTIONS[i][0],
                  position.y + Position::OBLIQUE_ORNTTIONS[i][1]);
}

}
}
