#ifndef STATE_HPP
#define STATE_HPP

#include <vector>
#include "Position.hpp"
#include "Maze.hpp"

class State {
    private:
        Position p;
        std::vector<directions> actions;
        void determineLegalActions(Maze *m);
        /* Agents map states to values */

    public:
        explicit State(const Position& pos, Maze *m);
        enum directions {Up, Right, Down, Left};
        Position getPosition();
        std::vector<State::directions> getActions();
};

#endif