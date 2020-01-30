#ifndef MONTE_CARLO_EVERY_VISIT_PLAYER
#define MONTE_CARLO_EVERY_VISIT_PLAYER

#include "MonteCarloPlayer.hpp"

class MonteCarloEveryVisitPlayer : public MonteCarloPlayer {
    private:
        static constexpr double STARTING_TEMPERATURE = 0.9;
        static constexpr double ENDING_TEMPERATURE = 0.2;
        double temperature;
        double G;
        double W;
        std::map<std::tuple<State*, Maze::Actions>, double> C;
        std::map<State*, std::vector<double>> explorationPolicy;
        void initialiseC();
        static std::vector<double> stateExplorationPolicy();
        void initialiseExplorationPolicy();
        void updateExplorationPolicy(State *s, Maze::Actions greedyAction);
        double actionProbability(State *s, Maze::Actions a) override;
        std::tuple<State*, Maze::Actions> initialStateActionPair() override;
        std::tuple<State*, Maze::Actions> nextStateActionPair(std::tuple<State*, Maze::Actions> currentPair) override;
        void generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) override;
        void performInitialisation() override;
        void performIteration() override;

    public:
        MonteCarloEveryVisitPlayer(Maze *m, double gamma, int T);
        ~MonteCarloEveryVisitPlayer() override;
        void solveMaze() override;
};

#endif