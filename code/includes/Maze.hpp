#ifndef MAZE_HPP
#define MAZE_HPP

#include <map>
#include <vector>

class Maze {
    private:
        int width;
        int height;
        std::map<std::vector<int>, char> contents;
        bool readMazeRowFromInput(int currentRow);
        bool isWithinBounds(int x, int y);

    public:
        Maze();
        virtual ~Maze();
        virtual void readMazeFromInput();
        int getWidth();
        int getHeight();
        char getEntry(int x, int y);
};

#endif