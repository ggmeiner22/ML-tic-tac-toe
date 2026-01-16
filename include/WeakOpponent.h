#pragma once
#include <utility>
#include "Board.h"

struct WeakOpponent {
    static std::pair<int,int> chooseMove(const Board& b, int me);
};
