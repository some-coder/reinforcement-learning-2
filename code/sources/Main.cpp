#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "Run.hpp"
#include "SynchronousValueIterationPlayer.hpp"

int main(int argc, char *argv[]) {
    Maze m = Maze("input/maze-1.in");
    RandomServices::initialiseRandomServices();
    std::map<Player::Types, bool> presentList;
    presentList[Player::Types::SynchronousPolicyIteration] = true;
    presentList[Player::Types::AsynchronousPolicyIteration] = true;
    Run r = Run(1, &presentList);
    printf("Survived construction!\n");
    r.performRun();
    return EXIT_SUCCESS;
}