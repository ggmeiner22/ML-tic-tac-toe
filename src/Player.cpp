#include "Player.h"
#include "FeatureEncoder.h"

std::pair<int,int> Player::chooseBestMove(const Board& b, int me, const Learner& learner) {
    auto moves = b.legalMoves();
    double bestVal = -1e9;
    std::pair<int,int> best = moves[0];

    for (auto [r,c] : moves) {
        Board nb = b;
        nb.place(r,c,me);
        auto x = FeatureEncoder::encode(nb, me);
        double v = learner.predict(x);
        if (v > bestVal) { bestVal = v; best = {r,c}; }
    }
    return best;
}
