#ifndef MONTE_CARLO_PLAYER_HPP
#define MONTE_CARLO_PLAYER_HPP

#include "LearningPlayer.hpp"

class MonteCarloPlayer : public LearningPlayer {
    protected:
        std::vector<std::tuple<State*, Maze::Actions>> episode;
        virtual void performInitialisation() = 0;
        virtual void performIteration() = 0;

    public:
        MonteCarloPlayer(Maze *m, double gamma, int T);
        ~MonteCarloPlayer() override;
};

#endif