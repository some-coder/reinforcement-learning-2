#include "RandomServices.hpp"
#include "Maze.hpp"
#include "UserInput.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    Experiment e = UserInput::experimentFromUserInput();
    e.conductExperiment();
    return EXIT_SUCCESS;
}