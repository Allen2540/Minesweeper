#include "Utils.hpp"
#include <cstdlib>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

void Utils::clearScreen() {
    system(CLEAR);
}
