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
        enum Types {RandomPlayer, SynchronousPolicyIteration, AsynchronousPolicyIteration, SynchronousValueIteration,
                AsynchronousValueIteration, MonteCarloExploringStarts, MonteCarloFirstVisit, MonteCarloEveryVisit,
                TDSarsa, TDQLearning};
        static constexpr int PLAYER_TYPE_AMOUNT = 10;

    protected:
        static constexpr double INITIAL_STATE_VALUE = 0.0;
        static constexpr int PRESENCE_SCORE_MAXIMUM = 10;
        static constexpr double EPISODE_TIMEOUT_FRACTION = 1.2;
        int currentEpoch;
        int timeoutEpoch;
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
        bool maximumIterationReached();

    public:
        Maze* maze;
        Player(Maze* m, double gamma = 0.0, bool initialiseStochastic = false);
        virtual ~Player();
        virtual Maze::Actions chooseAction(State *s);
        virtual void solveMaze() = 0;
        std::vector<double> getEpochTimings();
        std::map<std::tuple<int, int, Maze::Actions>, double> getPolicy();
        virtual std::vector<double> getTotalRewardPerEpisode();
        static std::string playerTypeAsStringShortened(Player::Types type);
};

#endif