#include <cmath>
#include "SynchronousValueIterationPlayer.hpp"

SynchronousValueIterationPlayer::SynchronousValueIterationPlayer(Maze *m, double gamma, double theta) :
        ValueIterationPlayer(m, gamma, theta) {}

SynchronousValueIterationPlayer::~SynchronousValueIterationPlayer() = default;

void SynchronousValueIterationPlayer::performRepetitionStep() {
    double delta;
    int stateIndex;
    State *s;
    do {
        delta = 0.0;
        for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
            s = this->maze->getState(stateIndex);
            if (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s)) {
                continue;
            }
            this->stateValues[s] = this->updatedStateValue(s);
            delta = std::max(delta, std::fabs(this->oldStateValues[s] - this->stateValues[s]));
        }
        this->copyStateValues(&(this->stateValues), &(this->oldStateValues));
    } while (delta >= this->theta);
}

void SynchronousValueIterationPlayer::performConstructionStep() {
    int stateIndex;
    State *s;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        s = this->maze->getState(stateIndex);
        this->policy[s] = Player::actionAsActionProbabilityDistribution(this->greedyActionForState(s));
    }
}