#ifndef ASYNCHRONOUS_POLICY_ITERATION_PLAYER_HPP
#define ASYNCHRONOUS_POLICY_ITERATION_PLAYER_HPP

#include "PolicyIterationPlayer.hpp"

class AsynchronousPolicyIterationPlayer : public PolicyIterationPlayer {
    private:
        State* currentState;
        int iteration;
        int maximumIteration;
        State* randomNontrivialState();
        void performEvaluationStep() override;
        void performImprovementStep() override;

    public:
        AsynchronousPolicyIterationPlayer(Maze *m, double gamma, double theta, int maximumIteration);
        ~AsynchronousPolicyIterationPlayer() override;
};

#endif