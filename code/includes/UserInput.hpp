#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <iostream>
#include "Maze.hpp"
#include "Player.hpp"
#include "Experiment.hpp"

class UserInput {
    private:
        static bool useRandomPreBuiltMazes();
        static std::vector<Maze> selectedMazes();
        static bool useAllPlayers();
        static std::vector<Player::Types> selectedPlayers();

    public:
        static Experiment experimentFromUserInput();
};

#endif