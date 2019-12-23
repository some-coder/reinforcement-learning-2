#ifndef POLICY_ITERATION_PLAYER_HPP
#define POLICY_ITERATION_PLAYER_HPP

#include "DynamicProgrammingPlayer.hpp"

class PolicyIterationPlayer : public DynamicProgrammingPlayer {
    private:
        bool policyIsStable;
        static std::vector<double> randomStatePolicy();
        std::map<State*, double> oldStateValues;
        void initialisePolicy();
        void copyStateValues(std::map<State*, double>* source, std::map<State*, double> *target);
        double stateValue(State *s, Maze::Actions a);
        double updatedStateValue(State *s, Maze::Actions a);
        Maze::Actions greedyActionForState(State *s);
        static std::vector<double> greedyActionProbabilities(Maze::Actions greedyAction);
        void performEvaluationStep();
        void performImprovementStep();

    public:
        PolicyIterationPlayer(Maze *m, double gamma, double theta);
        ~PolicyIterationPlayer() override;
        void solveMaze() override;
};

#endif