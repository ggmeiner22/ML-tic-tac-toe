#include "Board.h"

void Board::reset() {
    for (int r = 0; r < N; r++) {
        for (int col = 0; col < N; col++) {
            c[r][col] = 0; // All cells are set to 0 (empty)
        }
    }
}

bool Board::isEmpty(int r, int col) const {
    return c[r][col] == 0;
}

bool Board::place(int r, int col, int player) {
    if (r < 0 || r >= N || col < 0 || col >= N) {  // out of bounds
        return false;
    }
    if (!isEmpty(r, col)) {  // empty
        return false;
    }
    // otherwise valid
    c[r][col] = player;
    return true;
}

std::vector<std::pair<int,int>> Board::legalMoves() const {
    std::vector<std::pair<int,int>> moves;
    for (int r = 0; r < N; r++) { // rows
        for (int col = 0; col < N; col++) { // columns
            if (c[r][col] == 0) {  // if empty
                moves.push_back({r, col}); // possible move
            }
        }
    }
    return moves;
}

int Board::winner() const {
    // Check rows and columns
    for (int i = 0; i < N; i++){
        int rs = c[i][0] + c[i][1] + c[i][2];
        if (rs == 3) return +1;
        if (rs == -3) return -1;

        int cs = c[0][i] + c[1][i] + c[2][i];
        if (cs == 3) return +1;
        if (cs == -3) return -1;
    }

    // Check diagonals
    int d1 = c[0][0] + c[1][1] + c[2][2];
    if (d1 == 3) return +1;
    if (d1 == -3) return -1;

    int d2 = c[0][2] + c[1][1] + c[2][0];
    if (d2 == 3) return +1;
    if (d2 == -3) return -1;

    return 0;
}

bool Board::full() const {
    for (int r = 0; r < N; r++) {
        for (int col = 0; col < N; col++) {
            if (c[r][col] == 0) { // if empty spot, not full
                return false;
            }
        }
    }
    return true;
}

bool Board::terminal() const {
    return winner() != 0 || full();  // if winner or full -> T else F
}

void Board::print() const {
    std::cout << "  0 1 2\n";
    for (int r = 0; r < N; r++){
        std::cout << r << " ";
        for (int col = 0; col < N; col++){
            // X for first player, O for second player, else empty space
            char ch = (c[r][col] == +1) ? 'X' : (c[r][col]==-1) ? 'O' : ' ';
            std::cout << ch;

            // vertical lines
            if (col < 2) {
                std::cout << "|";
            }
        }
        std::cout << "\n";
        // horizontal lines
        if (r < 2) {
            std::cout << "  -+-+-\n";
        }
    }
}
