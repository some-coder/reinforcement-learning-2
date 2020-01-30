#include <iostream>
#include <fstream>
#include <sstream>
#include "RandomServices.hpp"
#include "Maze.hpp"

/**
 * Associates an input character to a state's tile type.
 *
 * @param input The input character to associate a tile type to.
 * @return The tile type.
 */
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

/**
 * Sets this maze's dimensions by reading from input.
 *
 * Two arguments are expected. The first is width, the second height.
 */
void Maze::getMazeDimensionsFromInput() {
    scanf("%d %d", &(this->width), &(this->height));
}

/**
 * Sets this maze's dimensions by reading from a plain text file.
 *
 * Two arguments are expected. The first is width, the second height.
 *
 * @param inputStream The input stream for the text file.
 */
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

/**
 * Sets this maze's states by reading from input.
 *
 * States are represented by characters. For an overview of all valid
 * characters, we recommend consulting this source file's header. Spaces entail
 * no state. Newlines start new rows of the maze.
 *   This method assumes the dimensions of the maze are already configured.
 */
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

/**
 * Sets this maze's states by reading from a plain text file.
 *
 * States are represented by characters. For an overview of all valid
 * characters, we recommend consulting this source file's header. Spaces entail
 * no state. Newlines start new rows of the maze.
 *   This method assumes the dimensions of the maze are already configured.
 *
 * @param inputStream The input stream for the text file.
 */
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

/**
 * Among all this maze's states, stores those states that are special.
 *
 * A state is deemed special if it is a starting tile, a gate, or a goal.
 */
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

/**
 * Constructs a maze.
 *
 * This is the full-fledged constructor for mazes.
 *   Mazes are either constructed from input or from a plain text file. By
 * supplying an empty relative path string, the maze is construed from input;
 * otherwise it is built from the given plain text file.
 *   Four relative movement probabilities must be supplied, which are required
 * to sum to one (certain probabilities may be zero). These represent the
 * probabilities of moving in certain relative directions from the intended
 * direction. They are listed in a clockwise manner, starting from the direction
 * which was intended.
 *
 * @param mps The relative movement probabilities, ordered clockwise, starting
 *      in the intended direction.
 * @param inputFile The relative path to the file from which to read, or an
 *      empty string.
 */
Maze::Maze(std::tuple<double, double, double, double> mps, const std::string& inputFile) : mazeIdentifier(inputFile) {
    if (inputFile.empty()) {
        /* Read from input. */
        printf("Please input your self-built maze.\n");
        printf("  Syntax (Step 1): Enter width, height (two integers greater than zero).\n");
        printf("  Syntax (Step 2): Enter rows of maze, represented by characters, ending in ENTER.\n");
        printf("  Legal characters:\n");
        printf("    (.) Path, (*) Start, (1) Goal, (0) Warp, (:) Lever, (|) Gate, (%%) Snack, (#) Pit, ( ) Empty.\n");
        this->getMazeDimensionsFromInput();
        this->getMazeStatesFromInput();
    } else {
        /* Read from a plain text file. */
        std::ifstream inputStream(inputFile);
        this->getMazeDimensionsFromFile(&inputStream);
        this->getMazeStatesFromFile(&inputStream);
    }
    this->getMazeSpecialStates();
    this->moveProbabilities = mps;
}

/**
 * Constructs a maze.
 *
 * This is the shortened constructor for mazes, in which pre-specified relative
 * movement probabilities are set. Only the input file is required.
 *   For details on maze construction, see the full-fledged constructor above.
 *
 * @param inputFile The relative path to the file from which to read, or an
 *      empty string.
 */
Maze::Maze(const std::string& inputFile) : Maze(std::make_tuple(0.8, 0.1, 0.0, 0.1), std::move(inputFile)) {}

/**
 * Destructs a maze.
 */
Maze::~Maze() = default;

/**
 * Determines whether players start at a random legal tile.
 *
 * If no explicitly designated starting states are given, players start at
 * arbitrary starting tiles.
 *
 * @return The question's answer.
 */
