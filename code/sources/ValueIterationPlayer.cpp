#include "ValueIterationPlayer.hpp"

/**
 * Constructs a value iteration player.
 *
 * @param m The maze to be solved by the player.
 * @param gamma The discount factor to be applied to earlier-obtained rewards.
 * @param theta The minimum utility difference to decide to keep iterating.
 */
ValueIterationPlayer::ValueIterationPlayer(Maze *m, double gamma, double theta) : DynamicProgrammingPlayer(m, gamma, theta) {}

/**
 * Destructs a value iteration player.
 */
ValueIterationPlayer::~ValueIterationPlayer() = default;

/**
 * Returns the value of the state-action pair.
 *
 * @param s The state of the state-action pair.
 * @param a The action of the state-action pair.
 * @return The value of the pair.
 */
double ValueIterationPlayer::stateActionValue(State *s, Maze::Actions a) {
    int relativeActionIndex;
    Maze::Actions action;
    State *nextState;
    double nextStateProbability, totalValue;
    totalValue = 0.0;
    for (relativeActionIndex = 0; relativeActionIndex < Maze::ACTION_NUMBER; relativeActionIndex++) {
        action = Maze::actionFromIndex( (a + relativeActionIndex) % Maze::ACTION_NUMBER );
        nextState = this->maze->getNextStateDeterministic(s, action);
        nextStateProbability = this->maze->getActionProbability(relativeActionIndex);
        totalValue += nextStateProbability * (Maze::getReward(nextState) +
                this->discountFactor * this->oldStateValues[nextState]);
    }
    return totalValue;
}

/**
 * Obtains the revised value of the supplied state.
 *
 * @param s The state to calculate a revised value for.
 * @return The state's revised value.
 */
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

/**
 * Obtains the greedy action to perform in the supplied state.
 *
 * @param s The state of which to obtain the greedy action.
 * @return The state's greedy action.
 */
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

/**
 * Solves the maze the player got tasked with.
 */
void ValueIterationPlayer::solveMaze() {
    this->performRepetitionStep();
    this->performConstructionStep();
}