#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <cmath>
#include <chrono>
#include "State.hpp"
#include "Maze.hpp"

class Player {
    public:
        enum Types {SynchronousPolicyIteration, AsynchronousPolicyIteration, SynchronousValueIteration,
                AsynchronousValueIteration, MonteCarloExploringStarts, MonteCarloFirstVisit, MonteCarloEveryVisit};
        static constexpr int PLAYER_TYPE_AMOUNT = 7;

    protected:
        static constexpr double INITIAL_STATE_VALUE = 0.0;
        static constexpr int PRESENCE_SCORE_MAXIMUM = 10;
        Maze* maze;
        double discountFactor;
        std::map<State*, double> stateValues;
        std::map<State*, std::vector<double>> policy;
        std::vector<double> epochTimings;
        static std::vector<double> randomDiscretePolicy();
        static std::vector<double> randomStochasticPolicy();
        static std::vector<double> randomStatePolicy(bool stochastic);
        void initialisePolicy(bool stochastic);
        void initialiseStateValues();
        virtual double actionProbability(State *s, Maze::Actions a);
        static std::vector<double> actionAsActionProbabilityDistribution(Maze::Actions a);

    public:
        Player(Maze* m, double gamma, bool initialiseStochastic = false);
        virtual ~Player();
        virtual Maze::Actions chooseAction(State *s);
        virtual void solveMaze() = 0;
        virtual void printStateActionProbabilities(State *s);
        virtual void printFinalPolicy();
        std::vector<double> getEpochTimings();
        std::map<std::tuple<int, int, Maze::Actions>, double> getPolicy();
        static std::string playerTypeAsString(Player::Types type);
};

#endif