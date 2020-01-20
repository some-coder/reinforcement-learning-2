#include "QLearningPlayer.hpp"

/**
 * Constructs a time-difference Q-learning player.
 *
 * @param m The maze to be solved by the player.
 * @param gamma The discount factor to apply to earlier-obtained rewards.
 * @param T The maximum iteration before forcing a stop.
 * @param alpha The learning rate.
 * @param epsilon The exploration rate.
 */
QLearningPlayer::QLearningPlayer(Maze *m, double gamma, int T, double alpha, double epsilon) :
        TimeDifferencePlayer(m, gamma, T, alpha, epsilon) {
    this->updatePolicyUsingStateActionValues();
}

/**
 * Destructs the time-difference Q-learning player.
 */
QLearningPlayer::~QLearningPlayer() = default;

/**
 * The public function that gets called when you want the player to play and solve the maze
 * whilst learning from doing so.
 */
void QLearningPlayer::solveMaze() {
    do {
        auto startTime = std::chrono::high_resolution_clock::now();
        this->performIteration();
        auto endTime = std::chrono::high_resolution_clock::now();
        this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
    } while (!this->maximumIterationReached());
}

/**
 * Perform iteration is a function that controls the generating of a single episode where the 
 * player tries to navigate the maze. Every iteration adds 1 to the current epoch.
 */
void QLearningPlayer::performIteration() {
    std::tuple<State*, Maze::Actions> startStateActionPair;
    startStateActionPair = this->initialStateActionPair();
    this->generateEpisode(startStateActionPair);
    this->addRewardsToTotalRewardPerEpisode();
    this->currentEpoch++;
    this->rewards.clear();
}

/**
 * This function generates the episode where the player tries to navigate the maze. It updates the
 * state values (which influences the policy) according to the Q-learning update function. This
 * function continues until the player reached the end of the maze or has reached the maximum
 * amount of steps for an episode.
 */
void QLearningPlayer::generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) {
    std::tuple<State*, Maze::Actions> stateActionPair, nextStateActionPair;
    int currentIteration, maximumIteration;
    maximumIteration = std::ceil(EPISODE_TIMEOUT_FRACTION * (double)this->stateValues.size());
    currentIteration = -1;
    stateActionPair = startStateActionPair;
    do {
        currentIteration++;
        nextStateActionPair = this->nextStateActionPair(stateActionPair);
        this->updateStateActionValue(stateActionPair, nextStateActionPair);
        this->updatePolicyUsingStateActionValues(std::get<0>(stateActionPair));
        stateActionPair = nextStateActionPair;
    } while (currentIteration < maximumIteration && !Maze::stateIsTerminal(std::get<0>(stateActionPair)));
    this->maze->resetMaze();
}

/**
 * This function updates the state values according to the Q-learning update function.
 */
void QLearningPlayer::updateStateActionValue(std::tuple<State*, Maze::Actions> stateActionPair, std::tuple<State*,
        Maze::Actions> nextStateActionPair) {
    std::tuple<State*, Maze::Actions> greedyNextStateActionPair;
    greedyNextStateActionPair = std::make_tuple(std::get<0>(nextStateActionPair),
            this->greedyAction(std::get<0>(nextStateActionPair)));
    this->stateActionValues[stateActionPair] = this->stateActionValues[stateActionPair] +
            this->alpha * (this->rewards.back() + this->discountFactor * this->stateActionValues[greedyNextStateActionPair]
            - this->stateActionValues[stateActionPair]);
}

/**
 * This function returns the state action pair of the start state and its first action.
 * 
 * @return startStateActionPair The state action pair where the player should begin
 */
std::tuple<State*, Maze::Actions> QLearningPlayer::initialStateActionPair() {
    State *s;
    Maze::Actions a;
    s = this->maze->getStartingState();
    a = this->chooseAction(s);
    return std::make_tuple(s, a);
}

/**
 * This function returns the resulting state after the player did an action combined with the new
 * action according to the policy for the new state.
 * 
 * @param currentPair The state which the player is in first combined with the action it's going to do
 * @return The state action pair of the new state the player is in
 */
std::tuple<State*, Maze::Actions> QLearningPlayer::nextStateActionPair(
        std::tuple<State *, Maze::Actions> currentPair) {
    std::tuple<State*, double> result;
    result = this->maze->getStateTransitionResult(std::get<0>(currentPair), std::get<1>(currentPair));
    this->rewards.push_back(std::get<1>(result));
    return std::make_tuple(std::get<0>(result), this->chooseAction(std::get<0>(result)));
}

/**
 * Update the policy values according to an epsilon-greedy representation of the state values.
 */
void QLearningPlayer::updatePolicyUsingStateActionValues() {
    State *state;
    Maze::Actions greedyAction, currentAction;
    std::vector<State>* states;
    std::vector<double> newPolicy;
    double newActionProbability;
    states = this->maze->getStates();
    for (int i = 0; i < (int)states->size(); i++) {
        state = &(states->at(i));
        greedyAction = this->greedyAction(state);
        for (int actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
            currentAction = Maze::actionFromIndex(actionIndex);
            if (currentAction == greedyAction) {
                newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER) + 1.0 - this->epsilon;
            } else {
                newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER);
            }
            newPolicy.push_back(newActionProbability);
        }
        this->policy[state] = newPolicy;
    }
}

/**
 * This function updates the policy of the provided state using an epsilon-greedy approach.
 * It is epsilon-greedy for the state action values.
 *
 * @param state The state for which we update the policy
 */
void QLearningPlayer::updatePolicyUsingStateActionValues(State *state) {
    Maze::Actions greedyAction, currentAction;
    std::vector<double> newPolicy;
    double newActionProbability;
    greedyAction = this->greedyAction(state);
    for (int actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        currentAction = Maze::actionFromIndex(actionIndex);
        if (currentAction == greedyAction) {
            newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER) + 1.0 - this->epsilon;
        } else {
            newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER);
        }
        newPolicy.push_back(newActionProbability);
    }
    this->policy[state] = newPolicy;
}
