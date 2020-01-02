#ifndef VALUE_ITERATION_PLAYER_HPP
#define VALUE_ITERATION_PLAYER_HPP

#include "DynamicProgrammingPlayer.hpp"

class ValueIterationPlayer : public DynamicProgrammingPlayer {
    protected:
        double stateActionValue(State *s, Maze::Actions a);
        double updatedStateValue(State *s);
        Maze::Actions greedyActionForState(State *s);
        virtual void performRepetitionStep() = 0;
        virtual void performConstructionStep() = 0;

    public:
        ValueIterationPlayer(Maze *m, double gamma, double theta);
        ~ValueIterationPlayer() override;
        void solveMaze() override;
};

#endif