bool Maze::shouldStartAtRandomPosition() {
    return this->startingStates.empty();
}

/**
 * Obtains the state index associated with a state's coordinates.
 *
 * @param x The state's horizontal coordinate.
 * @param y The state's vertical coordinate.
 * @return The state's index.
 */
int Maze::indexFromCoordinates(int x, int y) {
    return y * this->width + x;
}

/**
 * Clears the maze's states' activations.
 *
 * This method needs to be called whenever a player has finished an episode
 * through the maze.
 */
void Maze::resetMaze() {
    int stateIndex;
    for (stateIndex = 0; stateIndex < (int)this->states.size(); stateIndex++) {
        this->states[stateIndex].setActivation(false);
    }
}

/**
 * Obtains a reference to the collection of states of this maze.
 *
 * @return The reference to the states.
 */
std::vector<State>* Maze::getStates() {
    return &(this->states);
}

/**
 * Obtains a reference to a state, given its index.
 *
 * @param index The state's index.
 * @return A reference to the state at the given index.
 */
State* Maze::getState(int index) {
    return &(this->states[index]);
}

/**
 * Obtains the starting state for a player to start an episode with.
 *
 * @return The starting state.
 */
State* Maze::getStartingState() {
    int randomIndex;
    State *s;
    if (this->shouldStartAtRandomPosition()) {
        do {
            randomIndex = RandomServices::discreteUniformSample((int)this->states.size() - 1);
            s = &(this->states[randomIndex]);
        } while (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s));
    } else {
        /* From the designated starting states, choose one randomly. */
        do {
            randomIndex = RandomServices::discreteUniformSample((int)this->startingStates.size() - 1);
            s = this->startingStates[randomIndex];
        } while (Maze::stateIsTerminal(s) || Maze::stateIsIntraversible(s));
    }
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

/**
 * Given the intended action, returns the action which will actually occur.
 *
 * With the maze's relative movement probabilities, it is determined which
 * actual action gets executed.
 *
 * @param chosenAction The intended action.
 * @return The action that will be executed in actuality.
 */
Maze::Actions Maze::actualAction(Maze::Actions chosenAction) {
    int a;
    double bar, current;
    bar = RandomServices::continuousUniformSample(1.0);
    current = this->getActionProbability(0);
    for (a = 0; a < ACTION_NUMBER; a++) {
        /* Loop through the relative movement probabilities, and choose the
           action that 'hits the bar', i.e. the one that will be chosen. */
        if (bar <= current) {
            return this->actionFromIndex((chosenAction + a) % ACTION_NUMBER);
        }
        current += this->getActionProbability(a + 1);
    }
    return this->actionFromIndex(ACTION_NUMBER - 1);
}

/**
 * Given the coordinates of a movement's resulting state, determines validity.
 *
 * Notice that a state is deemed 'valid' when it resides within the maze's
 * borders. It may still be invalid due to it being an intraversible tile.
 *
 * @param x The horizontal coordinate of the resulting state.
 * @param y The vertical coordinate of the resulting state.
 * @return The question's answer.
 */
bool Maze::moveIsOutOfBounds(int x, int y) {
    return (x < 0 || x >= this->width) || (y < 0 || y >= this->height);
}

/**
 * Determines whether the given state is intraversible.
 *
 * A state is intraversible when it does not have a tile type, or when it has
 * a locked gate as tile type.
 *
 * @param s The state to evaluate intraversiblity for.
 * @return The question's answer.
 */
bool Maze::stateIsIntraversible(State *s) {
    State::Types t;
    t = s->getType();
    return (t == State::Types::none) || (t == State::Types::gate);
}

/**
 * Determines whether the given state is a termination state.
 *
 * A state is a terminal state when it is a goal (positive case) or a pit
 * (negative case).
 *
 * @param s The state to evaluate.
 * @return The question's answer.
 */
bool Maze::stateIsTerminal(State *s) {
    State::Types t;
    t = s->getType();
    return (t == State::Types::goal) || (t == State::Types::pit);
}

