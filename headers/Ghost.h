// Ghost.h
#ifndef GHOST_H
#define GHOST_H

#include <Entity.h>

class Ghost : public Entity {
private:
    char id;
    int mode;
    bool eaten;  // Indica si el fantasma fue comido y debe regenerarse
    bool inPellet;
    bool inPowerPellet;

public:
    Ghost(int x, int y, char look) : Entity(x, y), mode(0), eaten(false), inPellet(false), inPowerPellet(false), id(look) {}

    void setId(char c) { id = c; }
    char getId() { return id; }

    void setMode(int newMode) { mode = newMode; }
    int getMode() const { return mode; }

    void setWasInPellet(bool state) { inPellet = state; }
    bool getWasInPellet() { return inPellet; }

    void setWasInPowerPellet(bool state) { inPowerPellet = state; }
    bool getWasInPowerPellet() { return inPowerPellet; }

    void markAsEaten() { eaten = true; }
    bool isEaten() const { return eaten; }

    void regenerate(int x, int y) {
        pos = Position(x, y);
        eaten = false;
    }
};

#endif // GHOST_H