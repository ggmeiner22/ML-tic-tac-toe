#include <iostream>
#include <string>

#include "Experience.h"
#include "Learner.h"
#include "Play.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: ./testTeacher <teacher_file>\n";
        return 1;
    }

    Learner learner;
    Experience::trainFromTeacherFile(argv[1], learner, 20);
    learner.printWeights();

    int wins=0, losses=0, ties=0;
    while (true) {
        std::cout << "\nPlay a game? (y/n): ";
        std::string ans; std::cin >> ans;
        if (ans != "y" && ans != "Y") break;

        std::cout << "Go first? (y/n): ";
        std::string gf; std::cin >> gf;
        bool humanFirst = (gf == "y" || gf == "Y");

        int result = playHumanVsComputer(learner, humanFirst);
        if (result == +1) wins++;
        else if (result == -1) losses++;
        else ties++;

        std::cout << "Record (You): W=" << wins << " L=" << losses << " T=" << ties << "\n";
    }

    std::cout << "Final record (You): W=" << wins << " L=" << losses << " T=" << ties << "\n";
    return 0;
}
