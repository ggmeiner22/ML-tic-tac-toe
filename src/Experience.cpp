#include "Experience.h"
#include "Board.h"
#include "Player.h"
#include "FeatureEncoder.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

void Experience::trainFromTeacherFile(const std::string& path, Learner& learner, int gamesToUse) {
    std::ifstream fin(path);
    if (!fin) {
        std::cout << "ERROR: Cannot open teacher file: " << path << "\n";
        return;
    }

    std::string line;
    int used = 0;

    while (used < gamesToUse && std::getline(fin, line)) {
        if (line.empty()) continue;

        Board b; b.reset();
        int player = +1;

        // Store after-states separately for each side (same as before)
        std::vector<Board> statesX;
        std::vector<Board> statesO;

        std::stringstream ss(line);
        std::string tok;
        while (ss >> tok) {
            auto comma = tok.find(',');
            if (comma == std::string::npos) continue;

            int r = std::stoi(tok.substr(0, comma));
            int c = std::stoi(tok.substr(comma + 1));

            if (!b.place(r, c, player)) continue;

            if (player == +1) statesX.push_back(b);
            else statesO.push_back(b);

            if (b.terminal()) break;
            player = -player;
        }

        int w = b.winner();
        double targetX = (w==+1)? 1.0 : (w==0)? 0.0 : -1.0;
        double targetO = (w==-1)? 1.0 : (w==0)? 0.0 : -1.0;

        for (const auto& st : statesX) learner.update(FeatureEncoder::encode(st, +1), targetX);
        for (const auto& st : statesO) learner.update(FeatureEncoder::encode(st, -1), targetO);

        used++;
    }

    std::cout << "Teacher training done. Games used: " << used << "\n";
}

void Experience::trainSelfPlay(Learner& learner, int games) {
    // True self-play: the SAME learner controls both X and O.
    // Train from both sides and use successor-state bootstrapping:
    //   V_train(s) = V_hat(s') for nonterminal s, and terminal reward for terminal s.
    struct Sample {
        Board after; // after-state (board after the move)
        int p;       // player who just moved (+1 for X, -1 for O)
    };

    for (int g = 0; g < games; g++) {
        Board b; b.reset();

        std::vector<Sample> samples;
        samples.reserve(9);

        int player = +1; // X starts
        while (!b.terminal()) {
            // Both sides choose moves using the current evaluation function
            auto mv = Player::chooseBestMove(b, player, learner);
            b.place(mv.first, mv.second, player);

            // store after-state for the player who just moved
            samples.push_back({b, player});

            player = -player;
        }

        int w = b.winner();
        auto terminalValueFor = [&](int p) -> double {
            if (w == 0) return 0.0;
            return (w == p) ? 1.0 : -1.0;
        };

        // Bootstrapped LMS updates through the game
        // target for sample i is:
        //   terminal: outcome for that player
        //   nonterminal: predicted value of successor after-state, evaluated from same player's perspective
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
    }

    std::cout << "No-teacher (true self-play) training done. Games: " << games << "\n";
}
