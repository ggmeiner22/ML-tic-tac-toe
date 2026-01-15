#include "Play.h"
#include "Board.h"
#include "Player.h"
#include "Utils.h"

#include <iostream>
#include <string>

int playHumanVsComputer(Learner& learner, bool humanFirst) {
    Board b; b.reset();
    int human = humanFirst ? +1 : -1;
    int comp  = -human;

    int player = +1;
    while (!b.terminal()) {
        b.print();
        if (player == human) {
            int r,c;
            std::cout << "Enter your move (row col): ";
            if (!(std::cin >> r >> c)) { clearInput(); continue; }
            if (!b.place(r,c,human)) std::cout << "Invalid move.\n";
            else player = -player;
        } else {
            auto mv = Player::chooseBestMove(b, comp, learner);
            b.place(mv.first, mv.second, comp);
            std::cout << "Computer plays: " << mv.first << " " << mv.second << "\n";
            player = -player;
        }
    }

    b.print();
    int w = b.winner();
    if (w == 0) { std::cout << "Tie.\n"; return 0; }
    if (w == human) { std::cout << "You win.\n"; return +1; }
    std::cout << "Computer wins.\n";
    return -1;
}
