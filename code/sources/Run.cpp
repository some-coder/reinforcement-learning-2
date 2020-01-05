#include "Run.hpp"

Run::Run(int mazeUsed, std::map<Player::Types, bool> *playerSelection) : results(mazeUsed) {
    this->mazeUsed = mazeUsed;
    this->playerSelection = *playerSelection;
}

Run::~Run() = default;

void Run::performRun() {
    Maze maze = Maze("input/maze-" + std::to_string(this->mazeUsed) + ".in");
    if (true) {
        this->players.push_back(new SynchronousPolicyIterationPlayer(&maze, 0.9, 1e-3));
    }
    this->players.back()->solveMaze();
    this->players.back()->printFinalPolicy();
    delete this->players[0];
}