#include "DynamicProgrammingPlayer.hpp"

DynamicProgrammingPlayer::DynamicProgrammingPlayer(Maze *m, double gamma, double theta) : Player(m, gamma) {
    this->maze = m;
    this->discountFactor = gamma;
    this->theta = theta;
    this->copyStateValues(&(this->stateValues), &(this->oldStateValues));
}

DynamicProgrammingPlayer::~DynamicProgrammingPlayer() = default;

void DynamicProgrammingPlayer::copyStateValues(std::map<State*, double> *source, std::map<State*, double> *target) {
    int i;
    double stateValue;
    State *s;
    for (i = 0; i < (int)this->maze->getStates()->size(); i++) {
        s = this->maze->getState(i);
        stateValue = source->at(s);
        (*target)[s] = stateValue;
    }
}