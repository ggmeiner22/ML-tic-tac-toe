#include "Experience.h"
#include "Board.h"
#include "Player.h"
#include "FeatureEncoder.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>   

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

        // --- Extra terminal-board updates so terminal win/loss features actually learn ---
        if (w != 0) {
            // Winner sees this terminal board as a win
            learner.update(FeatureEncoder::encode(b, w), +1.0);
        
            // Loser sees this SAME terminal board as a loss
            learner.update(FeatureEncoder::encode(b, -w), -1.0);
        } else {
            // Tie: both sides see 0
            learner.update(FeatureEncoder::encode(b, +1), 0.0);
            learner.update(FeatureEncoder::encode(b, -1), 0.0);
        }


        used++; // game counter
    }

    std::cout << "Teacher training done. Games used: " << used << "\n";
}


void Experience::trainSelfPlay(Learner& learner, int games) {

    const double epsStart = 0.95;  // more exploration early
    const double epsEnd   = 0.02;  // less greedy near end


    struct Sample {
        Board after; // after-state (board after the move)
        int p;       // player who just moved (+1 for X, -1 for O)
    };

    // Seed RNG once
    static bool seeded = false;
    if (!seeded) {
        std::srand((unsigned)std::time(NULL));
        seeded = true;
    }

    for (int g = 0; g < games; g++) {
        Board b; b.reset();

        double t = (games <= 1) ? 1.0 : (double)g / (double)(games - 1);
        double epsilon = epsStart + (epsEnd - epsStart) * t; // linear decay

        std::vector<Sample> samples;
        samples.reserve(9);

        int player = +1; // X starts

        while (!b.terminal()) {
            std::vector<std::pair<int,int> > moves = b.legalMoves();
            if (moves.empty()) break;

            // Roll a random number in [0,1)
            double roll = (double)std::rand() / (double)RAND_MAX;

            std::pair<int,int> mv;

            if (roll < epsilon) {
                // Explore: random legal move
                int idx = std::rand() % (int)moves.size();
                mv = moves[idx];
            } else {
                // Exploit: greedy best move
                mv = Player::chooseBestMove(b, player, learner);
            }

            b.place(mv.first, mv.second, player);

            // Store after-state for the player who just moved
            samples.push_back(Sample{b, player});

            player = -player;
        }

        int w = b.winner(); // finds winner

        const Board terminalBoard = b;

        // assigns reward
        auto terminalValueFor = [&](int p) -> double {
            if (w == 0) {
                return 0.0;
            }
            return (w == p) ? 1.0 : -1.0;
        };

        // TD-style target: terminal outcome at end, otherwise use next state's prediction
        for (size_t i = 0; i < samples.size(); i++) {
            const Board& s = samples[i].after;
            int p = samples[i].p;

            double target;
            if (s.terminal() || i + 1 >= samples.size()) {
                target = terminalValueFor(p);
            } else {
                const Board& succ = samples[i + 1].after;
                target = learner.predict(FeatureEncoder::encode(succ, p));
            }

            learner.update(FeatureEncoder::encode(s, p), target);
        }
        if (w != 0) {
            int loser = -w; // if X won (+1), loser is O (-1), and vice versa

            // From the loser's perspective, the terminal board is a loss => target = -1
            learner.update(FeatureEncoder::encode(terminalBoard, loser), -1.0);
        }
    }

    std::cout << "No-teacher (true self-play, epsilon-greedy) training done. Games: " << games << "\n";
}

