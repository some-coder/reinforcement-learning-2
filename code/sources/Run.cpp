#include "Run.hpp"

Run::Run(std::vector<PlayerTypes> types) : maze(Maze(false)) {
    this->participantTypes = types;
}
