#include <utility>
#include "Run.hpp"

Run::Run(int mazeUsed, std::vector<Player::Types> playerSelection) :
        maze("input/maze-" + std::to_string(mazeUsed) + ".in"), results(mazeUsed) {
    this->playerSelection = std::move(playerSelection);
}

Run::~Run() = default;

void Run::allocatePlayer(Player::Types type) {
    switch (type) {
        case Player::Types::SynchronousPolicyIteration:
            this->players.push_back(new SynchronousPolicyIterationPlayer(&(this->maze), 0.9, 1e-3));
            break;
        case Player::Types::AsynchronousPolicyIteration:
            this->players.push_back(new AsynchronousPolicyIterationPlayer(&(this->maze), 0.9, 1e-3,
                    1e3));
            break;
        case Player::Types::SynchronousValueIteration:
            this->players.push_back(new SynchronousValueIterationPlayer(&(this->maze), 0.9, 1e-3));
            break;
        case Player::Types::AsynchronousValueIteration:
            this->players.push_back(new AsynchronousValueIterationPlayer(&(this->maze), 0.9, 1e-3,
                    1e3));
            break;
        case Player::Types::MonteCarloExploringStarts:
            this->players.push_back(new MonteCarloExploringStartsPlayer(&(this->maze), 0.9, 1e3));
            break;
        case Player::Types::MonteCarloFirstVisit:
            this->players.push_back(new MonteCarloFirstVisitPlayer(&(this->maze), 0.9, 1e3, 0.1));
            break;
        case Player::Types::MonteCarloEveryVisit:
            this->players.push_back(new MonteCarloEveryVisitPlayer(&(this->maze), 0.9, 1e3));
            break;
    }
}

void Run::allocatePlayers() {
    int selectedPlayerIndex;
    for (selectedPlayerIndex = 0; selectedPlayerIndex < (int)this->playerSelection.size(); selectedPlayerIndex++) {
        this->allocatePlayer(this->playerSelection[selectedPlayerIndex]);
    }
}

void Run::runAlgorithms() {
    int playerIndex;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        this->players[playerIndex]->solveMaze();
        printf("%d / %d.\n", (playerIndex + 1), (int)this->players.size());
    }
}

void Run::prepareDatum() {
    printf("Datum preparation to be worked upon.\n");
}

void Run::deallocatePlayers() {
    int playerIndex;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        delete this->players[playerIndex];
    }
}

void Run::performRun() {
    this->allocatePlayers();
    this->runAlgorithms();
    this->prepareDatum();
    this->deallocatePlayers();
}