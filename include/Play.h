#pragma once
#include "Learner.h"

// Play a game of tic-tac-toe against the computer.
// `learner` provides the computer's learned evaluation function.
// `humanFirst` determines whether the human plays first.
// Returns +1 if the human wins, -1 if the computer wins, or 0 for a tie.
int playHumanVsComputer(Learner& learner, bool humanFirst);
