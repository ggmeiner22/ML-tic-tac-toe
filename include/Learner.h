#pragma once
#include <vector>
#include <iostream>
#include "Constants.h"

// Learner implements a linear value function using Least Mean Squares (LMS)
// It predicts the value of a board state from features and updates weights
// based on observed targets during training
struct Learner {
    // Weight vector for the linear value function V̂(b) = w · x(b)
    std::vector<double> w;

    // Initialize all weights to a small constant value
    Learner();

    // Compute the predicted value of a board state given its feature vector
    double predict(const std::vector<double>& x) const;

    // Update weights using LMS learning rule toward the given target value
    // alpha controls the learning rate
    void update(const std::vector<double>& x, double target, double alpha = ALPHA);

    // Print the learned weight values
    void printWeights() const;
};
