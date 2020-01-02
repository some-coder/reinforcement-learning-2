#ifndef ASYNCHRONOUS_VALUE_ITERATION_PLAYER_HPP
#define ASYNCHRONOUS_VALUE_ITERATION_PLAYER_HPP

#include "ValueIterationPlayer.hpp"

class AsynchronousValueIterationPlayer : public ValueIterationPlayer {
    private:
        int iteration;
        int maximumIteration;
        void performRepetitionStep() override;
        void performConstructionStep() override;

    public:
        AsynchronousValueIterationPlayer(Maze *m, double gamma, double theta, int maximumIteration);
        ~AsynchronousValueIterationPlayer() override;
};

#endif