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
        std::map<Player::Types, bool> playerSelection;
        std::vector<Player*> players;
        Datum results;

    public:
        Run(int mazeUsed, std::map<Player::Types, bool> *playerSelection);
        ~Run();
        void performRun();
        Datum getResults();
};

#endif