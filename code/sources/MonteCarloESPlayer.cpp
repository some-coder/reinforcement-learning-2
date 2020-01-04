#include <RandomServices.hpp>
#include "MonteCarloESPlayer.hpp"

void MonteCarloExploringStartsPlayer::performInitialisation() {
    /* Todo: Returns vectors have been initialised for all <s, a> combinations? */
}

State *MonteCarloExploringStartsPlayer::randomState() {
    return this->maze->getState(RandomServices::discreteUniformSample((int)this->stateValues.size() - 1));
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

std::tuple<State*, Maze::Actions> MonteCarloExploringStartsPlayer::nextStateActionPair(
        std::tuple<State*, Maze::Actions> currentPair) {
    State *nextState;
    Maze::Actions nextAction;
    double reward;
    nextState = this->maze->getNextState(std::get<0>(currentPair), std::get<1>(currentPair));
    nextAction = this->chooseAction(nextState);
    reward = Maze::getReward(nextState);
    this->rewards.push_back(reward);
    return std::make_tuple(nextState, nextAction);
}

void MonteCarloExploringStartsPlayer::generateEpisode(std::tuple<State *, Maze::Actions> startStateActionPair) {
    /* Todo: We do not take into account special state effects. */
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

Maze::Actions MonteCarloExploringStartsPlayer::greedyAction(State *s) {
    int actionIndex;
    std::tuple<State*, Maze::Actions> stateActionPair;
    Maze::Actions currentAction, bestAction;
    double current, best;
    bestAction = Maze::actionFromIndex(0);
    stateActionPair = std::make_tuple(s, bestAction);
    best = this->stateActionValues.at(stateActionPair);
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

bool MonteCarloExploringStartsPlayer::maximumIterationReached() {
    return this->currentEpoch >= this->timeoutEpoch;
}

MonteCarloExploringStartsPlayer::MonteCarloExploringStartsPlayer(Maze *m, double gamma, int T) :
        MonteCarloPlayer(m, gamma, T) {}

MonteCarloExploringStartsPlayer::~MonteCarloExploringStartsPlayer() = default;

void MonteCarloExploringStartsPlayer::performIteration() {
    int episodeIteration;
    std::tuple<State*, Maze::Actions> stateActionPair;
    double episodeReturn;
    Maze::Actions greedyAction;
    this->generateEpisode(this->randomStateActionPair());
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
        this->performIteration();
    } while (!this->maximumIterationReached());
}