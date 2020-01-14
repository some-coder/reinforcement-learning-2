#include "MonteCarloPlayer.hpp"
#include "LearningPlayer.hpp"

MonteCarloPlayer::MonteCarloPlayer(Maze *m, double gamma, int T, bool initialiseStochastic) :
        LearningPlayer(m, gamma, T, initialiseStochastic) {
    //TODO: this is already done in learning player though, right?
    this->currentEpoch = 0;
    this->timeoutEpoch = T;
}

MonteCarloPlayer::~MonteCarloPlayer() = default;

