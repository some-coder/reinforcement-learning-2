#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"
#include "SynchronousPolicyIterationPlayer.hpp"
#include "AsynchronousPolicyIterationPlayer.hpp"
#include "SynchronousValueIterationPlayer.hpp"
#include "AsynchronousValueIterationPlayer.hpp"

int main(int argc, char *argv[]) {
    double gamma, theta;
    int maximumIteration;
    Maze m = Maze();
    RandomServices::initialiseRandomServices();
    gamma = 0.9;
    theta = 1e-2 * ((1.0 - gamma) / gamma);
    maximumIteration = 1e3;
    SynchronousPolicyIterationPlayer spp = SynchronousPolicyIterationPlayer(&m, gamma, theta);
    AsynchronousPolicyIterationPlayer app = AsynchronousPolicyIterationPlayer(&m, gamma, theta, maximumIteration);
    SynchronousValueIterationPlayer svp = SynchronousValueIterationPlayer(&m, gamma, theta);
    AsynchronousValueIterationPlayer avp = AsynchronousValueIterationPlayer(&m, gamma, theta, maximumIteration);
    spp.solveMaze();
    app.solveMaze();
    svp.solveMaze();
    avp.solveMaze();
    printf("Synchronous Policy Iteration:\n"), spp.printFinalPolicy(), printf("\n");
    printf("Asynchronous Policy Iteration:\n"), app.printFinalPolicy(), printf("\n");
    printf("Synchronous Value Iteration:\n"), svp.printFinalPolicy(), printf("\n");
    printf("Asynchronous Value Iteration:\n"), avp.printFinalPolicy(), printf("\n");
    printf("Done with solving.\n");
    return EXIT_SUCCESS;
}