#ifndef RANDOM_PLAYER_HPP
#define RANDOM_PLAYER_HPP

#include "Player.hpp"

class RandomPlayer : public Player {
    public:
        RandomPlayer(Maze *m);
        ~RandomPlayer();
        void solveMaze() override;
};

#endif