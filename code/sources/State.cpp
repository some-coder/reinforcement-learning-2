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
        if (Maze::isTraversableTile(entry)) {
            this->actions.push_back(direction(i));
        }
    }
}

State::State(const Position& pos, Maze *m) : p(pos) {
    this->determineLegalActions(m);
}

bool State::operator<(const State & other) const {
    if (this->p < other.p) {
        return true;
    }
    if (this->actions < other.actions) {
        return true;
    }
    return false;
}

Position State::getPosition() {
    return this->p;
}

std::vector<State::direction> State::getActions() {
    return this->actions;
}