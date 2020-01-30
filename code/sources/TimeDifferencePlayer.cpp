#include "TimeDifferencePlayer.hpp"

/**
 * Constructs a time-difference player.
 *
 * @param m The maze for the player to solve.
 * @param gamma The discount factor to apply to earlier-obtained rewards.
 * @param T The timeout epoch.
 * @param alpha The learning rate.
 * @param epsilon The exploration rate.
 */
TimeDifferencePlayer::TimeDifferencePlayer(Maze *m, double gamma, int T, double alpha, double epsilon) :
        LearningPlayer(m, gamma, T) {
    this->alpha = alpha;
    this->epsilon = epsilon;
    this->initialiseQuality();
}

/**
 * Destructs the time-difference player.
 */
TimeDifferencePlayer::~TimeDifferencePlayer() = default;

/**
 * This function initializes the values of quality to zero. Quality is later used to update the
 * policy of a player.
 */
void TimeDifferencePlayer::initialiseQuality() {
    int i;
    State *state;
    std::vector<State>* states;
    states = this->maze->getStates();
    for (i = 0; i < (int)states->size(); i++) {
        state = &(states->at(i));
        this->setQuality(state, Maze::Actions::moveUp, INITIAL_QUALITY_VALUE);
        this->setQuality(state, Maze::Actions::moveRight, INITIAL_QUALITY_VALUE);
        this->setQuality(state, Maze::Actions::moveDown, INITIAL_QUALITY_VALUE);
        this->setQuality(state, Maze::Actions::moveLeft, INITIAL_QUALITY_VALUE);
    }
}

/**
 * This function sets a value in quality of a certain state-action pair to the given value.
 * 
 * @param s The state 
 * @param action The action
 * @param value The quality value for the given state-action pair
 */
void TimeDifferencePlayer::setQuality(State *s, Maze::Actions action, double value) {
    this->quality[std::make_tuple(s, action)] = value;
}
