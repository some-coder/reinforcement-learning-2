#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    Maze m = Maze();
    m.readMazeFromInput();
    return EXIT_SUCCESS;
}
