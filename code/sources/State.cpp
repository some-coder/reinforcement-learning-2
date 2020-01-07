#include "State.hpp"

State::State(int x, int y, State::Types type) {
    this->x = x;
    this->y = y;
    this->type = type;
}

int State::getX() {
    return this->x;
}

int State::getY() {
    return this->y;
}

State::Types State::getType() {
    return this->type;
}

void State::setType(Types t) {
    this->type = t;
}