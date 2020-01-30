#include <RandomServices.hpp>
#include "MonteCarloExploringStartsPlayer.hpp"
#include "LearningPlayer.hpp"

/**
 * Trivially initialises the Monte Carlo exploring starts player.
 */
void MonteCarloExploringStartsPlayer::performInitialisation() {}

/**
 * Obtains a random non-trivial state of the player's maze's states.
 *
 * Recall our definition of non-triviality: A state is non-trivial if it is
 * neither a terminal state, nor an intraversible one.
 *
 * @return An arbitrary non-trivial state.
 */
State *MonteCarloExploringStartsPlayer::randomState() {
    State *s;
    do {
        s = this->maze->getState(RandomServices::discreteUniformSample((int)this->stateValues.size() - 1));
    } while (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s));
    return s;
}

/**
 * Obtains an arbitrary action.
 *
 * @return The random action.
 */
Maze::Actions MonteCarloExploringStartsPlayer::randomAction() {
    return Maze::actionFromIndex(RandomServices::discreteUniformSample(Maze::ACTION_NUMBER - 1));
}

/**
 * Builds and returns a random state-action pair.
 *
 * Note that only non-trivial states are used in exploring starts. This is
 * because we want our agent to learn as much as possible, and not simply
 * 'waste' some of its iterations simply succeeding or failing instantly.
 *
 * @return The arbitrary state-action tuple.
 */
std::tuple<State*, Maze::Actions> MonteCarloExploringStartsPlayer::randomStateActionPair() {
    State *s;
    Maze::Actions a;
    do {
        s = this->randomState();
    } while (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s));
    a = MonteCarloExploringStartsPlayer::randomAction();
    return std::make_tuple(s, a);
}

/**
 * Obtains the initial state-action pair to start an episode with.
 *
 * Although admittedly trivially implemented, this method is made purely to
 * adhere to a standard dictated in the parent class; such are the consequences
 * of building a project within an OOP language.
 *
 * @return The starting state-action pair.
 */
std::tuple<State*, Maze::Actions> MonteCarloExploringStartsPlayer::initialStateActionPair() {
    return this->randomStateActionPair();
}

/**
 * Obtains the next state-action pair after executing the current pair.
 *
 * @param currentPair The current state-action pair to advance from.
 * @return The succeeding tuple of state and action.
 */
std::tuple<State*, Maze::Actions> MonteCarloExploringStartsPlayer::nextStateActionPair(
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
void MonteCarloExploringStartsPlayer::generateEpisode(std::tuple<State *, Maze::Actions> startStateActionPair) {
    int episodeIteration, episodeTimeout;
    std::tuple<State*, Maze::Actions> currentStateActionPair;
    episodeIteration = -1;
    episodeTimeout = std::ceil(EPISODE_TIMEOUT_FRACTION * (double)this->stateValues.size());
    currentStateActionPair = startStateActionPair;
    this->episode.push_back(currentStateActionPair);
    this->rewards.push_back(0.0);  /* At the onset, no rewards are obtained yet. */
    do {
        episodeIteration++;
        currentStateActionPair = this->nextStateActionPair(currentStateActionPair);
        this->episode.push_back(currentStateActionPair);
    } while (episodeIteration < episodeTimeout && !Maze::stateIsTerminal(std::get<0>(currentStateActionPair)));
    this->maze->resetMaze();
}

/**
 * Computes a single component of the episode's returns.
 *
 * @param k The exponent to raise the gamma to.
 * @param rewardIndex The index of the current reward component.
 * @return The value of the component of the episode's returns.
 */
double MonteCarloExploringStartsPlayer::episodeReturnComponent(int k, int rewardIndex) {
    return std::pow(this->discountFactor, k) * this->rewards[rewardIndex];
}

/**
 * Computes the current episode's total returns.
 *
 * @param onsetIndex The index from which to start.
 * @return The episode's total returns.
 */
double MonteCarloExploringStartsPlayer::episodeReturn(int onsetIndex) {
    int i;
    double episodeReturn;
    episodeReturn = 0.0;
    for (i = onsetIndex; i < (int)this->rewards.size(); i++) {
        episodeReturn += this->episodeReturnComponent(i - onsetIndex, i);
    }
    return episodeReturn;
}

/**
 * Computes the average of the current episode's returns.
 *
 * @param stateActionPair THe state-action pair of which to get the mean return.
 * @return The state-action pair's average returns over episodes.
 */
double MonteCarloExploringStartsPlayer::returnsAverage(std::tuple<State*, Maze::Actions> stateActionPair) {
    int returnsIndex;
    double average;
    std::vector<double> stateActionReturns;
    average = 0.0;
    stateActionReturns = this->returns[stateActionPair];
    for (returnsIndex = 0; returnsIndex < (int)stateActionReturns.size(); returnsIndex++) {
        average += stateActionReturns[returnsIndex];
    }
    if (stateActionReturns.empty()) {
        return 0.0;
    } else {
        return (average / (double)stateActionReturns.size());
    }
}

/**
 * Constructs a Monte Carlo exploring starts player.
 *
 * @param m The maze to be solved by the player.
 * @param gamma The discount factor to apply to earlier-obtained rewards.
 * @param T The minimal utility difference to decide to keep iterating.
 */
MonteCarloExploringStartsPlayer::MonteCarloExploringStartsPlayer(Maze *m, double gamma, int T) :
        MonteCarloPlayer(m, gamma, T) {}

/**
 * Destructs the Monte Carlo exploring starts player.
 */
MonteCarloExploringStartsPlayer::~MonteCarloExploringStartsPlayer() = default;

/**
 * Performs an iteration of the Monte Carlo exploring starts algorithm.
 */
void MonteCarloExploringStartsPlayer::performIteration() {
    int episodeIteration;
    std::tuple<State*, Maze::Actions> stateActionPair;
    double episodeReturn;
    Maze::Actions greedyAction;
    this->generateEpisode(this->initialStateActionPair());
    for (episodeIteration = 0; episodeIteration < (int)this->episode.size(); episodeIteration++) {
        stateActionPair = this->episode[episodeIteration];
        episodeReturn = this->episodeReturn(episodeIteration + 1);
        this->returns[stateActionPair].push_back(episodeReturn);
        this->stateActionValues[stateActionPair] = this->returnsAverage(stateActionPair);
    }
    for (episodeIteration = 0; episodeIteration < (int)this->episode.size(); episodeIteration++) {
        stateActionPair = this->episode[episodeIteration];
        greedyAction = this->greedyAction(std::get<0>(stateActionPair));
        this->policy[std::get<0>(stateActionPair)] = Player::actionAsActionProbabilityDistribution(greedyAction);
    }
    this->addRewardsToTotalRewardPerEpisode();
    this->currentEpoch++;
    this->episode.clear();
    this->rewards.clear();
}

/**
 * Solves the maze the player was assigned to address.
 */
void MonteCarloExploringStartsPlayer::solveMaze() {
    this->performInitialisation();
    do {
        auto startTime = std::chrono::high_resolution_clock::now();
        this->performIteration();
        auto endTime = std::chrono::high_resolution_clock::now();
        this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
    } while (!this->maximumIterationReached());
}