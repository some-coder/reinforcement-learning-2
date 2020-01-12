#include "State.hpp"

State::State(int x, int y, State::Types type) {
    this->x = x;
    this->y = y;
    this->type = type;
    this->activated = false;
}

int State::getX() {
    return this->x;
}

int State::getY() {
    return this->y;
}

State::Types State::getType() {
    switch (this->type) {
        case Types::gate:
        case Types::snack:
        case Types::lever:
            return (this->activated ? Types::path : this->type);
        default:
            return this->type;
    }
}

void State::setType(Types t) {
    this->type = t;
}

void State::setActivation(bool activation) {
    this->activated = activation;
}