#ifndef DYNAMIC_PROGRAMMING_PLAYER_HPP
#define DYNAMIC_PROGRAMMING_PLAYER_HPP

#include "Player.hpp"

class DynamicProgrammingPlayer : public Player {
    protected:
        double theta;
        std::map<State*, double> oldStateValues;
        void copyStateValues(std::map<State*, double> *source, std::map<State*, double> *target);

    public:
        DynamicProgrammingPlayer(Maze *m, double gamma, double theta);
        ~DynamicProgrammingPlayer() override;
};

#endif