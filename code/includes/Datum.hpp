#ifndef DATUM_HPP
#define DATUM_HPP

#include "Player.hpp"

class Datum {
    private:
        int mazeUsed;
        std::vector<Player::Types> players;
        std::map<Player::Types, std::vector<double>> timings;
        std::map<std::tuple<Player::Types, int, int, Maze::Actions>, double> policies;
        std::map<Player::Types, std::vector<double>> testRunReturns;
        double totalTimeElapsed(Player::Types player);

    public:
        explicit Datum(int mazeUsed);
        ~Datum();
        void writeDatumToFiles();
};

#endif