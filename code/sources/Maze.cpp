#include <iostream>
#include "RandomServices.hpp"
#include "Maze.hpp"

State::Types Maze::typeFromInput(char input) {
    switch (input) {
        case '.':
            return State::Types::path;
        case '*':
            return State::Types::start;
        case '1':
            return State::Types::goal;
        case '0':
            return State::Types::warp;
        case ':':
            return State::Types::lever;
        case '|':
            return State::Types::gate;
        case '%':
            return State::Types::snack;
        case '#':
            return State::Types::pit;
        default:
            return State::Types::none;
    }
}

void Maze::getMazeDimensionsFromInput() {
    scanf("%d %d", &(this->width), &(this->height));
}

void Maze::getMazeStatesFromInput() {
    int x, y;
    char input;
    getchar();
    for (y = 0; y < this->height; y++) {
        for (x = 0; x < (this->width + 1); x++) {
            scanf("%c", &input);
            if (x != this->width) {
                this->states.emplace_back(x, y, this->typeFromInput(input));
            }
        }
    }
}

void Maze::getMazeSpecialStates() {
    int i;
    State *s = nullptr;
    for (i = 0; i < this->states.size(); i++) {
        s = &(this->states[i]);
        switch (s->getType()) {
            case State::Types::start:
                this->startingStates.push_back(s);
                break;
            case State::Types::gate:
                this->gateStates.push_back(s);
                break;
            case State::Types::goal:
                this->goalStates.push_back(s);
                break;
            default:
                break;
        }
    }
}

Maze::Maze() {
    /* Todo: Make new constructor in which move probabilities can be defined. */
    this->getMazeDimensionsFromInput();
    this->getMazeStatesFromInput();
    this->getMazeSpecialStates();
    this->moveProbabilities = std::make_tuple(0.8, 0.1, 0.0, 0.1);
}

bool Maze::shouldStartAtRandomPosition() {
    return this->startingStates.empty();
}

int Maze::indexFromCoordinates(int x, int y) {
    return y * this->width + x;
}

std::tuple<int, int> Maze::coordinatesFromIndex(int i) {
    return std::make_tuple(i % this->width, i / this->width);
}

std::vector<State>* Maze::getStates() {
    return &(this->states);
}

State* Maze::getState(int index) {
    return &(this->states[index]);
}

State* Maze::getStartingState() {
    int randomIndex;
    if (this->shouldStartAtRandomPosition()) {
        randomIndex = RandomServices::discreteUniformSample((int)this->states.size() - 1);
        return &(this->states[randomIndex]);
    } else {
        /* Todo: Does a uniform distribution work with one sample? */
        randomIndex = RandomServices::discreteUniformSample((int)this->startingStates.size() - 1);
        return this->startingStates[randomIndex];
    }
}

Maze::Actions Maze::actionFromIndex(int index) {
    switch (index) {
        case 0:
            return moveUp;
        case 1:
            return moveRight;
        case 2:
            return moveDown;
        default:
            return moveLeft;
    }
}

Maze::Actions Maze::actualAction(Maze::Actions chosenAction) {
    /* Todo: Function is incorrect; doesn't use chosenAction. */
    int a;
    double bar, current;
    bar = RandomServices::continuousUniformSample(1.0);
    current = std::get<0>(this->moveProbabilities);
    for (a = 0; a < ACTION_NUMBER; a++) {
        if (bar <= current) {
            return this->actionFromIndex(a);
        }
        /* Todo: May potentially crash when having number under- or overflow. */
        current += this->getActionProbability(this->actionFromIndex(a + 1));
    }
    return this->actionFromIndex(ACTION_NUMBER - 1);
}

bool Maze::moveIsOutOfBounds(int x, int y) {
    return (x < 0 || x >= this->width) || (y < 0 || y >= this->height);
}

bool Maze::stateIsIntraversible(State *s) {
    State::Types t;
    t = s->getType();
    return (t == State::Types::none) || (t == State::Types::gate);
}

bool Maze::stateIsTerminal(State *s) {
    State::Types t;
    t = s->getType();
    return (t == State::Types::goal) || (t == State::Types::pit);
}

