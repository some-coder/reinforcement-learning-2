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