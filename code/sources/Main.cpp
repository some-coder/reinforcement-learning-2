#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "PolicyIterationPlayer.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    std::cout << "Starting to read the maze!" << std::endl;
    Maze m = Maze();
    std::cout << "Maze read!" << std::endl;
    PolicyIterationPlayer p = PolicyIterationPlayer(&m, 0.5, 0.01);
    std::cout << "Policy Iteration Player built!" << std::endl;
    std::cout << "Solving maze..." << std::endl;
    p.solveMaze();
    std::cout << "Maze solved!" << std::endl;
    p.printFinalPolicy();
    return EXIT_SUCCESS;
}