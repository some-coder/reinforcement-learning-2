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
    for (i = 0; i < states->size(); i++) {
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

Maze::Actions Player::chooseAction(State *s) {
    int i;
    double bar, current;
    bar = RandomServices::continuousUniformSample(1.0);
    current = this->actionProbability(s, Maze::actionFromIndex(0));
//    printf("  Bar is %.3lf.\n", bar);
    for (i = 0; i < Maze::ACTION_NUMBER; i++) {
        if (bar <= current) {
            return Maze::actionFromIndex(i);
        }
        current += this->actionProbability(s, Maze::actionFromIndex(i + 1));
//        printf("  Considering action %d; current = %.2lf.\n", i + 1, current);
    }
    return Maze::actionFromIndex(Maze::ACTION_NUMBER - 1);
}

std::map<State*, std::vector<double>> Player::getPolicy() {
    return this->policy;
}

void Player::printFinalPolicy() {
    int i;
    State *s;
    std::cout << "States:" << std::endl;
    for (i = 0; i < this->maze->getStates()->size(); i++) {
        s = this->maze->getState(i);
        std::cout << "(" << s->getX() << ", " << s->getY() << "), ";
        std::cout << "action is " << this->chooseAction(s) << ", ";
        std::cout << "utility is " << this->stateValues[s] << "." << std::endl;
    }
}