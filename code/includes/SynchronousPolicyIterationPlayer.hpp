#ifndef SYNCHRONOUS_POLICY_ITERATION_PLAYER_HPP
#define SYNCHRONOUS_POLICY_ITERATION_PLAYER_HPP

#include "PolicyIterationPlayer.hpp"

class SynchronousPolicyIterationPlayer : public PolicyIterationPlayer {
    private:
        void performEvaluationStep() override;
        void performImprovementStep() override;

    public:
        SynchronousPolicyIterationPlayer(Maze *m, double gamma, double theta);
        ~SynchronousPolicyIterationPlayer() override;
};

#endif