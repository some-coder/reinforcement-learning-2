#include <RandomServices.hpp>
#include "Datum.hpp"

Datum::Datum(int mazeUsed) {
    this->mazeUsed = mazeUsed;
}

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