#include "State.hpp"

/**
 * Constructs a state.
 *
 * @param x The state's horizontal coordinate.
 * @param y The state's vertical coordinate.
 * @param type The tile type of the state.
 */
State::State(int x, int y, State::Types type) {
    this->x = x;
    this->y = y;
    this->type = type;
    this->activated = false;
}

/**
 * Obtains the horizontal coordinate of the state.
 *
 * @return The state's horizontal coordinate.
 */
int State::getX() {
    return this->x;
}

/**
 * Obtains the vertical coordinate of the state.
 *
 * @return The state's vertical coordinate.
 */
int State::getY() {
    return this->y;
}

/**
 * Obtains the tile type of the state.
 *
 * For dynamic tiles, activation changes the tile type. Specifically, gates,
 * snacks and levers change upon activation: they become ordinary paths.
 *
 * @return The state's tile type.
 */
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

/**
 * Configures whether the state is 'activated'.
 *
 * An activated state changes the tile type of some states. This behaviour
 * reflects the dynamic nature of such states.
 *
 * @param activation Should the tile be activated?
 */
void State::setActivation(bool activation) {
    this->activated = activation;
}