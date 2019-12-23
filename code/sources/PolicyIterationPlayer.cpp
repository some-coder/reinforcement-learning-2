#include <algorithm>
#include <cmath>
#include <iostream>
#include "RandomServices.hpp"
#include "PolicyIterationPlayer.hpp"

std::vector<double> PolicyIterationPlayer::randomStatePolicy() {
    int randomIndex, i;
    std::vector<double> statePolicy;
    randomIndex = RandomServices::discreteUniformSample(Maze::ACTION_NUMBER - 1);
    for (i = 0; i < Maze::ACTION_NUMBER; i++) {
        /* Todo: Replace 4s throughout the code by ACTION_NUMBER. */
        /* Todo: Check randomness of policy initialisation. */
        statePolicy.push_back((i == randomIndex ? 1.0 : 0.0));
    }
    return statePolicy;
}

/* Note: this is a deterministic policy, not a stochastic one. */
void PolicyIterationPlayer::initialisePolicy() {
    int i;
    State* s;
    std::vector<State> *states;
    states = this->maze->getStates();
    for (i = 0; i < states->size(); i++) {
        s = &(states->at(i));
        this->policy[s] = this->randomStatePolicy();
    }
}

void PolicyIterationPlayer::copyStateValues(std::map<State*, double> *source, std::map<State*, double> *target) {
    int i;
    double stateValue;
    State *s;
    for (i = 0; i < this->maze->getStates()->size(); i++) {
        s = this->maze->getState(i);
        stateValue = source->at(s);
        (*target)[s] = stateValue;
    }
}

PolicyIterationPlayer::PolicyIterationPlayer(Maze *m, double gamma, double theta) :
        DynamicProgrammingPlayer(m, gamma, theta) {
    this->policyIsStable = false;
    this->initialisePolicy();
    this->copyStateValues(&(this->stateValues), &(this->oldStateValues));
}

double PolicyIterationPlayer::stateEvaluation(State *s, Maze::Actions a) {
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
    return Maze::getReward(s) + this->discountFactor * newValue;
}

Maze::Actions PolicyIterationPlayer::greedyActionForState(State *s) {
    double bestValue, currentValue;
    int bestIndex, i;
    bestIndex = 0;
    bestValue = this->stateEvaluation(s, Maze::actionFromIndex(bestIndex));
    for (i = 1; i < Maze::ACTION_NUMBER; i++) {
        currentValue = this->stateEvaluation(s, Maze::actionFromIndex(i));
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
    printf("Before:\n");
    this->printFinalPolicy();
    do {
        delta = 0.0;
        for (i = 0; i < this->maze->getStates()->size(); i++) {
            s = this->maze->getState(i);
            if (Maze::stateIsIntraversible(s) || s->getType() == State::Types::goal ||
                    s->getType() == State::Types::pit) {
                /* We shouldn't alter these states in any way. */
                continue;
            }
            oldValue = this->stateValues[s];
            this->stateValues[s] = this->stateEvaluation(s, Maze::actionFromIndex(this->chooseAction(s)));
            delta = std::max(delta, std::fabs(oldValue - this->stateValues[s]));
        }
        this->copyStateValues(&(this->stateValues), &(this->oldStateValues));
    } while (delta >= this->theta);
    printf("After:\n");
    this->printFinalPolicy();
    printf("\n");
}

void PolicyIterationPlayer::performImprovementStep() {
    int i;
    State *s;
    Maze::Actions current, greedy;
    for (i = 0; i < this->maze->getStates()->size(); i++) {
        s = this->maze->getState(i);
        if (Maze::stateIsIntraversible(s)) {
            continue;
        }
        current = this->chooseAction(s);
        greedy = this->greedyActionForState(s);
        if (current != greedy) {
            this->policyIsStable = false;
            break;
        }
    }
}

void PolicyIterationPlayer::solveMaze() {
    this->printFinalPolicy();
    int i = 0;
    while (!this->policyIsStable && i < 100) {
        this->performEvaluationStep();
//        this->performImprovementStep();
        i++;
    }
}

PolicyIterationPlayer::~PolicyIterationPlayer() = default;
