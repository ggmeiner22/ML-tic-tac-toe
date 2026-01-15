#include "FeatureEncoder.h"
#include <vector>

std::vector<double> FeatureEncoder::encode(const Board& b, int me) {
    int opp = -me;
    std::vector<double> x(6, 0.0);

    x[0] = 1.0;

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (b.c[r][c] == me) x[1]++;
            else if (b.c[r][c] == opp) x[2]++;
        }
    }

    auto twoInRow = [&](int p) {
        int count = 0;

        for (int r = 0; r < 3; r++) {
            int pc = 0, ec = 0;
            for (int c = 0; c < 3; c++) {
                if (b.c[r][c] == p) pc++;
                else if (b.c[r][c] == 0) ec++;
            }
            if (pc == 2 && ec == 1) count++;
        }

        for (int c = 0; c < 3; c++) {
            int pc = 0, ec = 0;
            for (int r = 0; r < 3; r++) {
                if (b.c[r][c] == p) pc++;
                else if (b.c[r][c] == 0) ec++;
            }
            if (pc == 2 && ec == 1) count++;
        }

        int pc = 0, ec = 0;
        for (int i = 0; i < 3; i++) {
            if (b.c[i][i] == p) pc++;
            else if (b.c[i][i] == 0) ec++;
        }
        if (pc == 2 && ec == 1) count++;

        pc = ec = 0;
        for (int i = 0; i < 3; i++) {
            if (b.c[i][2 - i] == p) pc++;
            else if (b.c[i][2 - i] == 0) ec++;
        }
        if (pc == 2 && ec == 1) count++;

        return count;
    };

    x[3] = twoInRow(me);
    x[4] = twoInRow(opp);
    x[5] = (b.c[1][1] == me) ? 1.0 : 0.0;

    return x;
}
