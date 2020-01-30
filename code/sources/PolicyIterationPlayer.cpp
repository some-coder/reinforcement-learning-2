#include "PolicyIterationPlayer.hpp"

/**
 * Constructs a policy iteration player.
 *
 * @param m The maze to be solved by the player.
 * @param gamma The discount factor to apply to earlier-obtained rewards.
 * @param theta The minimal utility difference to decide to keep iterating.
 */
PolicyIterationPlayer::PolicyIterationPlayer(Maze *m, double gamma, double theta) :
        DynamicProgrammingPlayer(m, gamma, theta) {
    this->policyIsStable = true;
}

/**
 * Destructs the policy iteration player.
 */
PolicyIterationPlayer::~PolicyIterationPlayer() = default;

/**
 * Obtains the valuation of the given state, if its dominant action is supplied.
 *
 * @param s The state to evaluate.
 * @param a The dominant action to undertake in this state.
 * @return The state's value.
 */
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

/**
 * Obtains an updated version of the state's value, given current information.
 *
 * @param s The state to evaluate.
 * @param a The dominant action to undertake in this state.
 * @return The state's value.
 */
double PolicyIterationPlayer::updatedStateValue(State *s, Maze::Actions a) {
    return Maze::getReward(s) + this->discountFactor * this->stateValue(s, a);
}

/**
 * Obtains the greedy action for the supplied state.
 *
 * @param s The state for which to get the greedy action.
 * @return The state's greedy action.
 */
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
    do {
        auto startTime = std::chrono::high_resolution_clock::now();
        this->policyIsStable = true;
        this->performEvaluationStep();
        this->performImprovementStep();
        auto endTime = std::chrono::high_resolution_clock::now();
        this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
    } while (!this->policyIsStable);
}
