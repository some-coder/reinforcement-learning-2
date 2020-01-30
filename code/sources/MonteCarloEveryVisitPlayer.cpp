#include "MonteCarloEveryVisitPlayer.hpp"
#include "LearningPlayer.hpp"

/**
 * Constructs a Monte Carlo every-visit player.
 *
 * @param m The maze to be solved by the player.
 * @param gamma The discount factor to apply to earlier-obtained rewards.
 * @param T The maximum iteration before forcing a stop.
 */
MonteCarloEveryVisitPlayer::MonteCarloEveryVisitPlayer(Maze *m, double gamma, int T) : MonteCarloPlayer(m, gamma, T) {
    this->temperature = STARTING_TEMPERATURE;
    this->G = this->W = 0.0;
}

/**
 * Destructs the Monte Carlo every-visit player.
 */
MonteCarloEveryVisitPlayer::~MonteCarloEveryVisitPlayer() = default;

/**
 * Initalises the C (count) parameter of the algorithm.
 */
void MonteCarloEveryVisitPlayer::initialiseC() {
    int stateIndex, actionIndex;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
            this->C[std::make_tuple(this->maze->getState(stateIndex), Maze::actionFromIndex(actionIndex))] = 0.0;
        }
    }
}

/**
 * Constructs an exploration policy for the supplied state.
 *
 * This policy is used to actually traverse the maze. It is not the policy that
 * will be optimised across time.
 *
 * @return The exploration policy for this state.
 */
std::vector<double> MonteCarloEveryVisitPlayer::stateExplorationPolicy() {
    int actionIndex;
    std::vector<double> statePolicy;
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        statePolicy.push_back(1.0 / Maze::ACTION_NUMBER);
    }
    return statePolicy;
}

/**
 * Initialises the exploration policy across the states.
 */
void MonteCarloEveryVisitPlayer::initialiseExplorationPolicy() {
    int stateIndex;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        this->explorationPolicy[this->maze->getState(stateIndex)] = MonteCarloEveryVisitPlayer::stateExplorationPolicy();
    }
}

/**
 * Obtains the action probability of doing the given action in this state.
 *
 * @param s The state in which the action is performed.
 * @param a The action to perform.
 * @return The performance probability.
 */
double MonteCarloEveryVisitPlayer::actionProbability(State *s, Maze::Actions a) {
    return this->explorationPolicy[s][a];
}

/**
 * Obtains the first state-action pair for an episode generation.
 *
 * @return The initial state-action pair.
 */
std::tuple<State*, Maze::Actions> MonteCarloEveryVisitPlayer::initialStateActionPair() {
    State *startingState;
    startingState = this->maze->getStartingState();
    return std::make_tuple(startingState, this->chooseAction(startingState));
}

/**
 * Updates the exploration policy.
 *
 * Although the exploration policy is not optimised over execution, we do alter
 * it so as to more effectively explore the maze. Specifically, we lower the
 * rate of exploration to reduce the amount of updates to non-essential tiles.
 * We do so using a 'temperature' that gets cooled across time, not unlike
 * algorithms like simulated annealing.
 *
 * @param s The state for which to update the exploration policy.
 * @param greedyAction The greedy action for this state.
 */
void MonteCarloEveryVisitPlayer::updateExplorationPolicy(State *s, Maze::Actions greedyAction) {
    int actionIndex;
    double newActionProbability;
    Maze::Actions currentAction;
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        currentAction = Maze::actionFromIndex(actionIndex);
        if (currentAction == greedyAction) {
            newActionProbability = (this->temperature / Maze::ACTION_NUMBER) + 1.0 - this->temperature;
        } else {
            newActionProbability = (this->temperature / Maze::ACTION_NUMBER);
        }
        this->explorationPolicy[s][currentAction] = newActionProbability;
    }
}

/**
 * Obtains the succeeding state-action pair, given a current tuple.
 *
 * @param currentPair The current state-action pair.
 * @return The successor tuple.
 */
std::tuple<State*, Maze::Actions> MonteCarloEveryVisitPlayer::nextStateActionPair(
        std::tuple<State *, Maze::Actions> currentPair) {
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
void MonteCarloEveryVisitPlayer::generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) {
    int currentIteration, maximumIteration;
    std::tuple<State*, Maze::Actions> stateActionPair;
    currentIteration = -1;
    maximumIteration = std::ceil(EPISODE_TIMEOUT_FRACTION * (double)this->stateValues.size());
    stateActionPair = this->initialStateActionPair();
    this->episode.push_back(stateActionPair);
    this->rewards.push_back(0.0);   /* First iteration has no reward. */
    do {
        currentIteration++;
        stateActionPair = this->nextStateActionPair(stateActionPair);
        this->episode.push_back(stateActionPair);
    } while (currentIteration < maximumIteration && !Maze::stateIsTerminal(std::get<0>(stateActionPair)));
    this->maze->resetMaze();
}

/**
 * Performs the initialisation step of the Monte Carlo every-visit player.
 */
void MonteCarloEveryVisitPlayer::performInitialisation() {
    this->initialiseC();
    this->initialiseExplorationPolicy();
}

/**
 * Performs an iteration step of the Monte Carlo every-visit player.
 */
void MonteCarloEveryVisitPlayer::performIteration() {
    int iteration;
    std::tuple<State*, Maze::Actions> stateActionPair;
    State *state;
    stateActionPair = this->initialStateActionPair();
    this->generateEpisode(stateActionPair);
    this->G = 0.0;
    this->W = 1.0;
    for (iteration = (int)this->episode.size() - 2; iteration >= 0; iteration--) {
        stateActionPair = this->episode[iteration];
        this->G = this->discountFactor * this->G + this->rewards[iteration + 1];
        this->C[stateActionPair] += this->W;
        this->stateActionValues[stateActionPair] += (this->W / this->C[stateActionPair]) *
                (this->G - this->stateActionValues[stateActionPair]);
        state = std::get<0>(stateActionPair);
        this->policy[state] = Player::actionAsActionProbabilityDistribution(this->greedyAction(state));
        this->updateExplorationPolicy(state, this->greedyAction(state));
        this->W *= this->explorationPolicy[state][std::get<1>(stateActionPair)];
        if (this->W == 0.0) {
            break;
        }
    }
    this->addRewardsToTotalRewardPerEpisode();
    this->currentEpoch++;
    this->episode.clear();
    this->rewards.clear();
}

/**
 * Solves the maze the player was assigned to address.
 */
void MonteCarloEveryVisitPlayer::solveMaze() {
    this->performInitialisation();
    do {
        auto startTime = std::chrono::high_resolution_clock::now();
        this->performIteration();
        this->temperature = STARTING_TEMPERATURE - (STARTING_TEMPERATURE - ENDING_TEMPERATURE) *
                ((this->currentEpoch) / (this->timeoutEpoch - 1.0));
        auto endTime = std::chrono::high_resolution_clock::now();
        this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
    } while (!this->maximumIterationReached());
}
