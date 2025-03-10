#include "SarsaPlayer.hpp"

/**
 * Constructs a time-difference SARSA player.
 *
 * @param m The maze to be solved by the player.
 * @param gamma The discount factor to apply to earlier-obtained rewards.
 * @param T The maximum iteration before forcing a stop.
 * @param alpha The learning rate.
 * @param epsilon The exploration rate.
 */
SarsaPlayer::SarsaPlayer(Maze *m, double gamma, int T, double alpha, double epsilon) : TimeDifferencePlayer(m, gamma, T, alpha, epsilon) {
    this->updatePolicyUsingQuality();   /* Overwrite parent's policy construction, which was arbitrary. */
}

/**
 * Destructs the time-difference SARSA player.
 */
SarsaPlayer::~SarsaPlayer() = default;

/**
 * The public function that gets called when you want the player to play and solve the maze
 * whilst learning from doing so.
 */
void SarsaPlayer::solveMaze() {
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
void SarsaPlayer::performIteration() {
    std::tuple<State*, Maze::Actions> startStateActionPair;
    startStateActionPair = this->initialStateActionPair();
    this->generateEpisode(startStateActionPair);
    this->addRewardsToTotalRewardPerEpisode();
    this->currentEpoch++;
    this->rewards.clear();
}

/**
 * This function generates the episode where the player tries to navigate the maze. It updates the
 * quality values (which influences the policy) according to the SARSA update function. This
 * function continues until the player reached the end of the maze or has reached the maximum
 * amount of steps for an episode.
 */
void SarsaPlayer::generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) {
    int currentIteration, maximumIteration;
    double reward;
    std::tuple<State*, Maze::Actions> stateActionPair, nextStateActionPair;
    currentIteration = -1;
    maximumIteration = std::ceil(EPISODE_TIMEOUT_FRACTION * (double)this->stateValues.size());
    stateActionPair = startStateActionPair;
    do {
        currentIteration++;
        nextStateActionPair = this->nextStateActionPair(stateActionPair);
        reward = this->rewards.back();
        this->quality[stateActionPair] += this->alpha *
                (reward + (this->discountFactor * this->quality[nextStateActionPair]) - this->quality[stateActionPair]);
        this->updatePolicyUsingQuality(std::get<0>(stateActionPair));
        stateActionPair = nextStateActionPair;
    } while (currentIteration < maximumIteration && !Maze::stateIsTerminal(std::get<0>(stateActionPair)));
    this->maze->resetMaze();
}

/**
 * This function loops through all states and updates for every state the policy using an epsilon-greedy approach.
 * It is epsilon-greedy with respect to the quality of the action.
 */
void SarsaPlayer::updatePolicyUsingQuality() {
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
 * It is epsilon-greedy for the quality.
 * 
 * @param state The state for which we update the policy
 */
void SarsaPlayer::updatePolicyUsingQuality(State *state) {
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

/**
 * This function returns the state action pair of the start state and its first action.
 * 
 * @return The state action pair where the player should begin
 */
std::tuple<State*, Maze::Actions> SarsaPlayer::initialStateActionPair() {
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
std::tuple<State*, Maze::Actions> SarsaPlayer::nextStateActionPair(
        std::tuple<State*, Maze::Actions> currentPair) {
    std::tuple<State*, double> result;
    result = this->maze->getStateTransitionResult(std::get<0>(currentPair), std::get<1>(currentPair));
    this->rewards.push_back(std::get<1>(result));
    return std::make_tuple(std::get<0>(result), this->chooseAction(std::get<0>(result)));
}

/**
 * Returns the action with the highest quality in the provided state.
 *
 * @param s the state for which the action has to be chosen
 * @return an action
 */
Maze::Actions SarsaPlayer::greedyAction(State *s) {
    int actionIndex;
    std::tuple<State*, Maze::Actions> stateActionPair;
    Maze::Actions currentAction, bestAction;
    double current, best;
    bestAction = Maze::actionFromIndex(0);
    stateActionPair = std::make_tuple(s, bestAction);
    best = this->quality[stateActionPair];
    for (actionIndex = 1; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        currentAction = Maze::actionFromIndex(actionIndex);
        stateActionPair = std::make_tuple(s, currentAction);
        current = this->quality[stateActionPair];
        if (current > best) {
            bestAction = currentAction;
            best = current;
        }
    }
    return bestAction;
}
