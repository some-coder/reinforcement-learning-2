#include "MonteCarloPlayer.hpp"

MonteCarloPlayer::MonteCarloPlayer(Maze *m, double gamma, int T) : LearningPlayer(m, gamma, T) {
    this->currentEpoch = 0;
    this->timeoutEpoch = T;
}

MonteCarloPlayer::~MonteCarloPlayer() = default;