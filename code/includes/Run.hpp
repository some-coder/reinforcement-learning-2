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

class Run {
    private:
        int mazeUsed;
        std::vector<Player::Types> playerSelection;
        std::vector<Player*> players;
        Datum results;

    public:
        Run(int mazeUsed, std::vector<Player::Types> playerSelection);
        ~Run();
        void allocatePlayer(Player::Types type, Maze *maze);
        void allocatePlayers(Maze *maze);
        void deallocatePlayers();
        void performRun();
        Datum getResults();
};

#endif