#ifndef SARSA_PLAYER_HPP
#define SARSA_PLAYER_HPP

#include "TimeDifferencePlayer.hpp"

class SarsaPlayer : public TimeDifferencePlayer {
    private:
        double epsilon;

        void performIteration();
        void performEpisode();
        void updatePolicyUsingQuality();
        void updatePolicyUsingQuality(State *s);
        std::tuple<State*, Maze::Actions> initialStateActionPair();

    public:
        SarsaPlayer(Maze *m, double gamma, int T, double alpha, double epsilon);
        ~SarsaPlayer() override;

        void solveMaze() override;   
};

#endif