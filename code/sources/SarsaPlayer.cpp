#include "SarsaPlayer.hpp"

SarsaPlayer::SarsaPlayer(Maze *m, double gamma, int T, double alpha, double epsilon) : TimeDifferencePlayer(m, gamma, T, alpha) {
    this-> epsilon = epsilon;
    this->updatePolicyUsingQuality();   /* Overwrite parent's policy construction, which was arbitrary. */
}

SarsaPlayer::~SarsaPlayer() = default;

void SarsaPlayer::solveMaze() {
    do {
        auto startTime = std::chrono::high_resolution_clock::now();
        this->performIteration();
        auto endTime = std::chrono::high_resolution_clock::now();
        this->epochTimings.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count() / 1e3);
    } while (!this->maximumIterationReached());
}

void SarsaPlayer::performIteration() {
    std::tuple<State*, Maze::Actions> startStateActionPair;
    startStateActionPair = this->initialStateActionPair();
    this->generateEpisode(startStateActionPair);
    this->currentEpoch++;
}

void SarsaPlayer::generateEpisode(std::tuple<State*, Maze::Actions> startStateActionPair) {
    int currentIteration, maximumIteration;
    double reward;
    std::tuple<State*, Maze::Actions> stateActionPair, nextStateActionPair;
    currentIteration = -1;
    maximumIteration = std::ceil(EPISODE_TIMEOUT_FRACTION * (double)this->stateValues.size());
    stateActionPair = startStateActionPair;
    do {
        currentIteration++;
        nextStateActionPair = this->nextStateActionPair(stateActionPair);
        reward = this->rewards.back();
        this->quality[stateActionPair] += this->alpha *
                (reward + (this->discountFactor * this->quality[nextStateActionPair]) - this->quality[stateActionPair]);
        this->updatePolicyUsingQuality(std::get<0>(stateActionPair));
        stateActionPair = nextStateActionPair;
    } while (currentIteration < maximumIteration && !Maze::stateIsTerminal(std::get<0>(stateActionPair)));
    this->maze->resetMaze();
}

/**
 * This function loops through all states and updates for every state the policy using an epsilon-greedy approach.
 * It is epsilon-greedy with respect to the quality of the action.
 */
void SarsaPlayer::updatePolicyUsingQuality() {
    State *state;
    Maze::Actions greedyAction, currentAction;
    std::vector<State>* states;
    std::vector<double> newPolicy;
    double newActionProbability;
    states = this->maze->getStates();
    for (int i = 0; i < (int)states->size(); i++) {
        state = &(states->at(i));
        greedyAction = this->greedyAction(state);
        for (int actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
            currentAction = Maze::actionFromIndex(actionIndex);
            if (currentAction == greedyAction) {
                newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER) + 1.0 - this->epsilon;
            } else {
                newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER);
            }
            newPolicy.push_back(newActionProbability);
        }
        this->policy[state] = newPolicy;
    }
}

/**
 * This function updates the policy of the provided state using an epsilon-greedy approach.
 * It is epsilon-greedy for the quality.
 * 
 * @param state The state for which we update the policy
 */
void SarsaPlayer::updatePolicyUsingQuality(State *state) {
    Maze::Actions greedyAction, currentAction;
    std::vector<double> newPolicy;
    double newActionProbability;
    greedyAction = this->greedyAction(state);
    for (int actionIndex = 0; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        currentAction = Maze::actionFromIndex(actionIndex);
        if (currentAction == greedyAction) {
            newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER) + 1.0 - this->epsilon;
        } else {
            newActionProbability = (this->epsilon / (double)Maze::ACTION_NUMBER);
        }
        newPolicy.push_back(newActionProbability);
    }
    this->policy[state] = newPolicy;
}

std::tuple<State*, Maze::Actions> SarsaPlayer::initialStateActionPair() {
    State *s;
    Maze::Actions a;
    s = this->maze->getStartingState();
    a = this->chooseAction(s);
    return std::make_tuple(s, a);
}

std::tuple<State*, Maze::Actions> SarsaPlayer::nextStateActionPair(
        std::tuple<State*, Maze::Actions> currentPair) {
    std::tuple<State*, double> result;
    result = this->maze->getStateTransitionResult(std::get<0>(currentPair), std::get<1>(currentPair));
    this->rewards.push_back(std::get<1>(result));
    return std::make_tuple(std::get<0>(result), this->chooseAction(std::get<0>(result)));
}

/*
Evaluate_Policy(policy):
    randomly_initialize_non_terminal_states_values()

    Loop number_of_episodes:
        let s = start_state()  
        # Play episode until the end

        Loop until game_over():    
            let a = get_action(policy, s, 0.1) 
            # get action to perform on state s according 
            # to the given policy 90% of the time, and a
            # random action 10% of the time.    
        
            let (s', r) = make_move(s, a) 
            #make move from s using a and get 
            #the new state s' and the reward r
            # incrementally compute the average at V(s). Notice that V(s)
            # depends on an estimate of V(s') and not on the return 
            # G as in MC

            let V(s) = V(s) + alpha * [r + gamma * V(s') - V(s)]
    
            let s = s'
        End Loop
    End Loop
*/

