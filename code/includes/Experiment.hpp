#ifndef EXPERIMENT_HPP
#define EXPERIMENT_HPP

#include "Run.hpp"

class Experiment {
    private:
        int runNumber;
        std::vector<std::string> selectedMazes;
        std::vector<Player::Types> selectedPlayers;
        std::vector<Datum> data;
        std::string runMazeIdentifier(int runIndex);
        static void clearOldData();
        void writeData();

    public:
        Experiment(std::vector<Player::Types> selectedPlayers, int runNumber = 1e2);
        Experiment(std::vector<Player::Types> selectedPlayers, std::vector<std::string> selectedMazes, int runNumber = 1e2);
        ~Experiment();
        void conductExperiment();
};

#endif