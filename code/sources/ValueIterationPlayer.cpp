#include <cmath>
#include "ValueIterationPlayer.hpp"

ValueIterationPlayer::ValueIterationPlayer(Maze *m, double gamma, double theta) : DynamicProgrammingPlayer(m, gamma, theta) {}

ValueIterationPlayer::~ValueIterationPlayer() = default;

double ValueIterationPlayer::stateActionValue(State *s, Maze::Actions a) {
    int relativeActionIndex;
    Maze::Actions action;
    State *nextState;
    double nextStateProbability, totalValue;
    totalValue = 0.0;
    for (relativeActionIndex = 0; relativeActionIndex < Maze::ACTION_NUMBER; relativeActionIndex++) {
        action = Maze::actionFromIndex( (a + relativeActionIndex) % Maze::ACTION_NUMBER );
        nextState = this->maze->getNextStateDeterministic(s, action);
        nextStateProbability = this->maze->getActionProbability(action);
        totalValue += nextStateProbability * (Maze::getReward(nextState) +
                this->discountFactor * this->oldStateValues[nextState]);
    }
    return totalValue;
}

double ValueIterationPlayer::updatedStateValue(State *s) {
    int actionIndex;
    Maze::Actions a;
    double current, best;
    best = this->stateActionValue(s, Maze::actionFromIndex(0));
    for (actionIndex = 1; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        a = Maze::actionFromIndex(actionIndex);
        current = this->stateActionValue(s, a);
        best = (current > best ? current : best);
    }
    return best;
}

Maze::Actions ValueIterationPlayer::greedyActionForState(State *s) {
    int actionIndex;
    Maze::Actions current, best;
    double currentValue, bestValue;
    best = Maze::actionFromIndex(0);
    bestValue = this->stateActionValue(s, best);
    for (actionIndex = 1; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        current = Maze::actionFromIndex(actionIndex);
        currentValue = this->stateActionValue(s, current);
        if (currentValue > bestValue) {
            best = current;
            bestValue = currentValue;
        }
    }
    return best;
}

void ValueIterationPlayer::performRepetitionStep() {
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

void ValueIterationPlayer::performConstructionStep() {
    int stateIndex;
    State *s;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        s = this->maze->getState(stateIndex);
        this->policy[s] = Player::actionAsActionProbabilityDistribution(this->greedyActionForState(s));
    }
}

void ValueIterationPlayer::solveMaze() {
    this->performRepetitionStep();
    this->performConstructionStep();
}