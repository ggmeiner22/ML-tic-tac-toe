#pragma once
#include <vector>
#include "Board.h"
#include "Constants.h"

struct FeatureEncoder {
    static std::vector<double> encode(const Board& b, int me);
};
