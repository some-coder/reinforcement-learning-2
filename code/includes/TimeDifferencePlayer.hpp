#ifndef TIME_DIFFERENCE_PLAYER_HPP
#define TIME_DIFFERENCE_PLAYER_HPP

#include "LearningPlayer.hpp"

class TimeDifferencePlayer : public LearningPlayer {
    protected:
        static constexpr double INITIAL_QUALITY_VALUE = 0.0;

        double alpha;
        std::map<std::tuple<State*, Maze::Actions>, double> quality;
        
        Maze::Actions greedyAction(State *s);

    public:
        TimeDifferencePlayer(Maze *m, double gamma, int T, double alpha);
        ~TimeDifferencePlayer() override;

    private:
        void initialiseQuality();
        void setQuality(State *s, Maze::Actions action, double value); 
};

#endif