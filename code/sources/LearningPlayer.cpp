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

void LearningPlayer::printStateActionValues(State* s) {
    int actionIndex;
    std::string actionAsString;
    Maze::Actions action;
    std::tuple<State*, Maze::Actions> stateActionPair;
    double value;
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        action = Maze::actionFromIndex(actionIndex);
        actionAsString = Maze::actionAsString(action);
        stateActionPair = std::make_tuple(s, action);
        value = this->stateActionValues[stateActionPair];
        printf("\t\t%s has value %.3lf.\n", actionAsString.c_str(), value);
    }
}

void LearningPlayer::printFinalPolicy() {
    int stateIndex;
    State *s;
    std::string policyAction;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        s = this->maze->getState(stateIndex);
        printf("\tState (%d, %d) ", s->getX(), s->getY());
        if (Maze::stateIsTerminal(s)) {
            printf("is terminal.\n");
        } else if (Maze::stateIsIntraversible(s)) {
            printf("is intraversible.\n");
        } else {
            policyAction = Maze::actionAsString(this->chooseAction(s));
            printf("has action %s.\n", policyAction.c_str());
            this->printStateActionValues(s);
        }
    }
}