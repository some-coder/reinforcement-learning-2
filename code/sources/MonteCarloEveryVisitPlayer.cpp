#include <RandomServices.hpp>
#include "MonteCarloEveryVisitPlayer.hpp"

MonteCarloEveryVisitPlayer::MonteCarloEveryVisitPlayer(Maze *m, double gamma, int T) : MonteCarloPlayer(m, gamma, T) {
    this->temperature = STARTING_TEMPERATURE;
    this->G = this->W = 0.0;
}

MonteCarloEveryVisitPlayer::~MonteCarloEveryVisitPlayer() = default;

void MonteCarloEveryVisitPlayer::initialiseC() {
    int stateIndex, actionIndex;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
            this->C[std::make_tuple(this->maze->getState(stateIndex), Maze::actionFromIndex(actionIndex))] = 0.0;
        }
    }
}

std::vector<double> MonteCarloEveryVisitPlayer::stateExplorationPolicy() {
    /* Todo: Is total randomness acceptable? */
    int actionIndex;
    std::vector<double> statePolicy;
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        statePolicy.push_back(1.0 / Maze::ACTION_NUMBER);
    }
    return statePolicy;
}

void MonteCarloEveryVisitPlayer::initialiseExplorationPolicy() {
    int stateIndex;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        this->explorationPolicy[this->maze->getState(stateIndex)] = MonteCarloEveryVisitPlayer::stateExplorationPolicy();
    }
}

double MonteCarloEveryVisitPlayer::actionProbability(State *s, Maze::Actions a) {
    return this->explorationPolicy[s][a];
}

std::tuple<State*, Maze::Actions> MonteCarloEveryVisitPlayer::initialStateActionPair() {
    State *startingState;
    startingState = this->maze->getStartingState();
    return std::make_tuple(startingState, this->chooseAction(startingState));
}

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

std::tuple<State*, Maze::Actions> MonteCarloEveryVisitPlayer::nextStateActionPair(
        std::tuple<State *, Maze::Actions> currentPair) {
    std::tuple<State*, double> result;
    result = this->maze->getStateTransitionResult(std::get<0>(currentPair), std::get<1>(currentPair));
    this->rewards.push_back(std::get<1>(result));
    return std::make_tuple(std::get<0>(result), this->chooseAction(std::get<0>(result)));
}

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
}

void MonteCarloEveryVisitPlayer::performInitialisation() {
    this->initialiseC();
    this->initialiseExplorationPolicy();
}

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
    this->currentEpoch++;
    this->episode.clear();
    this->rewards.clear();
}

void MonteCarloEveryVisitPlayer::solveMaze() {
    this->performInitialisation();
    do {
        this->performIteration();
        this->temperature = STARTING_TEMPERATURE - (STARTING_TEMPERATURE - ENDING_TEMPERATURE) *
                ((this->currentEpoch) / (this->timeoutEpoch - 1.0));
    } while (!this->maximumIterationReached());
}
