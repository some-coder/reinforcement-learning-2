#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <tuple>
#include <map>
#include "State.hpp"
#include "Maze.hpp"

class Player {
    protected:
        static constexpr double INITIAL_STATE_VALUE = 0.0;
        Maze* maze;
        double discountFactor;
        std::map<State*, double> stateValues;
        std::map<State*, std::vector<double>> policy;
        static std::vector<double> randomStatePolicy();
        void initialisePolicy();
        void initialiseStateValues();
        double actionProbability(State *s, Maze::Actions a);
        static std::vector<double> actionAsActionProbabilityDistribution(Maze::Actions a);

    public:
        Player(Maze* m, double gamma);
        virtual ~Player();
        virtual Maze::Actions chooseAction(State *s);
        virtual void solveMaze() = 0;
        std::map<State*, std::vector<double>> getPolicy();
        virtual void printFinalPolicy();
};

#endif