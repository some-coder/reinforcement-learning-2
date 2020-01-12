#include "RandomServices.hpp"
#include "PolicyIterationPlayer.hpp"

PolicyIterationPlayer::PolicyIterationPlayer(Maze *m, double gamma, double theta) :
        DynamicProgrammingPlayer(m, gamma, theta) {
    this->policyIsStable = true;
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

/**
 * This function is called when the player is asked to solve the maze, i.e. start from your starting position and
 * try to move towards the end of the maze. It does this by repeating the steps of performing evaluation of the
 * policy and then improving the policy where needed.
 */
void PolicyIterationPlayer::solveMaze() {
    //TODO: why is there a variable i?
    int i = 0;
    do {
        auto startTime = std::chrono::high_resolution_clock::now();
        this->policyIsStable = true;
        this->performEvaluationStep();
        this->performImprovementStep();
        i++;
        auto endTime = std::chrono::high_resolution_clock::now();
        this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
    } while (!this->policyIsStable);
}

PolicyIterationPlayer::~PolicyIterationPlayer() = default;
