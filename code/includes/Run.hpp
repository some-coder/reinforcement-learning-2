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
#include "SarsaPlayer.hpp"
#include "Datum.hpp"

/* Todo: Make maze state separate for all algorithms. */

class Run {
    private:
        int id;
        std::vector<Maze> mazes;
        std::vector<Player::Types> playerSelection;
        std::vector<Player*> players;
        Datum results;
        void buildMazes(const std::string& mazeIdentifier);
        std::map<Player::Types, std::vector<double>> prepareTimings();
        std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> preparePolicies();
        void allocatePlayer(int playerIndex, Player::Types type);
        void allocatePlayers();
        void runAlgorithms();
        Datum datumFromRun();
        void deallocatePlayers();

    public:
        Run(int id, std::string mazeIdentifier, std::vector<Player::Types> playerSelection);
        ~Run();
        Datum conductRun();
};

#endif