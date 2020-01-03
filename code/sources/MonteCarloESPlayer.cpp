#include "MonteCarloESPlayer.hpp"

MonteCarloExploringStartsPlayer::MonteCarloExploringStartsPlayer(Maze *m, double gamma, int T) :
        MonteCarloPlayer(m, gamma, T) {}

MonteCarloExploringStartsPlayer::~MonteCarloExploringStartsPlayer() = default;