#include "FeatureEncoder.h"
#include <vector>

std::vector<double> FeatureEncoder::encode(const Board& b, int me) {
    int opp = -me;

    // Feature vector:
    // x[0] = bias term
    // x[1] = number of player's marks
    // x[2] = number of opponent's marks
    // x[3] = number of two-in-a-row opportunities for player
    // x[4] = number of two-in-a-row opportunities for opponent
    // x[5] = center control indicator
    std::vector<double> x(6, 0.0);

    // Bias feature
    x[0] = 1.0;

    // Count player and opponent marks on the board
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (b.c[r][c] == me) { // count mine
                x[1]++;
            } else if (b.c[r][c] == opp) {  // count opp
                x[2]++;
            }
        }
    }

    // Count two-in-a-row threats for a given player
    // pc = number of player marks in the line
    // ec = number of empty squares in the line
    auto twoInRow = [&](int p) {
        int count = 0;

        // Check rows
        for (int r = 0; r < 3; r++) {
            int pc = 0, ec = 0;
            for (int c = 0; c < 3; c++) {
                if (b.c[r][c] == p) {
                    pc++;
                } else if (b.c[r][c] == 0) {
                    ec++;
                }
            }
            if (pc == 2 && ec == 1) {
                count++;
            }
        }

        // Check columns
        for (int c = 0; c < 3; c++) {
            int pc = 0, ec = 0;
            for (int r = 0; r < 3; r++) {
                if (b.c[r][c] == p) {
                    pc++;
                } else if (b.c[r][c] == 0) {
                    ec++;
                }
            }
            if (pc == 2 && ec == 1) {
                count++;
            }
        }

        // Check main diagonal
        int pc = 0, ec = 0;
        for (int i = 0; i < 3; i++) {
            if (b.c[i][i] == p) {
                pc++;
             } else if (b.c[i][i] == 0) {
                ec++;
             }
        }
        if (pc == 2 && ec == 1) {
            count++;
        }

        // Check anti-diagonal
        pc = ec = 0;
        for (int i = 0; i < 3; i++) {
            if (b.c[i][2 - i] == p) {
                pc++;
            } else if (b.c[i][2 - i] == 0) {
                ec++;
            }
        }
        if (pc == 2 && ec == 1) {
            count++;
        }
        return count;
    };

    // Player and opponent two-in-a-row threats
    x[3] = twoInRow(me);
    x[4] = twoInRow(opp);

    // Center control feature
    x[5] = (b.c[1][1] == me) ? 1.0 : 0.0;

    return x;
}
