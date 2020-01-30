#ifndef SARSA_PLAYER_HPP
#define SARSA_PLAYER_HPP

#include "TimeDifferencePlayer.hpp"

class SarsaPlayer : public TimeDifferencePlayer {
    private:
        void performIteration();
        void updatePolicyUsingQuality();
        void updatePolicyUsingQuality(State *s);
        std::tuple<State*, Maze::Actions> initialStateActionPair() override;
        std::tuple<State*, Maze::Actions> nextStateActionPair(std::tuple<State*, Maze::Actions> currentPair) override;
        void generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) override;
        Maze::Actions greedyAction(State *s) override;

    public:
        SarsaPlayer(Maze *m, double gamma, int T, double alpha, double epsilon);
        ~SarsaPlayer() override;

        void solveMaze() override;
};

#endif