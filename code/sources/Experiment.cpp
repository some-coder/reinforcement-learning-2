#include <RandomServices.hpp>
#include <ExploitPlayer.hpp>
#include "Experiment.hpp"

Experiment::Experiment(std::vector<Player::Types> selectedPlayers, int runNumber) {
    this->selectedPlayers = std::move(selectedPlayers);
    this->runNumber = runNumber;
}

Experiment::Experiment(std::vector<Player::Types> selectedPlayers, std::vector<std::string> selectedMazes) {
    this->selectedPlayers = std::move(selectedPlayers);
    this->selectedMazes   = std::move(selectedMazes);
    this->runNumber = (int)this->selectedMazes.size();
}

Experiment::~Experiment() = default;

std::string Experiment::runMazeIdentifier(int runIndex) {
    int mazeIndex;
    if (this->selectedMazes.empty()) {
        mazeIndex = RandomServices::discreteUniformSample(Maze::MAZES_AVAILABLE - 1) + 1;
        return "input/maze-" + std::to_string(mazeIndex) + ".in";
    } else {
        return this->selectedMazes[runIndex];
    }
}

/**
 * Clears the files "output/data/timings.csv" and "output/data/policies.csv".
 * 
 * There might still be data in "output/data/timings.csv" and "output/data/policies.csv" 
 * from a previous experiment. This function clears these files.
 */
void Experiment::clearOldData() {
    std::ofstream output;
    output.open("output/data/timings.csv", std::ofstream::trunc);
    output.close();
    output.open("output/data/policies.csv", std::ofstream::trunc);
    output.close();
}

std::map<std::tuple<int, int, Maze::Actions>, double> Experiment::averagePolicy(const std::string& mazeIdentifier,
        Player::Types type) {
    int datumIndex, count;
    std::map<std::tuple<int, int, Maze::Actions>, double> policy;
    std::map<std::tuple<int, int, Maze::Actions>, double> average;
    count = 0;
    for (datumIndex = 0; datumIndex < (int)this->data.size(); datumIndex++) {
        policy = this->data[datumIndex].getPolicy(type);
        if (this->data[datumIndex].getMazeIdentifier() == mazeIdentifier) {
            count++;
            if (average.empty()) {
                /* Simply copy the policy. */
                average = policy;
            } else {
                /* Add to the probability sum for the state action pairs. */
                for (auto const& a : average) {
                    average[a.first] += policy[a.first];
                }
            }
        }
    }
    for (auto const& a : average) {
        /* Correct for the number of times the map was present in the experiment. */
        average[a.first] /= (double)count;
    }
    return average;
}

bool Experiment::mazeIdentifierAlreadyPresent(std::vector<std::string> *mazeIdentifiers, std::string mazeIdentifier) {
    int identifierIndex;
    for (identifierIndex = 0; identifierIndex < (int)mazeIdentifiers->size(); identifierIndex++) {
        if ((*mazeIdentifiers)[identifierIndex] == mazeIdentifier) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> Experiment::getMazeIdentifiers() {
    int datumIndex;
    std::string current;
    std::vector<std::string> mazeIdentifiers;
    for (datumIndex = 0; datumIndex < (int)this->data.size(); datumIndex++) {
        current = this->data[datumIndex].getMazeIdentifier();
        if (!this->mazeIdentifierAlreadyPresent(&mazeIdentifiers, current)) {
            mazeIdentifiers.push_back(current);
        }
    }
    return mazeIdentifiers;
}

void Experiment::getAveragePolicies() {
    int mazeIdentifierIndex, playerTypeIndex;
    std::string mazeIdentifier;
    Player::Types playerType;
    for (mazeIdentifierIndex = 0; mazeIdentifierIndex < (int)this->mazeIdentifiers.size(); mazeIdentifierIndex++) {
        mazeIdentifier = this->mazeIdentifiers[mazeIdentifierIndex];
        for (playerTypeIndex = 0; playerTypeIndex < (int)this->selectedPlayers.size(); playerTypeIndex++) {
            playerType = this->selectedPlayers[playerTypeIndex];
            this->averagePolicies[std::make_tuple(mazeIdentifier, playerType)] = this->averagePolicy(mazeIdentifier,
                    playerType);
        }
    }
}

std::map<State*, std::vector<double>> Experiment::convertedPolicy(Maze *m, Player::Types type) {
    int stateIndex, actionIndex;
    State *s;
    std::map<State*, std::vector<double>> converted;
    std::tuple<std::string, Player::Types> outerKey;
    std::tuple<int, int, Maze::Actions> innerKey;
    for (stateIndex = 0; stateIndex < (int)m->getStates()->size(); stateIndex++) {
        s = m->getState(stateIndex);
        std::vector<double> actionProbabilities;
        for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
            outerKey = std::make_tuple(m->getMazeIdentifier(), type);
            innerKey = std::make_tuple(s->getX(), s->getY(), (Maze::Actions)actionIndex);
            actionProbabilities.push_back(this->averagePolicies[outerKey][innerKey]);
        }
        converted[s] = actionProbabilities;
    }
    return converted;
}

void Experiment::evaluateAveragePolicy(int mazeIdentifierIndex, Player::Types type) {
    std::string mazeIdentifier = this->mazeIdentifiers[mazeIdentifierIndex];
    Maze maze = Maze(mazeIdentifier);
    ExploitPlayer p = ExploitPlayer(&maze, this->convertedPolicy(&maze, type));
    p.solveMaze();

}

/**
 * Writes data of the experiment to "output/data/".
 * 
 * Using the Datum class it writes the data in data[] to the output directory. The timings
 * (how long it takes for a player to complete an epoch) are written to "output/data/timings.csv".
 * The policies of the player for every epoch are written to "output/data/policies.csv".
 */
void Experiment::writeData() {
    int datumIndex;
    for (datumIndex = 0; datumIndex < (int)this->data.size(); datumIndex++) {
        this->data[datumIndex].writeDatumToFiles();
    }
}

/**
 * Runs the experiment with provided player types and mazes, writes timing results to "output/data/timings.csv".
 * 
 * This function runs the experiment using the Run class. It uses the playerSelection to details what player algorithm(s)
 * should be run in this experiment (playerSelection provided when Experiment object was created). It uses mazeSelection to detail what maze(s)
 * the player should navigate (mazeSelection provided when Experiment object was created). Before we run the experiment we clear the files in "output/data/".
 * The time it takes for an algorithm to complete an epoch is written to "output/data/timings.csv". The policies of the player are written to "output/data/policies.csv"
 */
void Experiment::conductExperiment() {
    int runIndex;
    this->clearOldData();
    for (runIndex = 0; runIndex < this->runNumber; runIndex++) {
        Run run = Run(runIndex, this->runMazeIdentifier(runIndex), this->selectedPlayers);
        this->data.push_back(run.conductRun());
    }
    this->mazeIdentifiers = this->getMazeIdentifiers();
    this->getAveragePolicies();
    this->writeData();
}

