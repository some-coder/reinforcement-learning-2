#ifndef MONTE_CARLO_FIRST_VISIT_PLAYER_HPP
#define MONTE_CARLO_FIRST_VISIT_PLAYER_HPP

#include "MonteCarloPlayer.hpp"

class MonteCarloFirstVisitPlayer : public MonteCarloPlayer {
    private:
        double epsilon;
        std::map<std::tuple<State*, Maze::Actions>, std::vector<double>> returns;
        std::tuple<State*, Maze::Actions> initialStateActionPair() override;
        std::tuple<State*, Maze::Actions> nextStateActionPair(std::tuple<State*, Maze::Actions> currentPair) override;
        void generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) override;
        double episodeReturnComponent(int k, int rewardIndex);
        double episodeReturn(int onsetIndex);
        double returnsAverage(std::tuple<State*, Maze::Actions> stateActionPair);
        Maze::Actions greedyAction(State *s);
        void updateStatePolicy(State *s, Maze::Actions greedyAction);
        void performInitialisation() override;
        void performIteration() override;

    public:
        MonteCarloFirstVisitPlayer(Maze *m, double gamma, int T, double epsilon);
        ~MonteCarloFirstVisitPlayer() override;
        void solveMaze() override;
};

#endif