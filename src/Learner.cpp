#include "Learner.h"

// Initialize all weights to a small constant value
Learner::Learner() : w(NUM_FEATS, INIT_W) {}

// Predict the value of a board state by computing the dot product
// between the weight vector and the feature vector
double Learner::predict(const std::vector<double>& x) const {
    double v = 0.0;
    for (int i = 0; i < NUM_FEATS; i++) {
        v += w[i] * x[i];
    }
    return v;
}

void Learner::update(const std::vector<double>& x, double target, double alpha) {
    double pred = predict(x);
    double err = target - pred;  // difference between the target value and the predicted value
    for (int i = 0; i < NUM_FEATS; i++) {
        w[i] += alpha * err * x[i];  // Each weight is adjusted in proportion to the error and feature value.
    }
}

void Learner::printWeights() const {
    std::cout << "Weights:\n";
    for (int i = 0; i < NUM_FEATS; i++) {
        std::cout << "  w[" << i << "] = " << w[i] << "\n"; // prints each learned weight
    }
}
