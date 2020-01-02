#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "SynchronousPolicyIterationPlayer.hpp"
#include "AsynchronousPolicyIterationPlayer.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    Maze m = Maze();
    double gamma, theta;
    gamma = 0.9;
    theta = 1e-3; // 1e-2 * ((1.0 - gamma) / gamma);
    SynchronousPolicyIterationPlayer sp = SynchronousPolicyIterationPlayer(&m, gamma, theta);
    AsynchronousPolicyIterationPlayer ap = AsynchronousPolicyIterationPlayer(&m, gamma, theta, 1e3);
    sp.solveMaze();
    ap.solveMaze();
    return EXIT_SUCCESS;
}