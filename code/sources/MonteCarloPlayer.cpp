#include "MonteCarloPlayer.hpp"

MonteCarloPlayer::MonteCarloPlayer(Maze *m, double gamma, int T, bool initialiseStochastic) :
        LearningPlayer(m, gamma, T, initialiseStochastic) {
    this->currentEpoch = 0;
    this->timeoutEpoch = T;
}

MonteCarloPlayer::~MonteCarloPlayer() = default;

Maze::Actions MonteCarloPlayer::greedyAction(State *s) {
    int actionIndex;
    std::tuple<State*, Maze::Actions> stateActionPair;
    Maze::Actions currentAction, bestAction;
    double current, best;
    bestAction = Maze::actionFromIndex(0);
    stateActionPair = std::make_tuple(s, bestAction);
    best = this->stateActionValues[stateActionPair];
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