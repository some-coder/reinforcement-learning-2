#ifndef DATUM_HPP
#define DATUM_HPP

#include "Player.hpp"

class Datum {
    private:
        std::string mazeIdentifier;
        std::vector<Player::Types> players;
        std::map<Player::Types, std::vector<double>> timings;
        std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> policies;

    public:
        Datum(std::string mazeIdentifier, std::vector<Player::Types> players,
                std::map<Player::Types, std::vector<double>> timings,
                std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> policies);
        Datum();
        ~Datum();
        double totalTimeElapsed(Player::Types player);
        void writeDatumToFiles();
};

#endif