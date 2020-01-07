#ifndef TIME_DIFFERENCE_PLAYER_HPP
#define TIME_DIFFERENCE_PLAYER_HPP

#include "LearningPlayer.hpp"

class TimeDifferencePlayer : public LearningPlayer {
    protected:
        double alpha;

    public:
        TimeDifferencePlayer(Maze *m, double gamma, int T, double alpha);
        ~TimeDifferencePlayer() override;
};

#endif