#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "Experiment.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    std::vector<Player::Types> selection;

    selection.push_back(Player::Types::SynchronousPolicyIteration);
    selection.push_back(Player::Types::MonteCarloFirstVisit);
    selection.push_back(Player::Types::SynchronousValueIteration);

    Experiment e = Experiment(selection, 3);
    e.conductExperiment();
    return EXIT_SUCCESS;
}