#ifndef LEARNING_PLAYER_HPP
#define LEARNING_PLAYER_HPP

#include "Player.hpp"

class LearningPlayer : public Player {
    protected:
        static constexpr double INITIAL_STATE_ACTION_VALUE = 0.0;
        static constexpr double EPISODE_TIMEOUT_FRACTION = 1.2;
        int currentEpoch;
        int timeoutEpoch;
        std::vector<double> rewards;
        std::map<std::tuple<State*, Maze::Actions>, double> stateActionValues;
        void initialiseStateActionValues();
        virtual std::tuple<State*, Maze::Actions> initialStateActionPair() = 0;
        virtual std::tuple<State*, Maze::Actions> nextStateActionPair(std::tuple<State*,
                Maze::Actions> currentPair) = 0;
        virtual void generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) = 0;
        bool maximumIterationReached();

    public:
        LearningPlayer(Maze *m, double gamma, int T, bool initialiseStochastic = false);
        ~LearningPlayer() override;
        void setStateActionValue(State *s, Maze::Actions action, double value);
        void printStateActionValues(State *s);
        void printFinalPolicy() override;
};

#endif