#include "LearningPlayer.hpp"

/**
 * Constructs a learning player.
 *
 * @param m The maze the player needs to solve.
 * @param gamma The discount factor to be applied to earlier-obtained rewards.
 * @param T The timeout epoch.
 * @param initialiseStochastic Should the player be initialised stochastically?
 */
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

/**
 * Destructs the current player.
 */
LearningPlayer::~LearningPlayer() = default;

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
        /* Per alternative action, check whether it trumps our current
           action. */
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

/**
 * Accumulates the rewards of the current episode, and appends it to the totals.
 */
void LearningPlayer::addRewardsToTotalRewardPerEpisode() {
    int rewardIndex;
    double total;
    total = 0.0;
    for (rewardIndex = 0; rewardIndex < (int)this->rewards.size(); rewardIndex++) {
        total += this->rewards[rewardIndex];
    }
    this->totalRewardPerEpisode.push_back(total);
}

/**
 * Obtains the total reward obtained per episode.
 *
 * @return The episode reward totals.
 */
std::vector<double> LearningPlayer::getTotalRewardPerEpisode() {
    return this->totalRewardPerEpisode;
}