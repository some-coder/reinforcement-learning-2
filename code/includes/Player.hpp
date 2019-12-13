#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>
#include <vector>
#include "State.hpp"

class Player {
    private:
        std::map<State, double> stateValues;
        std::map<std::vector<State, State::directions>, double> actionValues;

    public:
        Player();
        virtual ~Player();
};

#endif