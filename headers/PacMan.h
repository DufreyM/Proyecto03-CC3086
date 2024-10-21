// PacMan.h
#ifndef PACMAN_H
#define PACMAN_H

#include "Entity.h"

class PacMan : public Entity {
private:
    int score;
    int lives;
    bool poweredUp;  // Estado de poder al comer Power Pellet

public:
    PacMan() : Entity(23, 13), score(0), lives(3), poweredUp(false) {}

    void eatPellet() { score += 10; }
    void eatGhost() { score += 100; }
    void eatPowerPellet() { poweredUp = true; score += 50; }
    void loseLife() { if (lives > 0) --lives; }
    
    bool isPoweredUp() const { return poweredUp; }
    int getScore() const { return score; }
    int getLives() const { return lives; }

    void resetPower() { poweredUp = false; }
};

#endif // PACMAN_H