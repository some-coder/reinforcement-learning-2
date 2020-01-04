#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "MonteCarloExploringStartsPlayer.hpp"

int main(int argc, char *argv[]) {
    Maze m = Maze();
    RandomServices::initialiseRandomServices();
    MonteCarloExploringStartsPlayer p = MonteCarloExploringStartsPlayer(&m, 1.0, 5e3);
    p.solveMaze();
    printf("FINAL POLICY:\n");
    p.printFinalPolicy();
    return EXIT_SUCCESS;
}