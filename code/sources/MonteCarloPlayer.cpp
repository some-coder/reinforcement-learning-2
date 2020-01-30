#include "MonteCarloPlayer.hpp"
#include "LearningPlayer.hpp"

/**
 * Constructs a Monte Carlo player.
 *
 * @param m The maze for the player to solve.
 * @param gamma The discount factor to apply to earlier rewards.
 * @param T The timeout epoch upon which to stop episodes.
 * @param initialiseStochastic Should the player have a probabilistic starting
 *      policy?
 */
MonteCarloPlayer::MonteCarloPlayer(Maze *m, double gamma, int T, bool initialiseStochastic) :
        LearningPlayer(m, gamma, T, initialiseStochastic) {}

/**
 * Destructs the Monte Carlo player.
 */
MonteCarloPlayer::~MonteCarloPlayer() = default;

