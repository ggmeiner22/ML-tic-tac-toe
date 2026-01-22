#pragma once
#include <string>
#include "Learner.h"

// Experience encapsulates the training procedures for the learning agent
// It supports both supervised learning from teacher-provided games and
// unsupervised learning through self-play
struct Experience {

    // Train the learner using a .txt file of "teacher-provided" games
    // Each line in the file represents a complete game as a sequence of moves.
    // The learner updates its weights based on the final game outcome
    // Minimum 20 games
    static void trainFromTeacherFile(const std::string& path, Learner& learner, int gamesToUse = 20);

    // Train the learner using self-play (no teacher)
    // The same learner controls both players and updates its value function
    // based on the outcomes of games it generates on its own
    // Minimum 20 games
    static void trainSelfPlay(Learner& learner, int games = 20);
};
