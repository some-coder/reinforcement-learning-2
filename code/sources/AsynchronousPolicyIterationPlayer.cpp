#include <cmath>
#include "AsynchronousPolicyIterationPlayer.hpp"

/**
 * Constructs an asynchronous policy iteration player.
 *
 * @param m The maze to be solved by the player.
 * @param gamma The discount factor to apply to earlier-obtained rewards.
 * @param theta The minimal utility difference to decide to keep iterating.
 * @param maximumIteration The maximum iteration to keep running for.
 */
AsynchronousPolicyIterationPlayer::AsynchronousPolicyIterationPlayer(Maze *m, double gamma, double theta,
        int maximumIteration) : PolicyIterationPlayer(m, gamma, theta) {
    this->currentState = nullptr;
    this->iteration = 0;
    this->maximumIteration = maximumIteration;
}

/**
 * Destructs the asynchronous policy iteration player.
 */
AsynchronousPolicyIterationPlayer::~AsynchronousPolicyIterationPlayer() = default;

/**
 * Performs the evaluation step of the asynchronous policy iteration player.
 */
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

/**
 * Performs the improvement step of the asynchronous policy iteration player.
 */
void AsynchronousPolicyIterationPlayer::performImprovementStep() {
    Maze::Actions greedy;
    greedy = this->greedyActionForState(this->currentState);
    this->policy[this->currentState] = Player::actionAsActionProbabilityDistribution(greedy);
    if (this->iteration < this->maximumIteration) {
        this->policyIsStable = false;
    }
    this->iteration++;
}