#ifndef INPUT_SERVICES_HPP
#define INPUT_SERVICES_HPP

#include <vector>
#include <tuple>
#include <map>
#include "State.hpp"

class MazeMaintainer {
    private:
        static constexpr char NO_STATE = ' ';
        static State::Types typeFromInput(char input);

    public:
        static void getMazeSizeFromInput(int *width, int *height);
        static void getMazeStatesFromInput(int width, int height, std::vector<State> *states,
                std::map<std::tuple<int, int>, State*> *map);
};

#endif