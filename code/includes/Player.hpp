#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>
#include <tuple>
#include "State.hpp"

class Player {
    private:
        std::map<State, double> stateValues;
        std::map<std::tuple<State, State::direction>, double> stateActionValues;

    public:
        Player();
        virtual ~Player();
        double getStateValue(State s);
        void setStateValue(State s, double newValue);
        double getStateActionValue(State s, State::direction d);
        void setStateActionValue(State s, State::direction d, double newValue);
};

#endif