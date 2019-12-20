#ifndef RUN_HPP
#define RUN_HPP

#include <vector>
#include "Maze.hpp"
#include "Player.hpp"

class Run {
    public:
        enum PlayerTypes {random};

    private:
        std::vector<PlayerTypes> participantTypes;
        std::vector<Player*> participants;
        Maze maze;
        void

    public:
        Run(std::vector<PlayerTypes> types);
};

#endif