#include "DynamicProgrammingPlayer.hpp"

DynamicProgrammingPlayer::DynamicProgrammingPlayer(Maze *m, double gamma, double theta) : Player(m, gamma) {
    this->maze = m;
    this->discountFactor = gamma;
    this->theta = theta;
}

DynamicProgrammingPlayer::~DynamicProgrammingPlayer() = default;