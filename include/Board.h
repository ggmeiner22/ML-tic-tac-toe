#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include "Constants.h"


// This struct encapsulates all game-state logic, including move legality,
// terminal-state detection, and printing for interactive play
// 0 = empty, +1 = X, -1 = O
struct Board {
    // 3x3 board configuration
    int c[N][N]{};

    // Reset the board to the initial empty state
    void reset();

    // Check whether a given board position is empty
    bool isEmpty(int r, int col) const;

    // Attempts to place a move for the given player at (r, col)
    // Returns true if the move is valid and successful, false otherwise
    bool place(int r, int col, int player);

    // Return a list of all legal (empty) board positions
    // Used by players and training algorithms to enumerate possible actions
    std::vector<std::pair<int,int>> legalMoves() const;

    // Determine the winner of the game.
    // +1 X wins, -1 O wins, 0 no winner
    int winner() const;

    // Check whether the board is completely full
    bool full() const;

    // Check whether the game has reached a terminal state (win or draw)
    bool terminal() const;

    // Print the current board state
    void print() const;
};
