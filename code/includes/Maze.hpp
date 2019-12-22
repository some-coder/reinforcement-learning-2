#ifndef MAZE_HPP
#define MAZE_HPP

#include <vector>
#include <tuple>
#include "State.hpp"

class Maze {
    public:
        enum Actions {moveUp, moveRight, moveDown, moveLeft};
        static constexpr double NORMAL_REWARD = -1.0;
        static constexpr double POSITIVE_REWARD = 1.0;
        static constexpr double NEGATIVE_REWARD = -10.0;
        static constexpr double GOAL_REWARD = 0.0;

    private:
        std::tuple<double, double, double, double> moveProbabilities;
        int width;
        int height;
        std::vector<State> states;
        std::vector<State*> startingStates;
        std::vector<State*> gateStates;
        std::vector<State*> goalStates;
        static State::Types typeFromInput(char input);
        void getMazeDimensionsFromInput();
        void getMazeStatesFromInput();
        void getMazeSpecialStates();
        bool shouldStartAtRandomPosition();
        static Actions actionFromIndex(int index);
        Actions actualAction(Actions chosenAction);
        bool moveIsOutOfBounds(int x, int y);
        bool stateIsIntraversible(State *s);
        bool moveShouldFail(int x, int y);
        State* getWarpStateResult(State *s);
        void openGates();
        void removeSnack(State *s);
        State* getSpecialStateResult(State *s);
        State* getNextState(State *state, Actions action);
        static double getReward(State *state);

    public:
        Maze();
        int indexFromCoordinates(int x, int y);
        std::tuple<int, int> coordinatesFromIndex(int i);
        State* getStartingState();
        double getActionProbability(Actions action);
        std::tuple<State*, double> getStateTransitionResult(State *s, Actions a);
};

#endif