#include "Player.h"
#include "FeatureEncoder.h"

#include <utility>
#include <vector>

std::pair<int,int> Player::chooseBestMove(const Board& b, int me, const Learner& learner) {

    // Generate all legal moves for the current board state
    std::vector<std::pair<int,int> > moves = b.legalMoves();
    if (moves.empty()) {
        return std::make_pair(-1, -1);
    }

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
