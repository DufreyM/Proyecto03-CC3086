#ifndef POSITION_H
#define POSITION_H

class Position {
public:
    int x, y;

    Position(int x = 0, int y = 0) : x(x), y(y) {}
    
    // La palabra clave [const] indica que este método no modifica el estado del objeto.
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

#endif // POSITION_H