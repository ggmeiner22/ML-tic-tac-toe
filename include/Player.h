#pragma once
#include <utility>
#include "Board.h"
#include "Learner.h"

struct Player {
    static std::pair<int,int> chooseBestMove(const Board& b, int me, const Learner& learner);
};
