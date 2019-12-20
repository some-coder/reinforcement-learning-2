#ifndef RANDOM_PLAYER_HPP
#define RANDOM_PLAYER_HPP

#include "Player.hpp"

class RandomPlayer : public Player {
    private:
        void solveMaze();

    public:
        RandomPlayer(Maze *m);
};

#endif