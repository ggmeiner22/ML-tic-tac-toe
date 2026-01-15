#include "Experience.h"
#include "Board.h"
#include "Player.h"
#include "FeatureEncoder.h"

#include <fstream>
#include <sstream>
#include <iostream>

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
    for (int g = 0; g < games; g++) {
        Board b; b.reset();

        // We will collect training examples for BOTH sides:
        // Each entry is (after-state, playerWhoJustMoved)
        struct Sample { Board after; int player; };
        std::vector<Sample> samples;

        int player = +1; // X starts
        while (!b.terminal()) {
            // Learner chooses best move from this player's perspective
            auto mv = Player::chooseBestMove(b, player, learner);
            b.place(mv.first, mv.second, player);

            // store after-state for the player who just moved
            samples.push_back({b, player});

            // next turn
            player = -player;
        }

        // Now b is terminal. Define terminal values for each player.
        // (Use +1/-1/0 or +100/-100/0 based on your assignment.)
        int w = b.winner();
        auto terminalValueFor = [&](int p)->double {
            if (w == 0) return 0.0;        // tie
            if (w == p) return 100.0;        // p wins
            return -100.0;                   // p loses
        };

        // Train using Mitchell target:
        // For each after-state s_t (player p just moved), target is:
        // - terminalValueFor(p) if s_t is terminal
        // - V_hat(successor(s_t)) otherwise, where successor is the next after-state
        for (size_t i = 0; i < samples.size(); i++) {
            const Board& s = samples[i].after;
            int p = samples[i].player;

            double target = 0.0;
            if (s.terminal()) {
                target = terminalValueFor(p);
            } else {
                // successor after-state exists because s isn't terminal
                const Board& succ = samples[i + 1].after;

                // IMPORTANT: Evaluate successor from the SAME player's perspective (p),
                // because V(b) is "value of board for player p"
                auto x_succ = FeatureEncoder::encode(succ, p);
                target = learner.predict(x_succ);
            }

            // Update on current state from p's perspective
            auto x = FeatureEncoder::encode(s, p);
            learner.update(x, target);
        }
    }

    std::cout << "No-teacher (true self-play) training done. Games: " << games << "\n";
}
