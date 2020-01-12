#include <RandomServices.hpp>
#include "Player.hpp"

std::vector<double> Player::randomDiscretePolicy() {
    int randomActionIndex;
    randomActionIndex = RandomServices::discreteUniformSample(Maze::ACTION_NUMBER - 1);
    return Player::actionAsActionProbabilityDistribution(Maze::actionFromIndex(randomActionIndex));
}

std::vector<double> Player::randomStochasticPolicy() {
    int actionIndex;
    std::vector<int> presenceScores;
    std::vector<double> probabilisticPolicy;
    int sum;
    sum = 0;
    do {
        for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
            presenceScores.push_back(PRESENCE_SCORE_MAXIMUM - 1);
            sum += presenceScores.back();
        }
    } while (sum == 0);
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        probabilisticPolicy.push_back( presenceScores[actionIndex] / (double)sum );
    }
    return probabilisticPolicy;
}

std::vector<double> Player::randomStatePolicy(bool stochastic) {
    if (stochastic) {
        return Player::randomStochasticPolicy();
    } else {
        return Player::randomDiscretePolicy();
    }
}

void Player::initialisePolicy(bool stochastic) {
    int i;
    State* s;
    std::vector<State> *states;
    states = this->maze->getStates();
    for (i = 0; i < (int)states->size(); i++) {
        s = &(states->at(i));
        this->policy[s] = Player::randomStatePolicy(stochastic);
    }
}

Player::Player(Maze* m, double gamma, bool initialiseStochastic) {
    this->maze = m;
    this->discountFactor = gamma;
    this->initialiseStateValues();
    this->initialisePolicy(initialiseStochastic);
}

Player::~Player() = default;

void Player::initialiseStateValues() {
    int i;
    State *state;
    std::vector<State>* states;
    states = this->maze->getStates();
    for (i = 0; i < (int)states->size(); i++) {
        state = &(states->at(i));
        if (state->getType() == State::Types::goal ||
            state->getType() == State::Types::pit) {
            this->stateValues[state] = Maze::getReward(state);
        } else {
            this->stateValues[state] = INITIAL_STATE_VALUE;
        }
    }
}

double Player::actionProbability(State *s, Maze::Actions a) {
    return this->policy[s][a];
}

std::vector<double> Player::actionAsActionProbabilityDistribution(Maze::Actions a) {
    int actionIndex;
    std::vector<double> distribution;
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        distribution.push_back( (actionIndex == a ? 1.0 : 0.0) );
    }
    return distribution;
}

Maze::Actions Player::chooseAction(State *s) {
    int i;
    double bar, current;
    bar = RandomServices::continuousUniformSample(1.0);
    current = this->actionProbability(s, Maze::actionFromIndex(0));
    for (i = 0; i < Maze::ACTION_NUMBER; i++) {
        if (bar <= current) {
            return Maze::actionFromIndex(i);
        }
        current += this->actionProbability(s, Maze::actionFromIndex(i + 1));
    }
    return Maze::actionFromIndex(Maze::ACTION_NUMBER - 1);
}

void Player::printStateActionProbabilities(State *s) {
    int actionIndex;
    printf("[");
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        printf("%.3lf%s", this->policy[s][actionIndex], (actionIndex == Maze::ACTION_NUMBER - 1 ? "]" : ", "));
    }
}

void Player::printFinalPolicy() {
    int i;
    State *s;
    for (i = 0; i < (int)this->maze->getStates()->size(); i++) {
        s = this->maze->getState(i);
        printf("State (%d, %d) ", s->getX(), s->getY());
        if (Maze::stateIsTerminal(s)) {
            printf("is terminal and has ");
        } else if (Maze::stateIsIntraversible(s)) {
            printf("is intraversible and has ");
        } else {
            printf("has action %s and ", Maze::actionAsString(this->chooseAction(s)).c_str());
        }
        printf("state utility %.3lf.\n", this->stateValues[s]);
    }
}

std::vector<double> Player::getEpochTimings() {
    return this->epochTimings;
}

std::map<std::tuple<int, int, Maze::Actions>, double> Player::getPolicy() {
    int stateIndex, actionIndex;
    State *s;
    Maze::Actions a;
    std::tuple<int, int, Maze::Actions> stateActionPair;
    std::map<std::tuple<int, int, Maze::Actions>, double> outputPolicy;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
            s = this->maze->getState(stateIndex);
            a = Maze::actionFromIndex(actionIndex);
            stateActionPair = std::make_tuple(s->getX(), s->getY(), a);
            outputPolicy[stateActionPair] = this->policy[s][a];
        }
    }
    return outputPolicy;
}

std::string Player::playerTypeAsString(Player::Types type) {
    switch (type) {
        case SynchronousPolicyIteration:
            return "Synchronous Policy Iteration";
        case AsynchronousPolicyIteration:
            return "Asynchronous Policy Iteration";
        case SynchronousValueIteration:
            return "Synchronous Value Iteration";
        case AsynchronousValueIteration:
            return "Asynchronous Value Iteration";
        case MonteCarloExploringStarts:
            return "Monte Carlo Exploring Starts";
        case MonteCarloFirstVisit:
            return "Monte Carlo First Visit";
        case MonteCarloEveryVisit:
            return "Monte Carlo Every Visit";
        default:
            return "Unknown Algorithm";
    }
}

bool Player::maximumIterationReached() {
    return this->currentEpoch >= this->timeoutEpoch;
}

char Player::symbolToCharacter(State::Types symbol) {
    switch (symbol) {
        case State::Types::path:
            return '.';
        case State::Types::start:
            return '*';
        case State::Types::goal:
            return '1';
        case State::Types::warp:
            return '0';
        case State::Types::lever:
            return ':';
        case State::Types::gate:
            return '|';
        case State::Types::snack:
            return '%';
        case State::Types::pit:
            return '#';
        case State::Types::none:
            return ' ';
    }
}

void Player::printSituation() {
    int stateIndex;
    State *s;
    printf("EPOCH %d\n", this->currentEpoch);
    stateIndex = 0;
    do {
        s = this->maze->getState(stateIndex);
        printf("%c", Player::symbolToCharacter(this->maze->getState(stateIndex)->getType()));
        stateIndex++;
        if (this->maze->getState(stateIndex)->getX() == 0) {
            printf("\n");
        }
    } while (stateIndex < this->maze->getStates()->size());
}