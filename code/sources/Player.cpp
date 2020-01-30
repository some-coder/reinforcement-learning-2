#include <RandomServices.hpp>
#include "Player.hpp"

/**
 * Constructs an arbitrary state policy in which actions are non-probabilistic.
 *
 * @return The state policy.
 */
std::vector<double> Player::randomDiscretePolicy() {
    int randomActionIndex;
    randomActionIndex = RandomServices::discreteUniformSample(Maze::ACTION_NUMBER - 1);
    return Player::actionAsActionProbabilityDistribution(Maze::actionFromIndex(randomActionIndex));
}

/**
 * Constructs an arbitrary state policy in which actions are probabilistic.
 *
 * At the moment, no variations between the returned policies exists; all
 * deliver one in which each action is equally probable to be performed,
 * regardless of state. This is due to some algorithms requiring such a policy
 * to be soft.
 *
 * @return The state policy.
 */
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

/**
 * Constructs a random (possibly stochastic) state policy for the player.
 *
 * @param stochastic Should the returned policy be probabilistic?
 * @return The state policy.
 */
std::vector<double> Player::randomStatePolicy(bool stochastic) {
    if (stochastic) {
        return Player::randomStochasticPolicy();
    } else {
        return Player::randomDiscretePolicy();
    }
}

/**
 * Sets a random (possibly stochastic) maze-wide policy for the player.
 *
 * @param stochastic Should the configured policy be probabilistic?
 */
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

/**
 * Constructs a player.
 *
 * @param m The maze which the player should solve.
 * @param gamma The discounting factor for earlier-obtained rewards.
 * @param initialiseStochastic Should the player's initial policy be
 *      probabilistic?
 */
Player::Player(Maze* m, double gamma, bool initialiseStochastic) {
    this->maze = m;
    this->discountFactor = gamma;
    this->initialiseStateValues();
    this->initialisePolicy(initialiseStochastic);
    this->currentEpoch = this->timeoutEpoch = 0;
}

/**
 * Destructs a player.
 */
Player::~Player() = default;

/**
 * Initialises the state valuations to a pre-specified constant.
 */
void Player::initialiseStateValues() {
    int i;
    State *state;
    std::vector<State>* states;
    states = this->maze->getStates();
    for (i = 0; i < (int)states->size(); i++) {
        state = &(states->at(i));
        if (state->getType() == State::Types::goal ||
            state->getType() == State::Types::pit) {
            /* Terminal states' valuations should be their rewards. */
            this->stateValues[state] = Maze::getReward(state);
        } else {
            this->stateValues[state] = INITIAL_STATE_VALUE;
        }
    }
}

/**
 * Obtains the probability of choosing the given action in the specified state.
 *
 * @param s The state in the state-action pair.
 * @param a The action in the state-action pair.
 * @return The probability of having the action as intention in the state.
 */
double Player::actionProbability(State *s, Maze::Actions a) {
    return this->policy[s][a];
}

/**
 * Derives a discrete action probability distribution from the given action.
 *
 * This method is only meant for discrete, non-probabilistic players.
 *
 * @param a The intended action.
 * @return The action probability distribution.
 */
std::vector<double> Player::actionAsActionProbabilityDistribution(Maze::Actions a) {
    int actionIndex;
    std::vector<double> distribution;
    for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        distribution.push_back( (actionIndex == a ? 1.0 : 0.0) );
    }
    return distribution;
}

/**
 * This function chooses an action for a certain state. The action is chosen based on the policy.
 * The function looks at the probability of the action according to the policy and returns a randomly chosen
 * action (in accordance with its probability).
 * 
 * @param s the state the action has to be chosen for
 * @return an action
 */
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

/**
 * Obtains the time elapsed per epoch, expressed in milliseconds.
 *
 * @return The epoch timings, in milliseconds.
 */
std::vector<double> Player::getEpochTimings() {
    return this->epochTimings;
}

/**
 * Obtains the complete policy of the player.
 *
 * @return The player's policy.
 */
std::map<std::tuple<int, int, Maze::Actions>, double> Player::getPolicy() {
    int stateIndex, actionIndex;
    State *s;
    Maze::Actions a;
    std::tuple<int, int, Maze::Actions> stateActionPair;
    std::map<std::tuple<int, int, Maze::Actions>, double> outputPolicy;
    for (stateIndex = 0; stateIndex < (int)this->stateValues.size(); stateIndex++) {
        s = this->maze->getState(stateIndex);
        for (actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
            a = Maze::actionFromIndex(actionIndex);
            stateActionPair = std::make_tuple(s->getX(), s->getY(), a);
            outputPolicy[stateActionPair] = this->policy[s][a];
        }
    }
    return outputPolicy;
}

/**
 * Obtains each episode's total collected reward.
 *
 * @return The episode rewards.
 */
std::vector<double> Player::getTotalRewardPerEpisode() {
    return std::vector<double>();
}

/**
 * Obtains the string version of the player's type.
 *
 * The type of a player is simply the algorithm the player employs in solving
 * mazes. This may be as simplistic or elaborate as desired.
 *
 * @param type The player type.
 * @return The string version of the player type.
 */
std::string Player::playerTypeAsStringShortened(Player::Types type) {
    switch (type) {
        case RandomPlayer:
            return "RAND";
        case SynchronousPolicyIteration:
            return "SPI";
        case AsynchronousPolicyIteration:
            return "API";
        case SynchronousValueIteration:
            return "SVI";
        case AsynchronousValueIteration:
            return "AVI";
        case MonteCarloExploringStarts:
            return "MCES";
        case MonteCarloFirstVisit:
            return "MCFV";
        case MonteCarloEveryVisit:
            return "MCEV";
        case TDSarsa:
            return "TDS";
        case TDQLearning:
            return "TDQ";
        default:
            return "?";
    }
}

/**
 * Determines whether the timeout epoch has been reached.
 *
 * @return The question's answer.
 */
bool Player::maximumIterationReached() {
    return this->currentEpoch >= this->timeoutEpoch;
}