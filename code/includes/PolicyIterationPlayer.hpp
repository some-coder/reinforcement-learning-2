#ifndef POLICY_ITERATION_PLAYER_HPP
#define POLICY_ITERATION_PLAYER_HPP

#include "DynamicProgrammingPlayer.hpp"

class PolicyIterationPlayer : public DynamicProgrammingPlayer {
    private:
        bool policyIsStable;
        static std::vector<double> randomStatePolicy();
        void initialisePolicy();
        double stateEvaluation(State *s, Maze::Actions a);
        Maze::Actions greedyActionForState(State *s);
        void performEvaluationStep();
        void performImprovementStep();
        void solveMaze() override;

    public:
        PolicyIterationPlayer(Maze *m, double gamma, double theta);
        ~PolicyIterationPlayer() override;
};

#endif