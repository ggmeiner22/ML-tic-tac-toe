# ML-tic-tac-toe

## Compilation and Execution
#### To compile using gcc or on code01
```
gcc -std=c++11 -O2 -Iinclude main_noteacher.cpp src/*.cpp -lstdc++ -o testNoTeacher  
gcc -std=c++11 -O2 -Iinclude main_teacher.cpp src/*.cpp -lstdc++ -o testTeacher
```
#### Compile using g++
```
g++ -std=c++11 -O2 -Iinclude -o testNoTeacher   main_noteacher.cpp src/*.cpp
g++ -std=c++11 -O2 -Iinclude -o testTeacher   main_teacher.cpp src/*.cpp
```
#### Execution
```
./main_noteacher
./main_teacher teacher_games.txt
```
## File Structure
```
.
├── LICENSE
├── README.md
├── include
│   ├── Board.h
│   ├── Constants.h
│   ├── Experience.h
│   ├── FeatureEncoder.h
│   ├── Learner.h
│   ├── Play.h
│   ├── Player.h
│   └── Utils.h
├── main_noteacher.cpp
├── main_teacher.cpp
├── src
│   ├── Board.cpp
│   ├── Experience.cpp
│   ├── FeatureEncoder.cpp
│   ├── Learner.cpp
│   ├── Play.cpp
│   ├── Player.cpp
│   └── Utils.cpp
├── teacher_games.txt
├── testNoTeacher
└── testTeacher
```
## File Overview
### Constants.h

Defines global constants used throughout the project, including board size, number of features, initial weight values, and the learning rate. This centralizes configuration parameters for easy tuning and experimentation.

### Board.h / Board.cpp

Implements the tic-tac-toe game board and game rules.
This module handles move placement, legality checks, win detection, terminal state detection, and board printing for interactive play.

### FeatureEncoder.h / FeatureEncoder.cpp

Encodes a board state into a fixed-length numerical feature vector from the perspective of a given player.
These features are used by the learning algorithm to evaluate board positions and include piece counts, threat detection, and center control.

### Learner.h / Learner.cpp

Implements the learning component using a linear value function trained with the Least Mean Squares (LMS) update rule.
This module predicts the value of a board state and updates its weights during training based on observed outcomes.

### Player.h / Player.cpp

Defines the computer player’s move-selection policy.
The player first checks for immediate wins or necessary blocks, then uses the learned value function to choose the best remaining move.

### Experience.h / Experience.cpp

Handles training the learner using either:

1. Teacher-provided game data, or

2. True self-play, where the learner controls both players and learns from successive board states.

This module generates training examples and applies learning updates.

### Play.h / Play.cpp

Manages interactive gameplay between a human and the trained computer player.
Handles user input, turn management, game progression, and result reporting.

### Utils.h / Utils.cpp

Provides utility functions used across the project, such as safely clearing invalid user input during interactive play.

### main_teacher.cpp

Program entry point for teacher-based training.
Loads a file of teacher games, trains the learner, prints learned weights, and allows interactive play against the trained model.

### main_no_teacher.cpp (or equivalent self-play main)

Program entry point for no-teacher (self-play) training.
Trains the learner using true self-play, prints learned weights, and allows interactive play against the trained model.

### teacher_games.txt

Contains sequences of moves representing teacher-provided games.
Each line corresponds to one game and is used to guide supervised learning during teacher-based training.

