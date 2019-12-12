#ifndef MAZE_HPP
#define MAZE_HPP

#include <map>
#include <vector>
#include "Position.hpp"

class Maze {
    enum ValidTiles {Hedge = '#', Path = '.', Exit = '*'};

    private:
        int width;
        int height;
        std::map<Position, char> entries;
        static bool isValidTile(char entry);
        static bool isValidEntry(char *entry);
        void ensureConstantWidth(int current);
        bool readMazeEntryFromInput(Position p);
        bool readMazeRowFromInput(int row);
        bool isWithinBounds(Position p);
        void setWidth(int newWidth);

    public:
        Maze();
        virtual ~Maze();
        virtual void readMazeFromInput();
        int getWidth();
        int getHeight();
        char getEntry(Position p);
};

#endif