#include <utility>
#include "Datum.hpp"

Datum::Datum(std::string mazeIdentifier, std::vector<Player::Types> players,
        std::map<Player::Types, std::vector<double>> timings,
        std::map<Player::Types, std::map<std::tuple<int, int, Maze::Actions>, double>> policies) {
    this->mazeIdentifier = std::move(mazeIdentifier);
    this->players  = std::move(players);
    this->timings  = std::move(timings);
    this->policies = std::move(policies);
}

Datum::Datum() = default;

Datum::~Datum() = default;

double Datum::totalTimeElapsed(Player::Types player) {
    int timing;
    double total;
    std::vector<double> *playerTimings;
    playerTimings = &(this->timings[player]);
    total = 0.0;
    for (timing = 0; timing < (int)playerTimings->size(); timing++) {
        total += playerTimings->at(timing);
    }
    return total;
}

void Datum::writeDatumToFiles() {

}