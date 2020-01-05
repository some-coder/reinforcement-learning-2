#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "MonteCarloEveryVisitPlayer.hpp"

int main(int argc, char *argv[]) {
    Maze m = Maze("input/maze-1.in");
    RandomServices::initialiseRandomServices();
    MonteCarloEveryVisitPlayer p = MonteCarloEveryVisitPlayer(&m, 0.9, 1e4);
    p.solveMaze();
    p.printFinalPolicy();
    return EXIT_SUCCESS;
}