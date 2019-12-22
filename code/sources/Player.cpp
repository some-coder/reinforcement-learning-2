#include <RandomServices.hpp>
#include "Player.hpp"

Player::Player(Maze* m, double gamma) {
    this->maze = m;
    this->discountFactor = gamma;
    this->initialiseStateValues();
}

Player::~Player() = default;

void Player::initialiseStateValues() {
    int i;
    State *state;
    std::vector<State>* states;
    states = this->maze->getStates();
    for (i = 0; i < states->size(); i++) {
        state = &(states->at(i));
        this->stateValues[state] = INITIAL_STATE_VALUE;
    }
}

double Player::actionProbability(State *s, Maze::Actions a) {
    return this->policy[s][a];
}

Maze::Actions Player::chooseAction(State *s) {
    int i;
    double bar, current;
    bar = RandomServices::continuousUniformSample(Maze::ACTION_NUMBER - 1);
    current = this->actionProbability(s, Maze::actionFromIndex(0));
    for (i = 0; i < Maze::ACTION_NUMBER; i++) {
        if (bar <= current) {
            return Maze::actionFromIndex(i);
        }
        current += this->actionProbability(s, Maze::actionFromIndex(i + 1));
    }
    return Maze::actionFromIndex(Maze::ACTION_NUMBER - 1);
}

std::map<State*, std::vector<double>> Player::getPolicy() {
    return this->policy;
}