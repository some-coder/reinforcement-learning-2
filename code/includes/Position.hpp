#ifndef POSITION_HPP
#define POSITION_HPP

class Position {
    private:
        int x;
        int y;

    public:
        Position(int x, int y);
        bool operator<(const Position & other) const;
        int getXCoordinate();
        int getYCoordinate();
};

#endif