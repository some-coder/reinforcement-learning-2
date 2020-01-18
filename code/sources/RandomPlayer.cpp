#include "RandomPlayer.hpp"

RandomPlayer::RandomPlayer(Maze *m) : Player(m, 0.0, true) {}

RandomPlayer::~RandomPlayer() = default;

void RandomPlayer::solveMaze() {
    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = std::chrono::high_resolution_clock::now();
    this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
}