#include "Player.h"
#include "FeatureEncoder.h"

// Option A: Tactical-first move selection
// 1) If we can win immediately, do it.
// 2) Else if opponent can win immediately next move, block it.
// 3) Else fall back to learned value (greedy on after-states).
std::pair<int,int> Player::chooseBestMove(const Board& b, int me, const Learner& learner) {
    auto moves = b.legalMoves();
    if (moves.empty()) return {-1, -1};

    // 1) Win now if possible
    for (auto [r, c] : moves) {
        Board nb = b;
        nb.place(r, c, me);
        if (nb.winner() == me) return {r, c};
    }

    // 2) Block opponent's immediate win if needed
    int opp = -me;
    for (auto [r, c] : moves) {
        Board nb = b;
        nb.place(r, c, opp);         // pretend opponent plays here
        if (nb.winner() == opp) {
            return {r, c};           // block by playing here ourselves
        }
    }

    // 3) Otherwise, greedy using learned evaluation of after-states
    double bestVal = -1e18;
    std::pair<int,int> best = moves[0];

    for (auto [r, c] : moves) {
        Board nb = b;
        nb.place(r, c, me);
        auto x = FeatureEncoder::encode(nb, me);
        double v = learner.predict(x);
        if (v > bestVal) {
            bestVal = v;
            best = {r, c};
        }
    }

    return best;
}
