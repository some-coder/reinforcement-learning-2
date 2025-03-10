#ifndef DATUM_HPP
#define DATUM_HPP

#include <utility>
#include <fstream>
#include <iostream>
#include "Player.hpp"

class Datum {
    private:
        int id;
        int mazeWidth;
        int mazeHeight;
        std::string mazeIdentifier;
        std::vector<Player::Types> players;
        std::map<Player::Types, std::vector<double>> timings;
        std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> policies;
        std::map<Player::Types, std::vector<double>> averageRewards;

    public:
        Datum(int id, int mazeWidth, int mazeHeight, std::string mazeIdentifier, std::vector<Player::Types> players,
                std::map<Player::Types, std::vector<double>> timings,
                std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> policies,
                std::map<Player::Types, std::vector<double>> averageRewards);
        explicit Datum(int id);
        ~Datum();
        std::string singlePlayerTimings(Player::Types type);
        std::string playerTimings();
        std::string singlePlayerPolicy(Player::Types type);
        std::string playerPolicies();
        std::string singlePlayerAverageReward(Player::Types type);
        std::string playerAverageRewards();
        void writeDatumToFiles();
        std::string getMazeIdentifier();
        std::map<std::tuple<int, int, Maze::Actions>, double> getPolicy(Player::Types type);
};

#endif