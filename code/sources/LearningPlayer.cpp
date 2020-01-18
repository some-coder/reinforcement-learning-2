#include "MonteCarloPlayer.hpp"
#include "LearningPlayer.hpp"

LearningPlayer::LearningPlayer(Maze *m, double gamma, int T, bool initialiseStochastic) :
        Player(m, gamma, initialiseStochastic) {
    this->timeoutEpoch = T;
    this->currentEpoch = 0;
    this->initialiseStateActionValues();
}

/**
 * This function sets a value in state-action values of a certain state-action pair to the 
 * given value.
 * 
 * @param s The state 
 * @param action The action
 * @param value The state-action value for the given state-action pair
 */
void LearningPlayer::setStateActionValue(State *s, Maze::Actions action, double value) {
    this->stateActionValues[std::make_tuple(s, action)] = value;
}

/**
 * This function initializes the values of state-action values to zero. State-action values are 
 * later used to update the policy of a player.
 */
void LearningPlayer::initialiseStateActionValues() {
    int i;
    State *state;
    std::vector<State>* states;
    states = this->maze->getStates();
    for (i = 0; i < (int)states->size(); i++) {
        state = &(states->at(i));
        this->setStateActionValue(state, Maze::Actions::moveUp, INITIAL_STATE_ACTION_VALUE);
        this->setStateActionValue(state, Maze::Actions::moveRight, INITIAL_STATE_ACTION_VALUE);
        this->setStateActionValue(state, Maze::Actions::moveDown, INITIAL_STATE_ACTION_VALUE);
        this->setStateActionValue(state, Maze::Actions::moveLeft, INITIAL_STATE_ACTION_VALUE);
    }
}

LearningPlayer::~LearningPlayer() = default;

/**
 * Print the state-action values of the player for a certain state.
 * 
 * @param s The state for which you want to print the state-action values
 */
void LearningPlayer::printStateActionValues(State* s) {
    int actionIndex;
    std::string actionAsString;
    Maze::Actions action;
    std::tuple<State*, Maze::Actions> stateActionPair;
    double value;
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        action = Maze::actionFromIndex(actionIndex);
        actionAsString = Maze::actionAsString(action);
        stateActionPair = std::make_tuple(s, action);
        value = this->stateActionValues[stateActionPair];
        printf("\t\t%s has value %.3lf.\n", actionAsString.c_str(), value);
    }
}

/**
 * Print the policy of the player.
 */
void LearningPlayer::printFinalPolicy() {
    int stateIndex;
    State *s;
    std::string policyAction;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        s = this->maze->getState(stateIndex);
        printf("\tState (%d, %d) ", s->getX(), s->getY());
        if (Maze::stateIsTerminal(s)) {
            printf("is terminal.\n");
        } else if (Maze::stateIsIntraversible(s)) {
            printf("is intraversible.\n");
        } else {
            policyAction = Maze::actionAsString(this->chooseAction(s));
            printf("has action probability distribution ");
            this->printStateActionProbabilities(s);
            printf(".\n");
            this->printStateActionValues(s);
        }
    }
}

/**
 * Returns the action with the highest stateActionValue in the provided state.
 *
 * @param s the state for which the action has to be chosen
 * @return an action
 */
Maze::Actions LearningPlayer::greedyAction(State *s) {
    int actionIndex;
    std::tuple<State*, Maze::Actions> stateActionPair;
    Maze::Actions currentAction, bestAction;
    double current, best;
    bestAction = Maze::actionFromIndex(0);
    stateActionPair = std::make_tuple(s, bestAction);
    best = this->stateActionValues[stateActionPair];
    for (actionIndex = 1; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        currentAction = Maze::actionFromIndex(actionIndex);
        stateActionPair = std::make_tuple(s, currentAction);
        current = this->stateActionValues[stateActionPair];
        if (current > best) {
            bestAction = currentAction;
            best = current;
        }
    }
    return bestAction;
}

void LearningPlayer::addRewardsToTotalRewardPerEpisode() {
    int rewardIndex;
    double total;
    total = 0.0;
    for (rewardIndex = 0; rewardIndex < (int)this->rewards.size(); rewardIndex++) {
        total += this->rewards[rewardIndex];
    }
    this->totalRewardPerEpisode.push_back(total);
}

std::vector<double> LearningPlayer::getTotalRewardPerEpisode() {
    return this->totalRewardPerEpisode;
}