#include "Position.hpp"

/**
 * Constructs a new position.
 *
 * @param x The horizontal coordinate of this point.
 * @param y The vertical coordinate of this point.
 */
Position::Position(int x, int y) {
    this->x = x;
    this->y = y;
}

/**
 * Determines whether this position should precede the other object.
 *
 * This method needs to be defined in order to function with maps, as keys of
 * maps need to be ordered (thus requiring a sense of order).
 *
 * @param other The other object.
 * @return The question's answer.
 */
bool Position::operator<(const Position & other) const {
    if (this->x < other.x) {
        return true;
    } else if (this->x == other.x) {
        return this->y < other.y;
    } else {
        return false;
    }
}

/**
 * Obtains the X-coordinate of this point.
 *
 * @return The X-coordinate.
 */
int Position::getXCoordinate() {
    return this->x;
}

/**
 * Obtains the Y-coordinate of this point.
 *
 * @return The Y-coordinate.
 */
int Position::getYCoordinate() {
    return this->y;
}