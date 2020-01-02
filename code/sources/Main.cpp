#include <iostream>
#include <PolicyIterationPlayer.hpp>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "ValueIterationPlayer.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    Maze m = Maze();
    double gamma, theta;
    gamma = 1.0;
    theta = 1e-6; // 1e-2 * ((1.0 - gamma) / gamma);
    PolicyIterationPlayer pp = PolicyIterationPlayer(&m, gamma, theta);
    ValueIterationPlayer vp = ValueIterationPlayer(&m, gamma, theta);
    pp.solveMaze();
    printf("\n");
    vp.solveMaze();
    return EXIT_SUCCESS;
}