bool Maze::moveShouldFail(int x, int y) {
    if (this->moveIsOutOfBounds(x, y)) {
        return true;
    } else {
        State *s = &(this->states[this->indexFromCoordinates(x, y)]);
        return this->stateIsIntraversible(s);
    }
}

State* Maze::getNextStateDeterministic(class State * state, enum Maze::Actions action) {
    int x, y, deltas[ACTION_NUMBER][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    x = state->getX();
    y = state->getY();
    switch (action) {
        case moveUp:
            x += deltas[0][0];
            y += deltas[0][1];
            break;
        case moveRight:
            x += deltas[1][0];
            y += deltas[1][1];
            break;
        case moveDown:
            x += + deltas[2][0];
            y += deltas[2][1];
            break;
        default:
            x += deltas[3][0];
            y += deltas[3][1];
            break;
    }
    if (this->moveShouldFail(x, y)) {
        /* Maneuver failed. Remain in the current state.  */
        return state;
    } else {
        return &(this->states[this->indexFromCoordinates(x, y)]);
    }
}

State* Maze::getNextState(State *s, Actions action) {
    return this->getNextStateDeterministic(s, this->actualAction(action));
}

/**
 * Obtains the probability of performing an action given some action.
 *
 * @param relativeIndex The rotation from the action under consideration.
 * @return The probability of performing the given action.
 */
double Maze::getActionProbability(int relativeIndex) {
    switch (relativeIndex) {
        case Rotations::Zero:
            return std::get<0>(this->moveProbabilities);
        case Rotations::Quarter:
            return std::get<1>(this->moveProbabilities);
        case Rotations::Half:
            return std::get<2>(this->moveProbabilities);
        default:
            return std::get<3>(this->moveProbabilities);
    }
}

State* Maze::getWarpStateResult(State *s) {
    int i, x, y, deltas[ACTION_NUMBER][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    State *randomGoal;
    randomGoal = this->goalStates[RandomServices::discreteUniformSample((int)this->goalStates.size() - 1)];
    for (i = 0; i < ACTION_NUMBER; i++) {
        x = randomGoal->getX() + deltas[i][0];
        y = randomGoal->getY() + deltas[i][1];
        if (!this->moveShouldFail(x, y)) {
            return &(this->states[this->indexFromCoordinates(x, y)]);
        }
    }
    /* Theoretically impossible: Warp fails, so act as if it's a normal tile. */
    return s;
}

void Maze::openGates() {
    State *s;
    while (!this->gateStates.empty()) {
        s = this->gateStates.back();
        s->setType(State::Types::path);
        this->gateStates.pop_back();
    }
}

void Maze::removeSnack(State *s) {
    s->setType(State::Types::path);
}

State* Maze::getSpecialStateResult(State *s) {
    switch (s->getType()) {
        case State::Types::pit:
        case State::Types::goal:
            return this->getStartingState();
        case State::Types::warp:
            return this->getWarpStateResult(s);
        case State::Types::lever:
            this->openGates();
            break;
        case State::Types::snack:
            this->removeSnack(s);
            break;
        default:
            return s;
    }
    return s;
}

double Maze::getReward(State *state) {
    /* Todo: Snack state is shared among all players? */
    switch (state->getType()) {
        case State::Types::goal:
            return GOAL_REWARD;
        case State::Types::snack:
            return POSITIVE_REWARD;
        case State::Types::pit:
            return NEGATIVE_REWARD;
        default:
            return NORMAL_REWARD;
    }
}

std::tuple<State*, double> Maze::getStateTransitionResult(State *s, Actions a) {
    State *newState = this->getNextState(s, a);
    double reward = this->getReward(newState);
    newState = this->getSpecialStateResult(newState);
    return std::make_tuple(newState, reward);
}

std::string Maze::actionAsString(enum Maze::Actions a) {
    switch (a) {
        case Actions::moveUp:
            return "UP";
        case Actions::moveRight:
            return "RIGHT";
        case Actions::moveDown:
            return "DOWN";
        default:
            return "LEFT";
    }
}
