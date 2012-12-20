#include "position.h"

const BoardLen Position::STRAIGHT_ORNTTIONS[4][2] = {
    {0, -1},
    {1, 0},
    {0, 1},
    {-1, 0}
};

const BoardLen Position::OBLIQUE_ORNTTIONS[4][2] = {
    {1, -1},
    {1, 1},
    {-1, 1},
    {-1, -1}
};
