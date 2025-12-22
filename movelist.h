#ifndef MOVELIST_H
#define MOVELIST_H

#include <cstdint>
#include "move.h"

struct MoveList
{
public:
    Move moveList[256];
    Move count;
};

#endif