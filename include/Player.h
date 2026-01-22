#pragma once
#include <utility>
#include "Board.h"
#include "Learner.h"

struct Player {

    // Select the best legal move for player 'me' given the current board
    // Uses immediate win/block checks and then falls back to the learned
    // evaluation function to choose among remaining moves.
    static std::pair<int,int> chooseBestMove(const Board& b, int me, const Learner& learner);
};
