#ifndef STATE_HPP
#define STATE_HPP

#include <vector>
#include "Position.hpp"
#include "Maze.hpp"

class State {
    public:
        enum direction {Up, Right, Down, Left};

    private:
        Position p;
        std::vector<direction> actions;
        void determineLegalActions(Maze *m);
        /* Agents map states to values */

    public:
        State(Position pos, Maze *m);
        bool operator<(const State & other) const;
        Position getPosition();
        std::vector<State::direction> getActions();
};

#endif