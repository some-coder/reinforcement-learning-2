#ifndef MAZE_HPP
#define MAZE_HPP

#include <map>
#include <vector>
#include "Position.hpp"
#include "State.hpp"

class Maze {
    enum ValidTiles {Hedge = ' ', Path = '.', Exit = '*'};

    private:
        bool randomStarts;
        int width;
        int height;
        std::map<Position, char> entries;
        static bool isValidTile(char entry);
        static bool isValidEntry(char *entry);
        void ensureConstantWidth(int current);
        bool readMazeEntryFromInput(Position p);
        bool readMazeRowFromInput(int row);
        void setWidth(int newWidth);

    public:
        Maze(bool shouldStartRandomly);
        virtual ~Maze();
        virtual void readMazeFromInput();
        static bool isTraversableTile(char entry);
        bool isWithinBounds(Position p);
        int getWidth();
        int getHeight();
        char getEntry(Position p);
        State getStartingState();
};

#endif