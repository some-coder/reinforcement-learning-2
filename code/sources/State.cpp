#include "State.hpp"

/**
 * Determines the legal actions that can be taken at this position in the maze.
 *
 * This method assumes a valid position has been chosen in the first place.
 *
 * @param m The maze in which the position is present.
 */
void State::determineLegalActions(Maze *m) {
    int i, position[2], deltas[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    char entry;
    for (i = 0; i < 4; i++) {
        position[0] = this->p.getXCoordinate() + deltas[i][0];
        position[1] = this->p.getYCoordinate() + deltas[i][1];
        Position d = Position(position[0], position[1]);
        if (!m->isWithinBounds(d)) {
            continue;
        }
        entry = m->getEntry(d);
        if (Maze::isTraversibleTile(entry)) {
            this->actions.push_back(directions(i));
        }
    }
}

State::State(const Position& pos, Maze *m) : p(pos) {
    this->determineLegalActions(m);
}

Position State::getPosition() {
    return this->p;
}

std::vector<State::directions> State::getActions() {
    return this->actions;
}