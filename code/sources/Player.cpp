#include "Player.hpp"

Player::Player() = default;

Player::~Player() = default;

double Player::getStateValue(class State s) {
    return this->stateValues[s];
}

void Player::setStateValue(class State s, double newValue) {
    this->stateValues[s] = newValue;
}

double Player::getStateActionValue(State s, State::direction d) {
    return this->stateActionValues[std::make_tuple(s, d)];
}

void Player::setStateActionValue(State s, State::direction d, double newValue) {
    this->stateActionValues[std::make_tuple(s, d)] = newValue;
}