#ifndef TYPES_AND_CONSTANTS_H
#define TYPES_AND_CONSTANTS_H

#include <stdint.h>

typedef char PlayerColor;
const PlayerColor BLACK_PLAYER = 0;
const PlayerColor WHITE_PLAYER = 1;

//typedef uint32_t HashKeyType;
#define HashKeyType uint32_t

typedef char Point;
const Point BLACK_POINT = 0;
const Point WHITE_POINT = 1;
const Point EMPTY_POINT = 2;

#endif
