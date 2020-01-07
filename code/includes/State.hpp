#ifndef STATE_HPP
#define STATE_HPP

class State {
    public:
        enum Types {path, start, goal, warp, lever, gate, snack, pit, none};

    private:
        int x;
        int y;
        Types type;

    public:
        State(int x, int y, Types type);
        int getX();
        int getY();
        Types getType();
        void setType(Types t);
};

#endif