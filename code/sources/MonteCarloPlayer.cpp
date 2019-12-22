#include "MonteCarloPlayer.hpp"

MonteCarloPlayer::MonteCarloPlayer(Maze *m, double gamma, int T) : LearningPlayer(m, gamma, T) {}

MonteCarloPlayer::~MonteCarloPlayer() = default;