#include <iostream>
#include "Maze.hpp"
#include "InvalidWidthException.hpp"

/**
 * Determines whether the given maze entry is a valid one.
 *
 * A maze entry is valid if it uses a legal symbol and it is not a
 * line-terminating character.
 *
 * @param entry The entry in the maze's map.
 * @return The question's answer.
 */
bool Maze::isValidEntry(char *entry) {
    return scanf("%c", entry) && *entry != '\n';
}

/**
 * Ensures the maze is kept at a constant width.
 *
 * @param current The width implied by the input of the current row.
 */
void Maze::ensureConstantWidth(int current) {
    if (this->getWidth() == 0) {
        this->setWidth(current);
    } else if (this->getWidth() != current && current != 0) {
        throw InvalidWidthException("A maze row must match in length to all previous rows.");
    }
}

/**
 * Reads an entry field of the maze from input, and stores it if it is valid.
 *
 * @param p The position of the entry in the maze.
 * @return A signal indicating whether the read was successful.
 */
bool Maze::readMazeEntryFromInput(Position p) {
    char entry;
    if (this->isValidEntry(&entry)) {
        this->entries.insert(std::pair<Position, char>(p, entry));
        return true;
    }
    return false;
}

/**
 * Reads and stores a row of the maze given by input.
 *
 * @param row The index of the row on which entries are attempted to be put.
 * @return An exit signal specifying whether the row could be read.
 */
bool Maze::readMazeRowFromInput(int row) {
    int col;
    col = 0;
    Position p = Position(col, row);
    while (this->readMazeEntryFromInput(p)) {
        col++;
        p = Position(col, row);
    }
    this->ensureConstantWidth(col);
    return col > 0;
}

/**
 *  Reads and stores a maze supplied by input.
 */
void Maze::readMazeFromInput() {
    int row;
    row = 0;
    while (this->readMazeRowFromInput(row)) {
        row++;
    }
    this->height = row;
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
 * Determines whether the specified location is within the bounds of this maze.
 *
 * @param x The X-coordinate of the proposed position.
 * @param y The Y-coordinate of the proposed position.
 * @return The question's answer.
 */
bool Maze::isWithinBounds(Position p) {
    return (p.getXCoordinate() >= 0 && p.getXCoordinate() < this->getWidth() &&
            p.getYCoordinate() >= 0 && p.getYCoordinate() < this->getHeight());
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
 * Sets the width of the maze.
 *
 * @param width The width to assign to the maze.
 */
void Maze::setWidth(int newWidth) {
    this->width = newWidth;
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
 * @throws out_of_range If the position is out of range.
 */
char Maze::getEntry(Position p) {
    if (this->isWithinBounds(p)) {
        return this->entries.at(p);
    } else {
        throw std::out_of_range("This maze position is out of range!\n");
    }
}