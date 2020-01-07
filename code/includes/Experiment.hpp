#ifndef EXPERIMENT_HPP
#define EXPERIMENT_HPP

#include "Run.hpp"

class Experiment {
    private:
        int runNumber;
        std::vector<std::string> selectedMazes;
        std::vector<Player::Types> selectedPlayers;
        std::map<std::tuple<std::string, Player::Types>, std::map<std::tuple<int, int, Maze::Actions>, double>>
            averagePolicies;
        std::vector<std::string> mazeIdentifiers;
        std::vector<Datum> data;
        std::map<std::tuple<std::string, Player::Types>, std::vector<double>> averagePolicyRewards;
        std::string runMazeIdentifier(int runIndex);
        static void clearOldData();
        bool mazeIdentifierAlreadyPresent(std::vector<std::string> *mazeIdentifiers, std::string mazeIdentifier);
        std::vector<std::string> getMazeIdentifiers();
        std::map<std::tuple<int, int, Maze::Actions>, double> averagePolicy(const std::string& mazeIdentifier,
                Player::Types type);
        void getAveragePolicies();
        std::map<State*, std::vector<double>> convertedPolicy(Maze *m, Player::Types type);
        void evaluateAveragePolicy(int mazeIdentifierIndex, Player::Types type);
        void evaluateAveragePolicies();
        void writeData();

    public:
        Experiment(std::vector<Player::Types> selectedPlayers, int runNumber = 1e2);
        Experiment(std::vector<Player::Types> selectedPlayers, std::vector<std::string> selectedMazes);
        ~Experiment();
        void conductExperiment();
};

#endif