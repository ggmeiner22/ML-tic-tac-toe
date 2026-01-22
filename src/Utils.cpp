#include "Utils.h"

// Clear any error flags on std::cin and discard remaining input
// up to the next newline. 
// This is used to recover from invalid user input during interactive play
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
