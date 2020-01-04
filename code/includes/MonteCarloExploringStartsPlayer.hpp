#ifndef MONTE_CARLO_EXPLORING_STARTS_PLAYER
#define MONTE_CARLO_EXPLORING_STARTS_PLAYER

#include "MonteCarloPlayer.hpp"

class MonteCarloExploringStartsPlayer : public MonteCarloPlayer {
    private:
        std::map<std::tuple<State*, Maze::Actions>, std::vector<double>> returns;
        State *randomState();
        static Maze::Actions randomAction();
        std::tuple<State*, Maze::Actions> randomStateActionPair();
        std::tuple<State*, Maze::Actions> initialStateActionPair() override;
        std::tuple<State*, Maze::Actions> nextStateActionPair(std::tuple<State*, Maze::Actions> currentPair) override;
        void generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) override;
        double episodeReturnComponent(int k, int rewardIndex);
        double episodeReturn(int onsetIndex);
        double returnsAverage(std::tuple<State*, Maze::Actions> stateActionPair);
        void performInitialisation() override;
        void performIteration() override;

    public:
        MonteCarloExploringStartsPlayer(Maze *m, double gamma, int T);
        ~MonteCarloExploringStartsPlayer() override;
        void solveMaze() override;
};

#endif