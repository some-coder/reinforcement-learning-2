#include "TimeDifferencePlayer.hpp"

TimeDifferencePlayer::TimeDifferencePlayer(Maze *m, double gamma, int T, double alpha) : LearningPlayer(m, gamma, T) {
    this->alpha = alpha;
}

TimeDifferencePlayer::~TimeDifferencePlayer() = default;

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

