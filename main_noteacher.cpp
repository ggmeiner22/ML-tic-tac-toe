#include <iostream>
#include <string>

#include "Experience.h"
#include "Learner.h"
#include "Play.h"

// Entry point for no-teacher (self-play) training mode.
// Trains the learner using true self-play, prints learned weights,
// and then allows a human to play against the trained model.
int main() {
    // Create the learner and train via self-play
    Learner learner;
    Experience::trainSelfPlay(learner, 1000);
    learner.printWeights(); // Display learned weight values

    int wins=0, losses=0, ties=0;
    // Play loop
    while (true) {
        std::cout << "\nPlay a game? (y/n): ";
        std::string ans; std::cin >> ans;
        if (ans != "y" && ans != "Y") {
            break;
        }

        // Decide who plays first
        std::cout << "Go first? (y/n): ";
        std::string gf; std::cin >> gf;
        bool humanFirst = (gf == "y" || gf == "Y");

        // Play one game and record the outcome
        int result = playHumanVsComputer(learner, humanFirst);
        if (result == +1) {
            wins++;
        } else if (result == -1) {
            losses++;
        } else {
            ties++;
        }
        // print recorded W-L-T score
        std::cout << "Record (You): W=" << wins << " L=" << losses << " T=" << ties << "\n";
    }

    std::cout << "Final record (You): W=" << wins << " L=" << losses << " T=" << ties << "\n";
    return 0;
}
