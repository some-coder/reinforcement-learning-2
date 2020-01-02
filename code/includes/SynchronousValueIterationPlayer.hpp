#ifndef SYNCRHONOUS_VALUE_ITERATION_HPP
#define SYNCHRONOUS_VALUE_ITERATION_HPP

#include "ValueIterationPlayer.hpp"

class SynchronousValueIterationPlayer : public ValueIterationPlayer {
    private:
        void performRepetitionStep() override;
        void performConstructionStep() override;

    public:
        SynchronousValueIterationPlayer(Maze *m, double gamma, double theta);
        ~SynchronousValueIterationPlayer() override;
};

#endif