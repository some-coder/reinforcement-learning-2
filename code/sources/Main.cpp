#include "RandomServices.hpp"
#include "Maze.hpp"
#include "UserInput.hpp"

/**
 * Executes the program.
 *
 * @param argc The number (count) of command-line arguments.
 * @param argv A vector of command-line arguments.
 * @return The exit signal.
 */
int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    Experiment e = UserInput::experimentFromUserInput();
    e.conductExperiment();
    return EXIT_SUCCESS;
}
