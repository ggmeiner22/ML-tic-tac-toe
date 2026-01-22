#pragma once
#include <vector>
#include "Board.h"
#include "Constants.h"

// FeatureEncoder converts a board position into a numeric feature vector
// from the perspective of a given player for use by the learning algorithm
struct FeatureEncoder {

    // Encode the board into a fixed-length feature vector.
    // `b` is the current board state.
    // `me` is the player perspective (+1 for X, -1 for O).
    // The returned vector is used by the learner to predict state value.
    static std::vector<double> encode(const Board& b, int me);
};
