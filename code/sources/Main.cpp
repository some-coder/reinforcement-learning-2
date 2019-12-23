#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "PolicyIterationPlayer.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    Maze m = Maze();
    PolicyIterationPlayer p = PolicyIterationPlayer(&m, 1.0, 1e-3);
    p.solveMaze();
    return EXIT_SUCCESS;
}