#include "Play.h"
#include "Board.h"
#include "Player.h"
#include "Utils.h"

#include <iostream>
#include <string>

// Play a game between a user and the computer
// The computer selects moves using the learned evaluation function
int playHumanVsComputer(Learner& learner, bool humanFirst) {
    Board b; b.reset(); // resets board

    // Assign player roles: +1 is X, -1 is O
    int human = humanFirst ? +1 : -1;
    int comp  = -human;

    int player = +1; // X always moves first

    // Main game loop
    while (!b.terminal()) {
        b.print(); // print the board

        // Human move
        if (player == human) {
            int r, c;
            std::cout << "Enter your move (row col): ";
            if (!(std::cin >> r >> c)) { 
                clearInput(); 
                continue; 
            }
            if (!b.place(r, c, human)) {
                std::cout << "Invalid move.\n";
            } else {
                player = -player; // switch players
            }
        } else {
            // Computer move chosen by the learned policy
            auto mv = Player::chooseBestMove(b, comp, learner);
            b.place(mv.first, mv.second, comp); // places move
            std::cout << "Computer plays: " << mv.first << " " << mv.second << "\n";
            player = -player; // switch players
        }
    }

    b.print(); // prints board
    int w = b.winner(); // declare winner
    if (w == 0) { // tie
        std::cout << "Tie.\n"; 
        return 0;
    }
    if (w == human) {  // win
        std::cout << "You win.\n"; 
        return +1; 
    }
    // otherwise loss
    std::cout << "Computer wins.\n";
    return -1;
}
