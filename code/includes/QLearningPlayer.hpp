#ifndef Q_LEARNING_PLAYER_HPP
#define Q_LEARNING_PLAYER_HPP

#include "TimeDifferencePlayer.hpp"

class QLearningPlayer : public TimeDifferencePlayer {
    private:
        void performIteration();
        void generateEpisode(std::tuple<State *, Maze::Actions> startStateActionPair) override;
        void updateStateActionValue(std::tuple<State*, Maze::Actions> stateActionPair, std::tuple<State*,
                Maze::Actions> nextStateActionPair);
        std::tuple<State*, Maze::Actions> initialStateActionPair() override;
        std::tuple<State*, Maze::Actions> nextStateActionPair(std::tuple<State*, Maze::Actions> currentPair) override;
        void updatePolicyUsingStateActionValues();
        void updatePolicyUsingStateActionValues(State *state);


    public:
        QLearningPlayer(Maze *m, double gamma, int T, double alpha, double epsilon);
        ~QLearningPlayer();
        void solveMaze() override;
};

#endif