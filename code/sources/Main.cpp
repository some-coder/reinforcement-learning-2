#include "RandomServices.hpp"
#include "Maze.hpp"
#include "Experiment.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    std::vector<Player::Types> playerSelection;
    std::vector<std::string> mazeSelection;

    playerSelection.push_back(Player::Types::SynchronousPolicyIteration);
    playerSelection.push_back(Player::Types::MonteCarloFirstVisit);
    playerSelection.push_back(Player::Types::SynchronousValueIteration);

    mazeSelection.emplace_back("input/maze-1.in");
    mazeSelection.emplace_back("");

    Experiment e = Experiment(playerSelection, mazeSelection);
    e.conductExperiment();
    return EXIT_SUCCESS;
}