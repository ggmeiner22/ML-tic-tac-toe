#pragma once
#include <vector>
#include <iostream>
#include "Constants.h"

struct Learner {
    std::vector<double> w;

    Learner();

    double predict(const std::vector<double>& x) const;
    void update(const std::vector<double>& x, double target, double alpha = ALPHA);

    void printWeights() const;
};
