#include <algorithm>
#include <cmath>
#include "SynchronousPolicyIterationPlayer.hpp"

/**
 * Constructs a new synchronous policy iteration player.
 *
 * @param m The maze to be solved by the player.
 * @param gamma The discount factor to apply to earlier-obtained rewards.
 * @param theta The minimal utility difference to decide to keep iterating.
 */
SynchronousPolicyIterationPlayer::SynchronousPolicyIterationPlayer(Maze *m, double gamma, double theta) :
    PolicyIterationPlayer(m, gamma, theta) {}

/**
 * Destructs the synchronous policy iteration player.
 */
SynchronousPolicyIterationPlayer::~SynchronousPolicyIterationPlayer() = default;

/**
 * Performs the evaluation step of the policy iteration algorithm.
 */
void SynchronousPolicyIterationPlayer::performEvaluationStep() {
    int i;
    double delta, oldValue;
    State *s;
    do {
        delta = 0.0;
        for (i = 0; i < (int)this->maze->getStates()->size(); i++) {
            s = this->maze->getState(i);
            if (Maze::stateIsIntraversible(s) || Maze::stateIsTerminal(s)) {
                /* We shouldn't alter these states in any way. */
                continue;
            }
            oldValue = this->stateValues[s];
            this->stateValues[s] = this->updatedStateValue(s, Maze::actionFromIndex(this->chooseAction(s)));
            delta = std::max(delta, std::fabs(oldValue - this->stateValues[s]));
        }
        this->copyStateValues(&(this->stateValues), &(this->oldStateValues));
    } while (delta >= this->theta);
}

/**
 * Performs the improvement step of the policy iteration algorithm.
 */
void SynchronousPolicyIterationPlayer::performImprovementStep() {
    int i;
    State *s;
    Maze::Actions current, greedy;
    for (i = 0; i < (int)this->maze->getStates()->size(); i++) {
        s = this->maze->getState(i);
        if (Maze::stateIsIntraversible(s) || Maze::stateIsTerminal(s)) {
            continue;
        }
        current = this->chooseAction(s);
        greedy = this->greedyActionForState(s);
        this->policy[s] = Player::actionAsActionProbabilityDistribution(greedy);
        if (current != greedy && !Maze::stateIsIntraversible(s) &&
                !Maze::stateIsTerminal(s)) {
            this->policyIsStable = false;
        }
    }
}