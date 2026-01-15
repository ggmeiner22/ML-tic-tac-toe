#pragma once
#include <string>
#include "Learner.h"

struct Experience {
    static void trainFromTeacherFile(const std::string& path, Learner& learner, int gamesToUse = 20);
    static void trainSelfPlay(Learner& learner, int games = 20);
};
