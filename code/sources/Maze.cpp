#include "Maze.hpp"

/**
 * Reads and stores a row of the maze given by input.
 *
 * @param currentRow The index of the row on which entries are attempted to be put.
 * @return An exit signal, specifying whether the row could be read.
 */
bool Maze::readMazeRowFromInput(int currentRow) {
    char entry;
    int currentColumn;
    std::vector<int> coordinates;
    currentColumn = 0;
    while (scanf("%c", &entry) && entry != '\n') {
        /* Add the row. */
        coordinates.push_back(currentColumn);
        coordinates.push_back(currentRow);
        this->contents[coordinates] = entry;
        currentColumn++;
    }
    /* Update the maze width. */
    this->width = (this->width == 0 ? currentColumn : this->width);
    return currentColumn > 0;
}

/**
 *  Reads and stores a maze supplied by input.
 */
void Maze::readMazeFromInput() {
    int currentRow;
    currentRow = 0;
    while (this->readMazeRowFromInput(currentRow)) {
        currentRow++;
    }
    this->height = currentRow;
}

/**
 * Constructs a new maze.
 */
Maze::Maze() {
    this->width = 0;
    this->height = 0;
};

/**
 * Destructs the maze.
 */
Maze::~Maze() = default;

/**
 * Determines whether the specified location is within the bounds of the maze.
 *
 * @param x The X-coordinate of the proposed position.
 * @param y The Y-coordinate of the proposed position.
 * @return The question's answer.
 */
bool Maze::isWithinBounds(int x, int y) {
    return x >= 0 && x < this->width && y >= 0 && y < this->height;
}

/**
 * Obtains the width of the maze.
 *
 * @return The maze's width.
 */
int Maze::getWidth() {
    return this->width;
}

/**
 * Obtains the height of the maze.
 *
 * @return The maze's height.
 */
int Maze::getHeight() {
    return this->height;
}

/**
 * Obtains the entry in the maze at the specified location.
 *
 * If an invalid address is given, an exclamation mark '!' is returned.
 *
 * @param x The X-coordinate of the address.
 * @param y The Y-coordinate of the address.
 * @return The entry, if available.
 */
char Maze::getEntry(int x, int y) {
    std::vector<int> coordinates;
    if (this->isWithinBounds(x, y)) {
        coordinates.push_back(x);
        coordinates.push_back(y);
        return this->contents[coordinates];
    } else {
        return '!';
    }
}