/**
 * Given the resulting state's coordinates, determines movement invalidity.
 *
 * A move is deemed invalid if it is out of bounds or if it is intraversible.
 *
 * @param x The resulting state's horizontal position.
 * @param y The resulting state's vertical position.
 * @return The question's answer.
 */
bool Maze::moveShouldFail(int x, int y) {
    if (this->moveIsOutOfBounds(x, y)) {
        return true;
    } else {
        State *s = &(this->states[this->indexFromCoordinates(x, y)]);
        return this->stateIsIntraversible(s);
    }
}

/**
 * Returns a resulting state from a state-action pair deterministically.
 *
 * 'Deterministic computation' in this method means that no maze randomness
 * is taken into account. Put differently, the chosen action is not manipulated
 * by the maze's relative movement probability effects.
 *
 * @param state The state from which the action is performed.
 * @param action The action to execute.
 * @return The resulting state.
 */
State* Maze::getNextStateDeterministic(State *state, enum Maze::Actions action) {
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
    if (this->moveShouldFail(x, y)) {
        /* Maneuver failed. Remain in the current state.  */
        return state;
    } else {
        return &(this->states[this->indexFromCoordinates(x, y)]);
    }
}

/**
 * Given a state-action pair, computes the resulting state stochastically.
 *
 * Contrary to the deterministic state-action-result computation, this method
 * does take into account maze movement randomness.
 *
 * @param s The state from which to depart.
 * @param action The action to undertake.
 * @return The resulting state.
 */
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

/**
 * Computes the state in which one arrives after taking the given state's warp.
 *
 * A warp moves the player near one of the maze's goals.
 *
 * @param s The state from which to warp.
 * @return The resulting state.
 */
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

/**
 * Unlocks all gates present within the maze.
 *
 * Once the maze's gates are unlocked, they are permanently (within the episode)
 * removed. As such, levers practically become paths.
 */
void Maze::openGates() {
    int gateStateIndex;
    State *s;
    for (gateStateIndex = 0; gateStateIndex < (int)this->gateStates.size(); gateStateIndex++) {
        s = this->gateStates[gateStateIndex];
        s->setActivation(true);
    }
}

/**
 * Removes the snack in the specified state.
 *
 * @param s The state from which to remove the snack.
 */
void Maze::removeSnack(State *s) {
    s->setActivation(true);
}

/**
 * Given a state, computes the resulting state if it is a special state.
 *
 * Passing 'ordinary' states is defined behaviour: no side effects occur. Thus,
 * it is always safe to pass states through this method.
 *
 * @param s The state to evaluate.
 * @return The resulting state.
 */
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

/**
 * Obtains the reward associated with the state.
 *
 * @param state The current location.
 * @return The associated reward.
 */
double Maze::getReward(State *state) {
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

/**
 * Given a state-action pair, computes the state-prime-reward pair.
 *
 * @param s The state from which to depart.
 * @param a The action to undertake.
 * @return The resulting state-prime-reward pair.
 */
std::tuple<State*, double> Maze::getStateTransitionResult(State *s, Actions a) {
    State *newState = this->getNextState(s, a);
    double reward = this->getReward(newState);
    newState = this->getSpecialStateResult(newState);
    return std::make_tuple(newState, reward);
}

/**
 * Serves the string counterpart to the given action.
 *
 * @param a The action to get the string form of.
 * @return The action as a string.
 */
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

/**
 * Obtains the identifier of the maze.
 *
 * A maze identifier stores the file name from which the maze was constructed,
 * if it was built from a plain text file. If composed from input, its maze
 * identifier is simply a dash: '-'.
 *
 * @return The maze's identifier.
 */
std::string Maze::getMazeIdentifier() {
    return this->mazeIdentifier;
}

/**
 * Obtains the width of the maze.
 *
 * @return The maze's width.
 */
int Maze::getMazeWidth() {
    return this->width;
}

/**
 * Obtains the height of the maze.
 *
 * @return The maze's height.
 */
int Maze::getMazeHeight() {
    return this->height;
}
