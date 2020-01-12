#include <RandomServices.hpp>
#include "MonteCarloExploringStartsPlayer.hpp"

void MonteCarloExploringStartsPlayer::performInitialisation() {}

State *MonteCarloExploringStartsPlayer::randomState() {
    State *s;
    do {
        s = this->maze->getState(RandomServices::discreteUniformSample((int)this->stateValues.size() - 1));
    } while (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s));
    return s;
}

Maze::Actions MonteCarloExploringStartsPlayer::randomAction() {
    return Maze::actionFromIndex(RandomServices::discreteUniformSample(Maze::ACTION_NUMBER - 1));
}

std::tuple<State*, Maze::Actions> MonteCarloExploringStartsPlayer::randomStateActionPair() {
    /* Todo: Should terminal states be included in the state selection? */
    State *s;
    Maze::Actions a;
    do {
        s = this->randomState();
    } while (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s));
    a = MonteCarloExploringStartsPlayer::randomAction();
    return std::make_tuple(s, a);
}

std::tuple<State*, Maze::Actions> MonteCarloExploringStartsPlayer::initialStateActionPair() {
    return this->randomStateActionPair();
}

std::tuple<State*, Maze::Actions> MonteCarloExploringStartsPlayer::nextStateActionPair(
        std::tuple<State*, Maze::Actions> currentPair) {
    /* Todo: Check whether special state inclusion causes problems with state valuation and progress. */
    std::tuple<State*, double> result;
    result = this->maze->getStateTransitionResult(std::get<0>(currentPair), std::get<1>(currentPair));
    this->rewards.push_back(std::get<1>(result));
    return std::make_tuple(std::get<0>(result), this->chooseAction(std::get<0>(result)));
}

void MonteCarloExploringStartsPlayer::generateEpisode(std::tuple<State *, Maze::Actions> startStateActionPair) {
    int episodeIteration, episodeTimeout;
    std::tuple<State*, Maze::Actions> currentStateActionPair;
    episodeIteration = -1;
    episodeTimeout = std::ceil(EPISODE_TIMEOUT_FRACTION * (double)this->stateValues.size());
    currentStateActionPair = startStateActionPair;
    this->episode.push_back(currentStateActionPair);
    this->rewards.push_back(0.0);  /* At the first moment, no rewards are obtained yet. */
    do {
        episodeIteration++;
        currentStateActionPair = this->nextStateActionPair(currentStateActionPair);
        this->episode.push_back(currentStateActionPair);
    } while (episodeIteration < episodeTimeout && !Maze::stateIsTerminal(std::get<0>(currentStateActionPair)));
    this->maze->resetMaze();
}

double MonteCarloExploringStartsPlayer::episodeReturnComponent(int k, int rewardIndex) {
    return std::pow(this->discountFactor, k) * this->rewards[rewardIndex];
}

double MonteCarloExploringStartsPlayer::episodeReturn(int onsetIndex) {
    int i;
    double episodeReturn;
    episodeReturn = 0.0;
    for (i = onsetIndex; i < (int)this->rewards.size(); i++) {
        episodeReturn += this->episodeReturnComponent(i - onsetIndex, i);
    }
    return episodeReturn;
}

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

MonteCarloExploringStartsPlayer::MonteCarloExploringStartsPlayer(Maze *m, double gamma, int T) :
        MonteCarloPlayer(m, gamma, T) {}

MonteCarloExploringStartsPlayer::~MonteCarloExploringStartsPlayer() = default;

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
    this->currentEpoch++;
    this->episode.clear();
    this->rewards.clear();
}

void MonteCarloExploringStartsPlayer::solveMaze() {
    this->performInitialisation();
    do {
        auto startTime = std::chrono::high_resolution_clock::now();
        this->performIteration();
        auto endTime = std::chrono::high_resolution_clock::now();
        this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
    } while (!this->maximumIterationReached());
}