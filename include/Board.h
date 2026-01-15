#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include "Constants.h"

struct Board {
    // 0 = empty, +1 = X, -1 = O
    int c[N][N]{};

    void reset();
    bool isEmpty(int r, int col) const;
    bool place(int r, int col, int player);

    std::vector<std::pair<int,int>> legalMoves() const;

    int winner() const;    // +1 X wins, -1 O wins, 0 no winner
    bool full() const;
    bool terminal() const;

    void print() const;
};
