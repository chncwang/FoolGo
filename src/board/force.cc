#include "force.h"

#include <cassert>

namespace foolgo {
namespace board {

using std::string;

Force OppositeForce(Force color) {
  return color == Force::BLACK_FORCE ? Force::WHITE_FORCE : Force::BLACK_FORCE;
}

string ForceToString(Force force) {
  return force == BLACK_FORCE ? "Black" : "White";
}

Force PointToForce(PointState point) {
  assert(point != EMPTY_POINT);

  return static_cast<Force>(point);
}

}
}
