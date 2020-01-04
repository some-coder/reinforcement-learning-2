#include "MonteCarloFirstVisitPlayer.hpp"

MonteCarloFirstVisitPlayer::MonteCarloFirstVisitPlayer(Maze *m, double gamma, int T, double epsilon) :
        MonteCarloPlayer(m, gamma, T, true) {
    /* Todo: Assumes epsilon-soft policy, but policy is completely random. Okay? */
    this->epsilon = epsilon;
}

MonteCarloFirstVisitPlayer::~MonteCarloFirstVisitPlayer() = default;

std::tuple<State*, Maze::Actions> MonteCarloFirstVisitPlayer::initialStateActionPair() {
    State *s;
    Maze::Actions a;
    s = this->maze->getStartingState();
    a = this->chooseAction(s);
//    printf("\t\tAction chosen: %s.\n", Maze::actionAsString(a).c_str());
    return std::make_tuple(s, a);
}

std::tuple<State*, Maze::Actions> MonteCarloFirstVisitPlayer::nextStateActionPair(
        std::tuple<State*, Maze::Actions> currentPair) {
    std::tuple<State*, double> result;
    result = this->maze->getStateTransitionResult(std::get<0>(currentPair), std::get<1>(currentPair));
    this->rewards.push_back(std::get<1>(result));
    return std::make_tuple(std::get<0>(result), this->chooseAction(std::get<0>(result)));
}

void MonteCarloFirstVisitPlayer::generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) {
    int currentIteration, maximumIteration;
    std::tuple<State*, Maze::Actions> stateActionPair;
    currentIteration = -1;
    maximumIteration = std::ceil(EPISODE_TIMEOUT_FRACTION * (double)this->stateValues.size());
    stateActionPair = this->initialStateActionPair();
    this->episode.push_back(stateActionPair);
    this->rewards.push_back(0.0);  /* First iteration has no reward. */
    do {
        currentIteration++;
        stateActionPair = this->nextStateActionPair(stateActionPair);
        this->episode.push_back(stateActionPair);
    } while (currentIteration < maximumIteration && !Maze::stateIsTerminal(std::get<0>(stateActionPair)));
}

double MonteCarloFirstVisitPlayer::episodeReturnComponent(int k, int rewardIndex) {
    return std::pow(this->discountFactor, k) * this->rewards[rewardIndex];
}

double MonteCarloFirstVisitPlayer::episodeReturn(int onsetIndex) {
    int index;
    double episodeReturn;
    episodeReturn = 0.0;
    for (index = onsetIndex; index < (int)this->rewards.size(); index++) {
        episodeReturn += this->episodeReturnComponent(index - onsetIndex, index);
    }
    return episodeReturn;
}

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

void MonteCarloFirstVisitPlayer::performInitialisation() {}

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
//    printf("\n");
    this->currentEpoch++;
    this->episode.clear();
    this->rewards.clear();
}

void MonteCarloFirstVisitPlayer::solveMaze() {
    this->performInitialisation();
    do {
        this->performIteration();
    } while (!this->maximumIterationReached());
}