#include <RandomServices.hpp>
#include "DynamicProgrammingPlayer.hpp"

/**
 * Constructs a dynamic programming player.
 *
 * @param m The maze the player needs to solve.
 * @param gamma The discount factor for earlier-obtained rewards.
 * @param theta The minimal utility difference to keep iterating.
 */
DynamicProgrammingPlayer::DynamicProgrammingPlayer(Maze *m, double gamma, double theta) : Player(m, gamma) {
    this->maze = m;
    this->discountFactor = gamma;
    this->theta = theta;
    this->copyStateValues(&(this->stateValues), &(this->oldStateValues));
}

/**
 * Destructs the dynamic programming player.
 */
DynamicProgrammingPlayer::~DynamicProgrammingPlayer() = default;

/**
 * Copies the values of the states to a new, target state-value map.
 *
 * @param source The state-value mapping from which to copy.
 * @param target The state-value mapping to copy to.
 */
void DynamicProgrammingPlayer::copyStateValues(std::map<State*, double> *source, std::map<State*, double> *target) {
    int i;
    double stateValue;
    State *s;
    for (i = 0; i < (int)this->maze->getStates()->size(); i++) {
        s = this->maze->getState(i);
        stateValue = source->at(s);
        (*target)[s] = stateValue;
    }
}

/**
 * Obtains an arbitrary nontrivial state from the player's set of states.
 *
 * A state is regarded as being 'nontrivial' if it is neither terminal nor
 * intraversible.
 *
 * @return A random nontrivial state.
 */
State* DynamicProgrammingPlayer::randomNontrivialState() {
    State *s;
    int randomIndex;
    do {
        randomIndex = RandomServices::discreteUniformSample((int)this->maze->getStates()->size() - 1);
        s = &(this->maze->getStates()->at(randomIndex));
    } while (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s));
    return s;
}