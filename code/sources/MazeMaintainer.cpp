#include <iostream>
#include "MazeMaintainer.hpp"

State::Types MazeMaintainer::typeFromInput(char input) {
    switch (input) {
        case '1':
            return State::Types::goal;
        case '0':
            return State::Types::warp;
        case ':':
            return State::Types::lever;
        case '|':
            return State::Types::gate;
        case '%':
            return State::Types::snack;
        case '#':
            return State::Types::pit;
        default:
            return State::Types::path;
    }
}

void MazeMaintainer::getMazeSizeFromInput(int *width, int *height) {
    scanf("%d %d\n", width, height);
}

void MazeMaintainer::getMazeStatesFromInput(int width, int height, std::vector<State> *states,
        std::map<std::tuple<int, int>, State*> *map) {
    int x, y;
    char input;
    for (y = 0; y < height; y++) {
        for (x = 0; x < (width + 1); x++) {
            input = getchar();
            if (x != width && input != NO_STATE) {
                states->emplace_back(x, y, typeFromInput(input));
                std::tuple<int, int> key = std::make_tuple(x, y);
                State *value = &(states->at(states->size() - 1));
                std::cout << "Value: " << value->getX() << std::endl;
                map->insert(std::pair<std::tuple<int, int>, State*>(key, value));
                std::cout << "(" << x << ", " << y << ")" << std::endl;
            }
        }
    }
}
