#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"

int main(int argc, char *argv[]) {
    Maze m = Maze();
    RandomServices::initialiseRandomServices();
    return EXIT_SUCCESS;
}