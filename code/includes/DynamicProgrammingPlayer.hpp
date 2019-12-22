#ifndef DYNAMIC_PROGRAMMING_PLAYER_HPP
#define DYNAMIC_PROGRAMMING_PLAYER_HPP

#include "Player.hpp"

class DynamicProgrammingPlayer : public Player {
    protected:
        double theta;

    public:
        DynamicProgrammingPlayer(Maze *m, double gamma, double theta);
        ~DynamicProgrammingPlayer() override;
};

#endif