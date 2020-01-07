#include <RandomServices.hpp>
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
    this->writeData();
}

