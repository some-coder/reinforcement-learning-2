#include "TimeDifferencePlayer.hpp"

TimeDifferencePlayer::TimeDifferencePlayer(Maze *m, double gamma, int T, double alpha) : LearningPlayer(m, gamma, T) {
    this->alpha = alpha;
}

TimeDifferencePlayer::~TimeDifferencePlayer() = default;