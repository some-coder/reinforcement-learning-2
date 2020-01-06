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

void Experiment::clearOldData() {
    std::ofstream output;
    output.open("output/data/timings.csv", std::ofstream::out | std::ofstream::trunc);
    output.close();
}

void Experiment::writeData() {
    int datumIndex;
    for (datumIndex = 0; datumIndex < (int)this->data.size(); datumIndex++) {
        this->data[datumIndex].writeDatumToFiles();
    }
}

void Experiment::conductExperiment() {
    int runIndex;
    this->clearOldData();
    for (runIndex = 0; runIndex < this->runNumber; runIndex++) {
        Run run = Run(runIndex, this->runMazeIdentifier(runIndex), this->selectedPlayers);
        this->data.push_back(run.conductRun());
    }
    this->writeData();
}

