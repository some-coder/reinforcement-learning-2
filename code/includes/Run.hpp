#ifndef RUN_HPP
#define RUN_HPP

#include "Player.hpp"
#include "SynchronousPolicyIterationPlayer.hpp"
#include "AsynchronousPolicyIterationPlayer.hpp"
#include "SynchronousValueIterationPlayer.hpp"
#include "AsynchronousValueIterationPlayer.hpp"
#include "MonteCarloExploringStartsPlayer.hpp"
#include "MonteCarloFirstVisitPlayer.hpp"
#include "MonteCarloEveryVisitPlayer.hpp"
#include "Datum.hpp"

/* Todo: Make maze state separate for all algorithms. */

class Run {
    private:
        int id;
        Maze maze;
        std::vector<Player::Types> playerSelection;
        std::vector<Player*> players;
        Datum results;
        std::map<Player::Types, std::vector<double>> prepareTimings();
        std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> preparePolicies();

    public:
        Run(int id, std::string mazeIdentifier, std::vector<Player::Types> playerSelection);
        ~Run();
        void allocatePlayer(Player::Types type);
        void allocatePlayers();
        void runAlgorithms();
        void prepareDatum();
        void deallocatePlayers();
        void performRun();
        Datum *getResults();
};

#endif