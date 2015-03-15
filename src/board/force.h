#ifndef FOOLGO_SRC_BOARD_FORCE_H_
#define FOOLGO_SRC_BOARD_FORCE_H_

#include <iostream>

#include "../def.h"

namespace foolgo {
namespace board {

enum Force {
  BLACK_FORCE = 0,
  WHITE_FORCE = 1
};

Force OppositeForce(Force color);

std::string ForceToString(Force force);

Force PointToForce(PointState point);

}
}

#endif /* FOOLGO_SRC_BOARD_FORCE_H_ */
