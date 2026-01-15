#include "Learner.h"

Learner::Learner() : w(NUM_FEATS, INIT_W) {}

double Learner::predict(const std::vector<double>& x) const {
    double v = 0.0;
    for (int i=0;i<NUM_FEATS;i++) v += w[i] * x[i];
    return v;
}

void Learner::update(const std::vector<double>& x, double target, double alpha) {
    double pred = predict(x);
    double err = target - pred;
    for (int i=0;i<NUM_FEATS;i++) w[i] += alpha * err * x[i];
}

void Learner::printWeights() const {
    std::cout << "Weights:\n";
    for (int i=0;i<NUM_FEATS;i++) std::cout << "  w[" << i << "] = " << w[i] << "\n";
}
