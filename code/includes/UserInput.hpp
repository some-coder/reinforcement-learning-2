#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <iostream>
#include "Maze.hpp"
#include "Player.hpp"
#include "Experiment.hpp"

class UserInput {
    public:
        static constexpr int REPETITION_FACTOR = 1e1;

    private:
        static bool useRandomPreBuiltMazes();
        static int randomPreBuiltMazesAmount();
        static std::vector<std::string> randomMazeSelection();
        static std::vector<std::string> selectedMazes();
        static bool useAllPlayers();
        static Player::Types playerTypeFromInteger(int i);
        static std::vector<Player::Types> selectedPlayers();

    public:
        static Experiment experimentFromUserInput();
};

#endif