#include <iostream>
#include "Maze.hpp"

int main(int argc, char *argv[]) {
    Maze m = Maze();
    m.readMazeFromInput();
    std::cout << "Height is: " << m.getHeight() << std::endl;
    std::cout << "Width is: " << m.getWidth() << std::endl;
    Position p = Position(1, 1);
    std::cout << "Random entry (1, 1): " << m.getEntry(p) << std::endl;
    return EXIT_SUCCESS;
}