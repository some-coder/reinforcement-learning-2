#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "MonteCarloEveryVisitPlayer.hpp"

int main(int argc, char *argv[]) {
    Maze m = Maze();
    RandomServices::initialiseRandomServices();
    MonteCarloEveryVisitPlayer p = MonteCarloEveryVisitPlayer(&m, 0.9, 1e3);
    p.solveMaze();
    p.printFinalPolicy();
    return EXIT_SUCCESS;
}