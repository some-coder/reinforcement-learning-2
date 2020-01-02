#include "LearningPlayer.hpp"

LearningPlayer::LearningPlayer(Maze *m, double gamma, int T) : Player(m, gamma) {
    this->timeoutEpoch = T;
    this->currentEpoch = 0;
    this->initialiseStateActionValues();
}

void LearningPlayer::setStateActionValue(State *s, Maze::Actions action, double value) {
    this->stateActionValues[std::make_tuple(s, action)] = value;
}

void LearningPlayer::initialiseStateActionValues() {
    int i;
    State *state;
    std::vector<State>* states;
    states = this->maze->getStates();
    for (i = 0; i < (int)states->size(); i++) {
        state = &(states->at(i));
        this->setStateActionValue(state, Maze::Actions::moveUp, INITIAL_STATE_ACTION_VALUE);
        this->setStateActionValue(state, Maze::Actions::moveRight, INITIAL_STATE_ACTION_VALUE);
        this->setStateActionValue(state, Maze::Actions::moveDown, INITIAL_STATE_ACTION_VALUE);
        this->setStateActionValue(state, Maze::Actions::moveLeft, INITIAL_STATE_ACTION_VALUE);
    }
}

LearningPlayer::~LearningPlayer() = default;