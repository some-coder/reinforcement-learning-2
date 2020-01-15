#include <iostream>
#include <fstream>
#include <sstream>
#include <Player.hpp>
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

void Maze::getMazeDimensionsFromFile(std::ifstream *inputStream) {
    int w, h;
    std::string line;
    std::getline(*inputStream, line);
    std::istringstream stream(line);
    if (!(stream >> w >> h)) {
        printf("[Maze] Error: Could not read width and height. Aborting.\n");
    } else {
        this->width = w;
        this->height = h;
    }
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

void Maze::getMazeStatesFromFile(std::ifstream *inputStream) {
    int x, y;
    char input;
    for (y = 0; y < this->height; y++) {
        for (x = 0; x < (this->width + 1); x++) {
            input = inputStream->get();
            if (x != this->width) {
                this->states.emplace_back(x, y, this->typeFromInput(input));
            }
        }
    }
}

void Maze::getMazeSpecialStates() {
    int i;
    State *s = nullptr;
    for (i = 0; i < (int)this->states.size(); i++) {
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

Maze::Maze(std::tuple<double, double, double, double> mps, const std::string& inputFile) : mazeIdentifier(inputFile) {
    printf("Creating maze '%s'.\n", this->mazeIdentifier.c_str());
    if (inputFile.empty()) {
        printf("Please input your self-built maze.\n");
        printf("  Syntax (Step 1): Enter width, height (two integers greater than zero).\n");
        printf("  Syntax (Step 2): Enter rows of maze, represented by characters, ending in ENTER.\n");
        printf("  Legal characters:\n");
        printf("    (.) Path, (*) Start, (1) Goal, (0) Warp, (:) Lever, (|) Gate, (%%) Snack, (#) Pit, ( ) Empty.\n");
        this->getMazeDimensionsFromInput();
        this->getMazeStatesFromInput();
    } else {
        std::ifstream inputStream(inputFile);
        this->getMazeDimensionsFromFile(&inputStream);
        this->getMazeStatesFromFile(&inputStream);
    }
    this->getMazeSpecialStates();
    this->moveProbabilities = mps;
}

Maze::Maze(const std::string& inputFile) : Maze(std::make_tuple(0.8, 0.1, 0.0, 0.1), std::move(inputFile)) {}

Maze::~Maze() {
    printf("Deleting maze '%s'.\n", this->mazeIdentifier.c_str());
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

void Maze::resetMaze() {
    int stateIndex;
    for (stateIndex = 0; stateIndex < (int)this->states.size(); stateIndex++) {
        this->states[stateIndex].setActivation(false);
    }
}

std::vector<State>* Maze::getStates() {
    return &(this->states);
}

State* Maze::getState(int index) {
    return &(this->states[index]);
}

State* Maze::getStartingState() {
    int randomIndex;
    State *s;
    if (this->shouldStartAtRandomPosition()) {
        printf("Got random position!\n");
        do {
            randomIndex = RandomServices::discreteUniformSample((int)this->states.size() - 1);
            s = &(this->states[randomIndex]);
        } while (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s));
    } else {
        printf("Got nonrandom position!\n");
        /* Works 'till here (X, Y, Type). */
        /* Todo: Does a uniform distribution work with one sample? */
        do {
            randomIndex = RandomServices::discreteUniformSample((int)this->startingStates.size() - 1);
            s = this->startingStates[randomIndex];
        } while (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s));
    }
    printf("(%d, %d): %c.\n", this->getState(14)->getX(), this->getState(14)->getY(),
           Player::symbolToCharacter(this->getState(14)->getType()));
    printf("(%d, %d): %c.\n", s->getX(), s->getY(),
           Player::symbolToCharacter(s->getType()));
    printf("(%d, %d): %c.\n", this->startingStates[0]->getX(), this->startingStates[0]->getY(),
            Player::symbolToCharacter(this->startingStates[0]->getType()));
    return s;
}

/**
 * Returns the action according to the provided index. The index corresponds with an action in the Actions enum.
 * 
 * @return an action
 */
Maze::Actions Maze::actionFromIndex(int index) {
    /* Todo: This cast works successfully? */
    return (Actions)index;
}

Maze::Actions Maze::actualAction(Maze::Actions chosenAction) {
    int a;
    double bar, current;
    bar = RandomServices::continuousUniformSample(1.0);
    current = this->getActionProbability(0);
    for (a = 0; a < ACTION_NUMBER; a++) {
        if (bar <= current) {
            return this->actionFromIndex((chosenAction + a) % ACTION_NUMBER);
        }
        /* Todo: May potentially crash when having number under- or overflow. */
        current += this->getActionProbability(a + 1);
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
            x += deltas[2][0];
            y += deltas[2][1];
            break;
        case moveLeft:
            x += deltas[3][0];
            y += deltas[3][1];
            break;
        default:
            break;
    }
//    printf("\t\tIn (%d, %d) and should move %s.\n", state->getX(), state->getY(), actionAsString(action).c_str());
    if (this->moveShouldFail(x, y)) {
        /* Maneuver failed. Remain in the current state.  */
//        printf("\t\tAction failed. Remaining in current state.\n");
        return state;
    } else {
//        printf("\t\tSuccessfully moved.\n");
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
    int gateStateIndex;
    State *s;
    for (gateStateIndex = 0; gateStateIndex < (int)this->gateStates.size(); gateStateIndex++) {
        s = this->gateStates[gateStateIndex];
        s->setActivation(true);
    }
}

void Maze::removeSnack(State *s) {
    s->setActivation(true);
}

State* Maze::getSpecialStateResult(State *s) {
    State *newState;
    switch (s->getType()) {
        case State::Types::warp:
            newState = this->getWarpStateResult(s);
            return newState;
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

std::string Maze::getMazeIdentifier() {
    return this->mazeIdentifier;
}

int Maze::getMazeWidth() {
    return this->width;
}

int Maze::getMazeHeight() {
    return this->height;
}
