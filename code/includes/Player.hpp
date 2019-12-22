#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <tuple>
#include <map>
#include "State.hpp"
#include "Maze.hpp"

class Player {
    private:
        static constexpr double INITIAL_STATE_VALUE = 0.0;
        Maze* maze;
        double discountFactor;
        std::map<State*, double> stateValues;
        std::map<std::tuple<State*, Maze::Actions>, std::vector<double>> policy;
        void initialiseStateValues();

    public:
        Player(Maze* m, double d);
        virtual ~Player();
        virtual void solveMaze() = 0;
};

#endif