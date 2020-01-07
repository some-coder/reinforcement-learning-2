#include <cmath>
#include "AsynchronousPolicyIterationPlayer.hpp"

AsynchronousPolicyIterationPlayer::AsynchronousPolicyIterationPlayer(Maze *m, double gamma, double theta,
        int maximumIteration) : PolicyIterationPlayer(m, gamma, theta) {
    this->currentState = nullptr;
    this->iteration = 0;
    this->maximumIteration = maximumIteration;
}

AsynchronousPolicyIterationPlayer::~AsynchronousPolicyIterationPlayer() = default;

void AsynchronousPolicyIterationPlayer::performEvaluationStep() {
    double delta, oldValue;
    State *s;
    do {
        delta = 0.0;
        s = this->randomNontrivialState();
        oldValue = this->stateValues[s];
        this->stateValues[s] = this->updatedStateValue(s, Maze::actionFromIndex(this->chooseAction(s)));
        delta = std::max(delta, std::fabs(oldValue - this->stateValues[s]));
        this->copyStateValues(&(this->stateValues), &(this->oldStateValues));
        this->currentState = s;
    } while (delta >= this->theta);
}

void AsynchronousPolicyIterationPlayer::performImprovementStep() {
    Maze::Actions greedy;
    greedy = this->greedyActionForState(this->currentState);
    this->policy[this->currentState] = Player::actionAsActionProbabilityDistribution(greedy);
    if (this->iteration < this->maximumIteration) {
        this->policyIsStable = false;
    }
    this->iteration++;
}