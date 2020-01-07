#ifndef POLICY_ITERATION_PLAYER_HPP
#define POLICY_ITERATION_PLAYER_HPP

#include "DynamicProgrammingPlayer.hpp"

class PolicyIterationPlayer : public DynamicProgrammingPlayer {
    protected:
        bool policyIsStable;
        double stateValue(State *s, Maze::Actions a);
        double updatedStateValue(State *s, Maze::Actions a);
        Maze::Actions greedyActionForState(State *s);
        virtual void performEvaluationStep() = 0;
        virtual void performImprovementStep() = 0;

    public:
        PolicyIterationPlayer(Maze *m, double gamma, double theta);
        ~PolicyIterationPlayer() override;
        void solveMaze() override;
};

#endif