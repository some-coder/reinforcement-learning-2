#include "RandomPlayer.hpp"

/**
 * Constructs an arbitrary-action player.
 *
 * @param m The maze to be solved by the player.
 */
RandomPlayer::RandomPlayer(Maze *m) : Player(m, 0.0, true) {}

/**
 * Destructs the random player.
 */
RandomPlayer::~RandomPlayer() = default;

/**
 * Trivially 'solves' the maze.
 *
 * This method is purely implemented for uniformity reasons. Moreover, it allows
 * us to store at least one epoch duration datum, so that the player at least
 * participates in timing data.
 */
void RandomPlayer::solveMaze() {
    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = std::chrono::high_resolution_clock::now();
    this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
}