#include <algorithm>
#include <cmath>
#include "SynchronousPolicyIterationPlayer.hpp"

/* Todo: This method gets stuck sometimes. */

SynchronousPolicyIterationPlayer::SynchronousPolicyIterationPlayer(Maze *m, double gamma, double theta) :
    PolicyIterationPlayer(m, gamma, theta) {}

SynchronousPolicyIterationPlayer::~SynchronousPolicyIterationPlayer() = default;

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