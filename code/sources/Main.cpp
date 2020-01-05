#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "Run.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    std::vector<Player::Types> selection;

    selection.push_back(Player::Types::SynchronousPolicyIteration);
    selection.push_back(Player::Types::MonteCarloFirstVisit);
    selection.push_back(Player::Types::SynchronousValueIteration);

    Run r = Run(1, selection);

    r.performRun();
    return EXIT_SUCCESS;
}