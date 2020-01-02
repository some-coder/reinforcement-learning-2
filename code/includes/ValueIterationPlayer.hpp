#ifndef VALUE_ITERATION_PLAYER_HPP
#define VALUE_ITERATION_PLAYER_HPP

#include "DynamicProgrammingPlayer.hpp"

class ValueIterationPlayer : public DynamicProgrammingPlayer {
    private:
        double stateActionValue(State *s, Maze::Actions a);
        double updatedStateValue(State *s);
        Maze::Actions greedyActionForState(State *s);
        void performRepetitionStep();
        void performConstructionStep();

    public:
        ValueIterationPlayer(Maze *m, double gamma, double theta);
        ~ValueIterationPlayer() override;
        void solveMaze() override;
};

#endif