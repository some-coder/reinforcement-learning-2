#include "TimeDifferencePlayer.hpp"

TimeDifferencePlayer::TimeDifferencePlayer(Maze *m, double gamma, int T, double alpha) : LearningPlayer(m, gamma, T) {
    this->alpha = alpha;
    this->initialiseQuality();
}

TimeDifferencePlayer::~TimeDifferencePlayer() = default;

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

void TimeDifferencePlayer::setQuality(State *s, Maze::Actions action, double value) {
    this->quality[std::make_tuple(s, action)] = value;
}

/**
 * Returns the action with the highest quality in the provided state.
 * 
 * @param s the state for which the action has to be chosen
 * @return an action
 */
Maze::Actions TimeDifferencePlayer::greedyAction(State *s) {
    int actionIndex;
    std::tuple<State*, Maze::Actions> stateActionPair;
    Maze::Actions currentAction, bestAction;
    double current, best;
    bestAction = Maze::actionFromIndex(0);
    stateActionPair = std::make_tuple(s, bestAction);
    best = this->quality[stateActionPair];
    for (actionIndex = 1; actionIndex < Maze::ACTION_NUMBER; actionIndex++) {
        currentAction = Maze::actionFromIndex(actionIndex);
        stateActionPair = std::make_tuple(s, currentAction);
        current = this->quality[stateActionPair];
        if (current > best) {
            bestAction = currentAction;
            best = current;
        }
    }
    return bestAction;
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

