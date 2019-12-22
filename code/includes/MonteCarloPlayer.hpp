#ifndef MONTE_CARLO_PLAYER_HPP
#define MONTE_CARLO_PLAYER_HPP

#include "LearningPlayer.hpp"

class MonteCarloPlayer : public LearningPlayer {
    private:
        std::vector<std::tuple<State*, Maze::Actions>> episode;

    public:
        MonteCarloPlayer(Maze *m, double gamma, int T);
        ~MonteCarloPlayer() override;
};

#endif