#include <utility>
#include "Run.hpp"

Run::Run(int id, std::string mazeIdentifier, std::vector<Player::Types> playerSelection) :
        maze(std::move(mazeIdentifier)), results(id) {
    this->id = id;
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
    }
}

std::map<Player::Types, std::vector<double>> Run::prepareTimings() {
    int playerIndex;
    std::map<Player::Types, std::vector<double>> playerTimings;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        playerTimings[this->playerSelection[playerIndex]] = this->players[playerIndex]->getEpochTimings();
    }
    return playerTimings;
}

std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> Run::preparePolicies() {
    int playerIndex;
    std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> playerPolicies;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        playerPolicies[this->playerSelection[playerIndex]] = this->players[playerIndex]->getPolicy();
    }
    return playerPolicies;
}

Datum Run::datumFromRun() {
    return Datum(this->id, this->maze.getMazeWidth(), this->maze.getMazeHeight(),
            this->maze.getMazeIdentifier(), this->playerSelection,
            this->prepareTimings(), this->preparePolicies());
}

void Run::deallocatePlayers() {
    int playerIndex;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        delete this->players[playerIndex];
    }
}

Datum Run::conductRun() {
    this->allocatePlayers();
    this->runAlgorithms();
    this->results = this->datumFromRun();
    this->deallocatePlayers();
    return this->results;
}