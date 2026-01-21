# ML-tic-tac-toe

## Compilation and Execution
```
g++ -std=c++17 -O2 -Iinclude -o testNoTeacher   main_noteacher.cpp src/*.cpp
or
gcc -std=c++11 -O2 -Iinclude main_not_teacher.cpp src/*.cpp -lstdc++ -o testNoTeacher  

./main_noteacher

g++ -std=c++17 -O2 -Iinclude -o testTeacher   main_teacher.cpp src/*.cpp
or
gcc -std=c++11 -O2 -Iinclude main_teacher.cpp src/*.cpp -lstdc++ -o testTeacher

./main_teacher teacher_games.txt
```

## File Structure

