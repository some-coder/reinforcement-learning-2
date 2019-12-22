#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"

int main(int argc, char *argv[]) {
    RandomServices::initialiseRandomServices();
    Maze m = Maze();
    State *s = m.getStartingState();
    std::cout << "(" << s->getX() << ", " << s->getY() << "): " << s->getType() << "." << std::endl;
    return EXIT_SUCCESS;
}