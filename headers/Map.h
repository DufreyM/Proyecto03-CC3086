// Map.h
#ifndef MAP_H
#define MAP_H

#include <vector>
#include <iostream>
#include "Position.h"
#include "PacMan.h"
#include "Ghost.h"

class Map {
private:
    std::vector<std::vector<char>> grid;
    PacMan pacman;
    Ghost blinky;
    Ghost pinky;
    Ghost inky;
    Ghost clyde;

    bool isWall(const Position& pos) const { return grid[pos.x][pos.y] == '\xDB'; }

    bool isGhost(const Position& pos) const;

    bool isPellet(const Position& pos) const { return grid[pos.x][pos.y] == '.'; }

    bool isPowerPellet(const Position& pos) const { return grid[pos.x][pos.y] == 'O'; }

    bool isClearCell(const Position& pos) const { return grid[pos.x][pos.y] == ' '; }

    void clearCell(const Position& pos) { grid[pos.x][pos.y] = ' '; }

    void setCell(const Position& pos, char character) { grid[pos.x][pos.y] = character; }

public:
    Map(std::vector<std::vector<char>> layout): grid(layout), pacman(PacMan()), blinky(Ghost(14,13)), pinky(Ghost(14,14)), inky(Ghost(14,15)), clyde(Ghost(14,16)) {}

    void mostrarMapa() const;
    
    void movePacman(int x, int y);

    void moveBlinky(int x, int y);

    PacMan getPacMan() { return pacman; }

    void gameOver() const;
};

#endif // MAP_H