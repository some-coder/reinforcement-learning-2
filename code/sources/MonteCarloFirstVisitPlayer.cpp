#include "MonteCarloFirstVisitPlayer.hpp"
#include "LearningPlayer.hpp"

/**
 * Constructs a Monte Carlo first-visit player.
 *
 * @param m The maze to be solved by the player.
 * @param gamma The discount factor to apply to earlier-obtained rewards.
 * @param T The minimal utility difference to decide to keep iterating.
 * @param epsilon The error rate.
 */
MonteCarloFirstVisitPlayer::MonteCarloFirstVisitPlayer(Maze *m, double gamma, int T, double epsilon) :
        MonteCarloPlayer(m, gamma, T, true) {
    this->epsilon = epsilon;
}

/**
 * Destructs the Monte Carlo first-visit player.
 */
MonteCarloFirstVisitPlayer::~MonteCarloFirstVisitPlayer() = default;

/**
 * Obtains the first state-action pair for an episode generation.
 *
 * @return The initial state-action pair.
 */
std::tuple<State*, Maze::Actions> MonteCarloFirstVisitPlayer::initialStateActionPair() {
    State *s;
    Maze::Actions a;
    s = this->maze->getStartingState();
    a = this->chooseAction(s);
    return std::make_tuple(s, a);
}

/**
 * Obtains the succeeding state-action pair, given a current tuple.
 *
 * @param currentPair The current state-action pair.
 * @return The successor tuple.
 */
std::tuple<State*, Maze::Actions> MonteCarloFirstVisitPlayer::nextStateActionPair(
        std::tuple<State*, Maze::Actions> currentPair) {
    std::tuple<State*, double> result;
    result = this->maze->getStateTransitionResult(std::get<0>(currentPair), std::get<1>(currentPair));
    this->rewards.push_back(std::get<1>(result));
    return std::make_tuple(std::get<0>(result), this->chooseAction(std::get<0>(result)));
}

/**
 * Generates a complete episode of successive state-action pairs.
 *
 * @param startStateActionPair The initial state-action pair to depart from.
 */
void MonteCarloFirstVisitPlayer::generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) {
    int currentIteration, maximumIteration;
    std::tuple<State*, Maze::Actions> stateActionPair;
    currentIteration = -1;
    maximumIteration = std::ceil(EPISODE_TIMEOUT_FRACTION * (double)this->stateValues.size());
    stateActionPair = startStateActionPair;
    this->episode.push_back(stateActionPair);
    this->rewards.push_back(0.0);  /* First iteration has no reward. */
    do {
        currentIteration++;
        stateActionPair = this->nextStateActionPair(stateActionPair);
        this->episode.push_back(stateActionPair);
    } while (currentIteration < maximumIteration && !Maze::stateIsTerminal(std::get<0>(stateActionPair)));
    this->maze->resetMaze();
}

/**
 * Computes a single component of the episode's returns.
 *
 * @param k The exponent to raise the gamma to.
 * @param rewardIndex The index of the current reward component.
 * @return The value of the component of the episode's returns.
 */
double MonteCarloFirstVisitPlayer::episodeReturnComponent(int k, int rewardIndex) {
    return std::pow(this->discountFactor, k) * this->rewards[rewardIndex];
}

/**
 * Computes the current episode's total returns.
 *
 * @param onsetIndex The index from which to start.
 * @return The episode's total returns.
 */
double MonteCarloFirstVisitPlayer::episodeReturn(int onsetIndex) {
    int index;
    double episodeReturn;
    episodeReturn = 0.0;
    for (index = onsetIndex; index < (int)this->rewards.size(); index++) {
        episodeReturn += this->episodeReturnComponent(index - onsetIndex, index);
    }
    return episodeReturn;
}

/**
 * Computes the average of the current episode's returns.
 *
 * @param stateActionPair THe state-action pair of which to get the mean return.
 * @return The state-action pair's average returns over episodes.
 */
double MonteCarloFirstVisitPlayer::returnsAverage(std::tuple<State*, Maze::Actions> stateActionPair) {
    int returnsIndex;
    double sum;
    std::vector<double> currentReturns;
    sum = 0;
    currentReturns = this->returns[stateActionPair];
    for (returnsIndex = 0; returnsIndex < (int)currentReturns.size(); returnsIndex++) {
        sum += currentReturns[returnsIndex];
    }
    if (currentReturns.empty()) {
        return 0.0;
    } else {
        return (sum / (double)currentReturns.size());
    }
}

/**
 * Updates the policy of the supplied state, given its now-greedy action.
 *
 * @param s The state to start from.
 * @param greedyAction The greedy action in this state.
 */
void MonteCarloFirstVisitPlayer::updateStatePolicy(State *s, Maze::Actions greedyAction) {
    int actionIndex;
    Maze::Actions currentAction;
    double newActionProbability;
    std::vector<double> newPolicy;
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        currentAction = Maze::actionFromIndex(actionIndex);
        if (currentAction == greedyAction) {
            newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER) + 1.0 - this->epsilon;
        } else {
            newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER);
        }
        newPolicy.push_back(newActionProbability);
    }
    this->policy[s] = newPolicy;
}

/**
 * Performs the initialisation step of the Monte Carlo first visit player.
 *
 * Note that this method is trivial, although kept for compatibility reasons.
 */
void MonteCarloFirstVisitPlayer::performInitialisation() {}

/**
 * Performs the iteration step of the Monte Carlo first visit player.
 */
void MonteCarloFirstVisitPlayer::performIteration() {
    int index;
    double currentReturn;
    std::tuple<State*, Maze::Actions> stateActionPair;
    Maze::Actions greedyAction;
    stateActionPair = this->initialStateActionPair();
    this->generateEpisode(stateActionPair);
    for (index = 0; index < (int)this->episode.size(); index++) {
        stateActionPair = this->episode[index];
        currentReturn = this->episodeReturn(index + 1);
        this->returns[stateActionPair].push_back(currentReturn);
        this->stateActionValues[stateActionPair] = this->returnsAverage(stateActionPair);
    }
    for (index = 0; index < (int)this->episode.size(); index++) {
        stateActionPair = this->episode[index];
        greedyAction = this->greedyAction(std::get<0>(stateActionPair));
        this->updateStatePolicy(std::get<0>(stateActionPair), greedyAction);
    }
    this->addRewardsToTotalRewardPerEpisode();
    this->currentEpoch++;
    this->episode.clear();
    this->rewards.clear();
}

/**
 * Solves the maze the player was assigned to address.
 */
void MonteCarloFirstVisitPlayer::solveMaze() {
    this->performInitialisation();
    do {
        auto startTime = std::chrono::high_resolution_clock::now();
        this->performIteration();
        auto endTime = std::chrono::high_resolution_clock::now();
        this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
    } while (!this->maximumIterationReached());
}