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

Defines project-wide constants: board size (N), number of features (NUM_FEATS), initial weight value (INIT_W), and learning rate (ALPHA). Keeps tuning parameters in one place.

### Board.h / Board.cpp

Implements the tic-tac-toe game board and game rules.
This module handles move placement, legality checks, win detection, terminal state detection, and board printing for interactive play.

### FeatureEncoder.h / FeatureEncoder.cpp

Encodes a board state into a fixed-length numerical feature vector from the perspective of a given player.
These features are used by the learning algorithm to evaluate board positions and includes: bias, threat counts (two-in-a-row with an empty third), fork counts, center control, corner control, and terminal win/loss indicators.

### Learner.h / Learner.cpp

Implements the learning component using a linear value function trained with the Least Mean Squares (LMS) update rule.
This module predicts the value of a board state and updates its weights during training based on observed outcomes.

### Player.h / Player.cpp

Defines the computer player’s move-selection policy.
It chooses the move that maximizes the learner’s predicted value of the resulting after-state (greedy evaluation)

### Experience.h / Experience.cpp

Handles training the learner in two modes:

1. Teacher training: reads games from teacher_games.txt, stores after-states for X and O, and updates the learner using the final game outcome as the target for each side.

2. True self-play: the same learner controls both players and updates values across successive after-states (includes an ε-greedy exploration to randomly select some board positions).

This module generates the training examples and applies learning updates.

### Play.h / Play.cpp

Manages interactive gameplay between a human and the trained computer player.
Handles user input, turn management, game progression, and result reporting.

### Utils.h / Utils.cpp

Provides utility functions used across the project, such as safely clearing invalid user input during interactive play.

### main_teacher.cpp

Program entry point for teacher-based training.
Loads a file of teacher games, trains the learner, prints learned weights, and allows interactive play against the trained model.

### main_noteacher.cpp

Program entry point for no-teacher (self-play) training.
Trains the learner using true self-play, prints learned weights, and allows interactive play against the trained model.

### teacher_games.txt

Contains sequences of moves representing teacher-provided games.
Each line corresponds to one game and is used to guide supervised learning during teacher-based training.

