#include "MonteCarloPlayer.hpp"

MonteCarloPlayer::MonteCarloPlayer(Maze *m, double gamma, int T, bool initialiseStochastic) :
        LearningPlayer(m, gamma, T, initialiseStochastic) {
    this->currentEpoch = 0;
    this->timeoutEpoch = T;
}

MonteCarloPlayer::~MonteCarloPlayer() = default;