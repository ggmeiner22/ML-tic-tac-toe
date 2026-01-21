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
### include Directory

### src Directory

### main_noteacher.cpp

### main_teacher.cpp

### teacher_games.txt

