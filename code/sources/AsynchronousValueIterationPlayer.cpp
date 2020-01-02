#include "AsynchronousValueIterationPlayer.hpp"

AsynchronousValueIterationPlayer::AsynchronousValueIterationPlayer(Maze *m, double gamma, double theta,
        int maximumIteration) : ValueIterationPlayer(m, gamma, theta) {
    this->iteration = -1;
    this->maximumIteration = maximumIteration;
}

AsynchronousValueIterationPlayer::~AsynchronousValueIterationPlayer() = default;

void AsynchronousValueIterationPlayer::performRepetitionStep() {
    double delta;
    State *s;
    do {
        this->iteration++;
        delta = 0.0;
        s = this->randomNontrivialState();
        this->stateValues[s] = this->updatedStateValue(s);
        delta = std::max(delta, std::fabs(this->oldStateValues[s] - this->stateValues[s]));
        this->copyStateValues(&(this->stateValues), &(this->oldStateValues));
    } while (this->iteration < this->maximumIteration || delta >= this->theta);
}

void AsynchronousValueIterationPlayer::performConstructionStep() {
    int stateIndex;
    State* s;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        s = this->maze->getState(stateIndex);
        this->policy[s] = Player::actionAsActionProbabilityDistribution(
            this->greedyActionForState(s));
    }
}