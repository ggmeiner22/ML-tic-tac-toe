#include "Player.h"
#include "FeatureEncoder.h"

#include <utility>
#include <vector>

// Implements the computer's move-selection strategy.
// win-first move selection
// 1) If we can win immediately, do it.
// 2) Else if opponent can win immediately next move, block it.
// 3) Else fall back to learned value (greedy on after-states).
std::pair<int,int> Player::chooseBestMove(const Board& b, int me, const Learner& learner) {

    // Generate all legal moves for the current board state
    std::vector<std::pair<int,int> > moves = b.legalMoves();
    if (moves.empty()) {
        return std::make_pair(-1, -1);
    }

    // 1) Win now if possible
    for (size_t i = 0; i < moves.size(); i++) {
        int r = moves[i].first;
        int c = moves[i].second;

        Board nb = b;
        nb.place(r, c, me);
        if (nb.winner() == me) { // if winner
            return std::make_pair(r, c);
        }
    }

    // 2) Block opponent's immediate win if needed
    int opp = -me;
    for (size_t i = 0; i < moves.size(); i++) {
        int r = moves[i].first;
        int c = moves[i].second;

        Board nb = b;
        nb.place(r, c, opp); // simulate opponent move
        if (nb.winner() == opp) { // if loser
            return std::make_pair(r, c); // block by playing here ourselves
        }
    }

    // 3) Otherwise, greedy using learned evaluation of after-states
    double bestVal = -1e18; // Initialize to a very small value so any move evaluation will be larger
    std::pair<int,int> best = moves[0];

    for (size_t i = 0; i < moves.size(); i++) {
        int r = moves[i].first;
        int c = moves[i].second;

        Board nb = b;
        nb.place(r, c, me);

        // Evaluate the after-state using the learned value function
        std::vector<double> x = FeatureEncoder::encode(nb, me);
        double v = learner.predict(x);

        // when best move is found, make it a pair
        if (v > bestVal) {
            bestVal = v;
            best = std::make_pair(r, c);
        }
    }

    return best;
}
