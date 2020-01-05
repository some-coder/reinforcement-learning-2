#include <utility>
#include "Run.hpp"

Run::Run(int mazeUsed, std::vector<Player::Types> playerSelection) : results(mazeUsed) {
    this->mazeUsed = mazeUsed;
    this->playerSelection = std::move(playerSelection);
}

Run::~Run() = default;

void Run::allocatePlayer(Player::Types type, Maze *maze) {
    switch (type) {
        case Player::Types::SynchronousPolicyIteration:
            this->players.push_back(new SynchronousPolicyIterationPlayer(maze, 0.9, 1e-3));
            break;
        case Player::Types::AsynchronousPolicyIteration:
            this->players.push_back(new AsynchronousPolicyIterationPlayer(maze, 0.9, 1e-3,
                    1e3));
            break;
        case Player::Types::SynchronousValueIteration:
            this->players.push_back(new SynchronousValueIterationPlayer(maze, 0.9, 1e-3));
            break;
        case Player::Types::AsynchronousValueIteration:
            this->players.push_back(new AsynchronousValueIterationPlayer(maze, 0.9, 1e-3,
                    1e3));
            break;
        case Player::Types::MonteCarloExploringStarts:
            this->players.push_back(new MonteCarloExploringStartsPlayer(maze, 0.9, 1e3));
            break;
        case Player::Types::MonteCarloFirstVisit:
            this->players.push_back(new MonteCarloFirstVisitPlayer(maze, 0.9, 1e3, 0.1));
            break;
        case Player::Types::MonteCarloEveryVisit:
            this->players.push_back(new MonteCarloEveryVisitPlayer(maze, 0.9, 1e3));
            break;
    }
}

void Run::allocatePlayers(Maze *maze) {
    int selectedPlayerIndex;
    for (selectedPlayerIndex = 0; selectedPlayerIndex < (int)this->playerSelection.size(); selectedPlayerIndex++) {
        this->allocatePlayer(this->playerSelection[selectedPlayerIndex], maze);
    }
}

void Run::deallocatePlayers() {
    int playerIndex;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        delete this->players[playerIndex];
    }
}

void Run::performRun() {
    int playerIndex;
    Maze maze = Maze("input/maze-" + std::to_string(this->mazeUsed) + ".in");
    this->allocatePlayers(&maze);
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        printf("ALGORITHM %d\n", playerIndex);
        this->players[playerIndex]->solveMaze();
        this->players[playerIndex]->printFinalPolicy();
        printf("\n");
    }
    this->deallocatePlayers();
}