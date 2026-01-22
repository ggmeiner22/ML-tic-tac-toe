#include "Experience.h"
#include "Board.h"
#include "Player.h"
#include "FeatureEncoder.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

void Experience::trainFromTeacherFile(const std::string& path, Learner& learner, int gamesToUse) {
    std::ifstream fin(path); // file input
    if (!fin) {
        std::cout << "ERROR: Cannot open teacher file: " << path << "\n";
        return;
    }

    std::string line;
    int used = 0;

    // Read up to the specified number of games
    while (used < gamesToUse && std::getline(fin, line)) {
        if (line.empty()) continue;

        Board b; b.reset(); // reset board
        int player = +1; // X always starts

        // Store after-states separately for each side
        std::vector<Board> statesX;
        std::vector<Board> statesO;

        // Break the line into individual move tokens of the form "row,column".
        std::stringstream ss(line);
        std::string tok;
        while (ss >> tok) {
            auto comma = tok.find(',');
            if (comma == std::string::npos) {
                continue;
            }

            int r = std::stoi(tok.substr(0, comma));
            int c = std::stoi(tok.substr(comma + 1));

            // Apply the move if valid
            if (!b.place(r, c, player)) {
                continue;
            }

            // Store the resulting board (after-state)
            if (player == +1) {
                statesX.push_back(b);
            } else {
                statesO.push_back(b);
            }

            // Stop if the game reaches a terminal state
            if (b.terminal()) {
                break;
            }
            // Switch player
            player = -player;
        }

        // Determine final game outcome
        int w = b.winner();

        // Set the learning target for each player.
        // Winning gives +1, losing gives -1, and a tie gives 0.
        double targetX = (w == +1) ? 1.0 : (w == 0) ? 0.0 : -1.0;
        double targetO = (w == -1) ? 1.0 : (w == 0) ? 0.0 : -1.0;

        // Update weights for all stored after-states
        for (const auto& st : statesX) {
            learner.update(FeatureEncoder::encode(st, +1), targetX);
        }
        for (const auto& st : statesO) {
            learner.update(FeatureEncoder::encode(st, -1), targetO);
        }

        used++; // game counter
    }

    std::cout << "Teacher training done. Games used: " << used << "\n";
}


// True self-play: the SAME learner controls both X and O.
// Train from both sides and use successor-state bootstrapping:
//   V_train(s) = V_hat(s') for nonterminal s, and terminal reward for terminal s.
void Experience::trainSelfPlay(Learner& learner, int games) {
   
    struct Sample {
        Board after; // after-state (board after the move)
        int p;       // player who just moved (+1 for X, -1 for O)
    };

    for (int g = 0; g < games; g++) {
        Board b; b.reset(); // reset board

        std::vector<Sample> samples;
        samples.reserve(9);

        int player = +1; // X starts
        while (!b.terminal()) {

            // Both sides choose moves using the current evaluation function
            auto mv = Player::chooseBestMove(b, player, learner);
            b.place(mv.first, mv.second, player);

            // store after-state for the player who just moved
            samples.push_back({b, player});

            player = -player; // switch player
        }

        // Determine final game outcome
        int w = b.winner();

        // Give the final reward for player p based on the game result.
        auto terminalValueFor = [&](int p) -> double {
            if (w == 0) {
                return 0.0;
            }
            return (w == p) ? 1.0 : -1.0;
        };

        // Loop over every stored position in the game
        for (size_t i = 0; i < samples.size(); i++) {
            const Board& s = samples[i].after; // the board after move i
            int p = samples[i].p;  // the player who just made that move

            double target;

            // If this state is terminal (or last stored move), use the final outcome
            if (s.terminal() || i + 1 >= samples.size()) {
                target = terminalValueFor(p);

            // Otherwise, use the learner’s predicted value of the next state
            } else { 
                const Board& succ = samples[i + 1].after; // the board after move i + 1
                target = learner.predict(FeatureEncoder::encode(succ, p));
            }

            // Update the weights so prediction for s moves toward target
            learner.update(FeatureEncoder::encode(s, p), target);
        }
    }

    std::cout << "No-teacher (true self-play) training done. Games: " << games << "\n";
}
