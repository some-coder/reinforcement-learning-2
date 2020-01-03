#ifndef MONTE_CARLO_EXPLORING_STARTS_PLAYER
#define MONTE_CARLO_EXPLORING_STARTS_PLAYER

/* Todo: Pull from reattempt! */

#include "MonteCarloPlayer.hpp"

class MonteCarloExploringStartsPlayer : public MonteCarloPlayer {
    private:


    public:
        MonteCarloExploringStartsPlayer(Maze *m, double gamma, int T);
        ~MonteCarloExploringStartsPlayer() override;
};

#endif