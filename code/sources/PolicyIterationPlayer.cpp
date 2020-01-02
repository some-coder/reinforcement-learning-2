#include <algorithm>
#include <cmath>
#include <iostream>
#include "RandomServices.hpp"
#include "PolicyIterationPlayer.hpp"

std::vector<double> PolicyIterationPlayer::randomStatePolicy() {
    int randomIndex;
    std::vector<double> statePolicy;
    randomIndex = RandomServices::discreteUniformSample(Maze::ACTION_NUMBER - 1);
    return Player::actionAsActionProbabilityDistribution(Maze::actionFromIndex(randomIndex));
}

/* Note: this is a deterministic policy, not a stochastic one. */
void PolicyIterationPlayer::initialisePolicy() {
    int i;
    State* s;
    std::vector<State> *states;
    states = this->maze->getStates();
    for (i = 0; i < (int)states->size(); i++) {
        s = &(states->at(i));
        this->policy[s] = this->randomStatePolicy();
    }
}

PolicyIterationPlayer::PolicyIterationPlayer(Maze *m, double gamma, double theta) :
        DynamicProgrammingPlayer(m, gamma, theta) {
    this->policyIsStable = true;
    this->initialisePolicy();
}

double PolicyIterationPlayer::stateValue(State *s, Maze::Actions a) {
    int i;
    double newValue;
    State *nextState;
    Maze::Actions actualAction;
    newValue = 0.0;
    for (i = 0; i < Maze::ACTION_NUMBER; i++) {
        /* Given our choice of action, consider all possible results and their valuations. */
        actualAction = Maze::actionFromIndex((a + i) % Maze::ACTION_NUMBER);
        nextState = this->maze->getNextStateDeterministic(s, actualAction);
        newValue += this->maze->getActionProbability(i) * this->oldStateValues[nextState];
    }
    return newValue;
}

double PolicyIterationPlayer::updatedStateValue(State *s, Maze::Actions a) {
    return Maze::getReward(s) + this->discountFactor * this->stateValue(s, a);
}

Maze::Actions PolicyIterationPlayer::greedyActionForState(State *s) {
    double bestValue, currentValue;
    int bestIndex, i;
    bestIndex = 0;
    bestValue = this->stateValue(s, Maze::actionFromIndex(bestIndex));
    for (i = 1; i < Maze::ACTION_NUMBER; i++) {
        currentValue = this->stateValue(s, Maze::actionFromIndex(i));
        if (currentValue > bestValue) {
            bestIndex = i;
            bestValue = currentValue;
        }
    }
    return Maze::actionFromIndex(bestIndex);
}

void PolicyIterationPlayer::performEvaluationStep() {
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

void PolicyIterationPlayer::performImprovementStep() {
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

void PolicyIterationPlayer::solveMaze() {
    int i = 0;
    do {
        this->policyIsStable = true;
        this->performEvaluationStep();
        this->performImprovementStep();
        i++;
    } while (!this->policyIsStable);
    printf("Took %d epochs.\n", i);
    this->printFinalPolicy();
}

PolicyIterationPlayer::~PolicyIterationPlayer() = default;
