#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "Run.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    std::vector<Player::Types> selection;
    Datum *datum;

    selection.push_back(Player::Types::SynchronousPolicyIteration);
    selection.push_back(Player::Types::MonteCarloFirstVisit);
    selection.push_back(Player::Types::SynchronousValueIteration);

    Run r = Run(0, "input/maze-1.in", selection);
    r.performRun();
    datum = r.getResults();
    datum->writeDatumToFiles();
    return EXIT_SUCCESS;
}