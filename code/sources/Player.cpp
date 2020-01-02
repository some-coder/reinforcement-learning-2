#include <RandomServices.hpp>
#include <iostream>
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
    for (i = 0; i < (int)states->size(); i++) {
        state = &(states->at(i));
        if (state->getType() == State::Types::goal ||
            state->getType() == State::Types::pit) {
            this->stateValues[state] = Maze::getReward(state);
        } else {
            this->stateValues[state] = INITIAL_STATE_VALUE;
        }
    }
}

double Player::actionProbability(State *s, Maze::Actions a) {
    return this->policy[s][a];
}

std::vector<double> Player::actionAsActionProbabilityDistribution(Maze::Actions a) {
    int actionIndex;
    std::vector<double> distribution;
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        distribution.push_back( (actionIndex == a ? 1.0 : 0.0) );
    }
    return distribution;
}

Maze::Actions Player::chooseAction(State *s) {
    int i;
    double bar, current;
    bar = RandomServices::continuousUniformSample(1.0);
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

void Player::printFinalPolicy() {
    int i;
    State *s;
    for (i = 0; i < (int)this->maze->getStates()->size(); i++) {
        s = this->maze->getState(i);
        printf("State (%d, %d) has action %s and state utility %.3lf.\n",
            s->getX(), s->getY(), Maze::actionAsString(this->chooseAction(s)).c_str(), this->stateValues[s]);
    }
}