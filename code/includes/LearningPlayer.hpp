#ifndef LEARNING_PLAYER_HPP
#define LEARNING_PLAYER_HPP

#include "Player.hpp"

class LearningPlayer : public Player {
    private:
        static constexpr double INITIAL_STATE_ACTION_VALUE = 0.0;
        int timeoutEpoch;
        int currentEpoch;
        std::map<std::tuple<State*, Maze::Actions>, double> stateActionValues;
        void initialiseStateActionValues();

    public:
        LearningPlayer(Maze *m, double gamma, int T);
        ~LearningPlayer() override;
        void setStateActionValue(State *s, Maze::Actions action, double value);
};

#endif