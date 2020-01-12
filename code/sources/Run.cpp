#include <utility>
#include "Run.hpp"

void Run::buildMazes(const std::string& mazeIdentifier) {
    int mazeIndex;
    for (mazeIndex = 0; mazeIndex < (int)this->playerSelection.size(); mazeIndex++) {
        this->mazes.emplace_back(mazeIdentifier);
    }
}

Run::Run(int id, std::string mazeIdentifier, std::vector<Player::Types> playerSelection) :
        results(id) {
    this->id = id;
    this->playerSelection = std::move(playerSelection);
    this->buildMazes(mazeIdentifier);
}

Run::~Run() = default;

/**
 * Create a player of the provided type and add the player to the vector 'players'.
 * 
 * @param type the type of player that has to be created
 * @return the created player
 */
void Run::allocatePlayer(int playerIndex, Player::Types type) {
    switch (type) {
        case Player::Types::SynchronousPolicyIteration:
            this->players.push_back(new SynchronousPolicyIterationPlayer(&(this->mazes[playerIndex]), 0.9, 1e-3));
            break;
        case Player::Types::AsynchronousPolicyIteration:
            this->players.push_back(new AsynchronousPolicyIterationPlayer(&(this->mazes[playerIndex]), 0.9, 1e-3,
                    1e3));
            break;
        case Player::Types::SynchronousValueIteration:
            this->players.push_back(new SynchronousValueIterationPlayer(&(this->mazes[playerIndex]), 0.9, 1e-3));
            break;
        case Player::Types::AsynchronousValueIteration:
            this->players.push_back(new AsynchronousValueIterationPlayer(&(this->mazes[playerIndex]), 0.9, 1e-3,
                    1e3));
            break;
        case Player::Types::MonteCarloExploringStarts:
            this->players.push_back(new MonteCarloExploringStartsPlayer(&(this->mazes[playerIndex]), 0.9, 1e3));
            break;
        case Player::Types::MonteCarloFirstVisit:
            this->players.push_back(new MonteCarloFirstVisitPlayer(&(this->mazes[playerIndex]), 0.9, 1e3, 0.1));
            break;
        case Player::Types::MonteCarloEveryVisit:
            this->players.push_back(new MonteCarloEveryVisitPlayer(&(this->mazes[playerIndex]), 0.9, 1e3));
            break;
        case Player::Types::TDSarsa:
            this->players.push_back(new SarsaPlayer(&(this->mazes[playerIndex]), 0.9, 1e3, 0.5, 0.1));
            break;
    }
}

/**
 * Loop through all players selected for this run, create the players using Player and allocate them to
 * the vector 'players'.
 */
void Run::allocatePlayers() {
    int selectedPlayerIndex;
    for (selectedPlayerIndex = 0; selectedPlayerIndex < (int)this->playerSelection.size(); selectedPlayerIndex++) {
        this->allocatePlayer(selectedPlayerIndex, this->playerSelection[selectedPlayerIndex]);
    }
}

/**
 * Make all players selected for this run solve the maze.
 */
void Run::runAlgorithms() {
    int playerIndex;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        this->players[playerIndex]->solveMaze();
    }
}

/**
 * Maps the player type to his timings.
 * 
 * The function loops through all player types and gets per player the timings of all the player's epochs.
 * 
 * @return the timings of all the players
 */
std::map<Player::Types, std::vector<double>> Run::prepareTimings() {
    int playerIndex;
    std::map<Player::Types, std::vector<double>> playerTimings;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        playerTimings[this->playerSelection[playerIndex]] = this->players[playerIndex]->getEpochTimings();
    }
    return playerTimings;
}

/**
 * Maps the player type to his policy.
 * 
 * The function loops through all player types and gets per player the resulting policy.
 * 
 * @return the policies of all the players
 */
std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> Run::preparePolicies() {
    int playerIndex;
    std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> playerPolicies;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        playerPolicies[this->playerSelection[playerIndex]] = this->players[playerIndex]->getPolicy();
    }
    return playerPolicies;
}

/**
 * Creates a datum of the run.
 * 
 * Creates a datum of the run. This datum contains all information about the run
 * (i.e. the run id, the maze dimensions, the maze identifier, all selected player types,
 * the timings of the player(s) as result and the policies of the player(s) as result).
 * 
 * @return A Datum with the results of this run
 */
Datum Run::datumFromRun() {
    return Datum(this->id, this->mazes[0].getMazeWidth(), this->mazes[0].getMazeHeight(),
            this->mazes[0].getMazeIdentifier(), this->playerSelection,
            this->prepareTimings(), this->preparePolicies());
}

/**
 * Loop through all players selected for this run and deallocate them from
 * the vector 'players'.
 */
void Run::deallocatePlayers() {
    int playerIndex;
    for (playerIndex = 0; playerIndex < (int)this->players.size(); playerIndex++) {
        delete this->players[playerIndex];
    }
}

/**
 * This function conducts the run for all players in the provided maze.
 * 
 * This function loops through all players and for each player conducts a run in the provided maze. It then
 * collects the results as a datum using Datum. At the end it returns the results as type Datum.
 * 
 * @return results of the run (all players) as a Datum
 */
Datum Run::conductRun() {
    this->allocatePlayers();
    this->runAlgorithms();
    this->results = this->datumFromRun();
    this->deallocatePlayers();
    return this->results;
}