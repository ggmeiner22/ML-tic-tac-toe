#include "FeatureEncoder.h"
#include <vector>

static int countTwoInRowWithEmpty(const Board& b, int p) {
    int count = 0;

    // rows
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

    // cols
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

    // main diag
    {
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
    }

    // anti diag
    {
        int pc = 0, ec = 0;
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
    }

    return count;
}

// Forks: number of moves that would create 2+ "two-in-row-with-empty" threats next turn.
static int countForkMoves(const Board& b, int p) {
    std::vector<std::pair<int,int> > moves = b.legalMoves();
    int forks = 0;

    for (size_t i = 0; i < moves.size(); i++) {
        int r = moves[i].first;
        int c = moves[i].second;

        Board nb = b;
        nb.place(r, c, p);

        // after making this move, how many immediate threats exist?
        int threats = countTwoInRowWithEmpty(nb, p);

        if (threats >= 2) {
            forks++;
        }
    }

    return forks;
}

// Counts the number of corner pieces by player p
static int countCorners(const Board& b, int p) {
    int cnt = 0;
    if (b.c[0][0] == p) {
        cnt++;
    }
    if (b.c[0][2] == p) {
        cnt++;
    }
    if (b.c[2][0] == p) {
        cnt++;
    }
    if (b.c[2][2] == p) {
        cnt++;
    }
    return cnt;
}

std::vector<double> FeatureEncoder::encode(const Board& b, int me) {
    int opp = -me;

    // x0..x9
    std::vector<double> x(NUM_FEATS, 0.0);

    // x0 bias
    x[0] = 1.0;

    // x1/x2 immediate threats
    x[1] = (double)countTwoInRowWithEmpty(b, me);
    x[2] = (double)countTwoInRowWithEmpty(b, opp);

    // x3/x4 forks
    x[3] = (double)countForkMoves(b, me);
    x[4] = (double)countForkMoves(b, opp);

    // x5 center control
    x[5] = (b.c[1][1] == me) ? 1.0 : 0.0;

    // x6/x7 corner control
    x[6] = (double)countCorners(b, me);
    x[7] = (double)countCorners(b, opp);

    // x8/x9 terminal indicators
    int w = b.winner();
    x[8] = (w == me)  ? 1.0 : 0.0;
    x[9] = (w == opp) ? 1.0 : 0.0;

    return x;
}
