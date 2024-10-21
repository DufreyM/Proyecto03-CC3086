#ifndef ENTITY_H
#define ENTITY_H

#include "Position.h"

class Entity {
protected:
    Position pos;

public:
    Entity(int x = 0, int y = 0) : pos(x, y) {}
    
    virtual void move(int dx, int dy) {
        pos.x += dx;
        pos.y += dy;
    }

    // La palabra clave [const] indica que este método no modifica el estado del objeto.
    Position getPosition() const { return pos; }

    void setPosition(int x, int y) { pos.x = x, pos.y = y; }
    
    void setPosition(Position newPos) { pos = newPos; }

    // Destructore virtual (usar como: delete [nombre del objeto])
    virtual ~Entity() = default;
};

#endif // ENTITY_H