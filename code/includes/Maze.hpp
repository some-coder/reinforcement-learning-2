#ifndef MAZE_HPP
#define MAZE_HPP

#include <vector>
#include <tuple>
#include <string>
#include "State.hpp"

class Maze {
    public:
        enum Actions {moveUp, moveRight, moveDown, moveLeft};
        enum Rotations {Zero, Quarter, Half, ThreeQuarters};
        static constexpr int ACTION_NUMBER = 4;
        static constexpr double NORMAL_REWARD = -0.04;
        static constexpr double POSITIVE_REWARD = 1.0;
        static constexpr double NEGATIVE_REWARD = -1.0;
        static constexpr double GOAL_REWARD = 1.0;
        static constexpr int MAZES_AVAILABLE = 1;

    private:
        std::tuple<double, double, double, double> moveProbabilities;
        int width;
        int height;
        const std::string mazeIdentifier;
        std::vector<State> states;
        std::vector<State*> startingStates;
        std::vector<State*> gateStates;
        std::vector<State*> goalStates;
        static State::Types typeFromInput(char input);
        void getMazeDimensionsFromInput();
        void getMazeDimensionsFromFile(std::ifstream *inputStream);
        void getMazeStatesFromInput();
        void getMazeStatesFromFile(std::ifstream *inputStream);
        void getMazeSpecialStates();
        bool shouldStartAtRandomPosition();
        Actions actualAction(Actions chosenAction);
        bool moveIsOutOfBounds(int x, int y);
        bool moveShouldFail(int x, int y);
        State* getWarpStateResult(State *s);
        void openGates();
        static void removeSnack(State *s);
        State* getSpecialStateResult(State *s);

    public:
        explicit Maze(std::tuple<double, double, double, double> mps, const std::string& inputFile = "");
        explicit Maze(const std::string& inputFile = "");
        static bool stateIsIntraversible(State *s);
        static bool stateIsTerminal(State *s);
        int indexFromCoordinates(int x, int y);
        static Actions actionFromIndex(int index);
        std::tuple<int, int> coordinatesFromIndex(int i);
        void resetMaze();
        std::vector<State>* getStates();
        static double getReward(State *state);
        State* getState(int index);
        State* getStartingState();
        double getActionProbability(int relativeIndex);
        State* getNextStateDeterministic(State *state, Actions action);
        State* getNextState(State *state, Actions action);
        std::tuple<State*, double> getStateTransitionResult(State *s, Actions a);
        std::string getMazeIdentifier();
        int getMazeWidth();
        int getMazeHeight();
        static std::string actionAsString(Actions a);
};

#endif