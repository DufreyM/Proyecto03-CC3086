// Map.h
#ifndef MAP_H
#define MAP_H

#include <vector>
#include <iostream>
#include <PacMan.h>
#include <Ghost.h>

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
    
    bool isPacman(const Position& pos) const { return grid[pos.x][pos.y] == '@'; }

    bool isPellet(const Position& pos) const { return grid[pos.x][pos.y] == '.'; }

    bool isPowerPellet(const Position& pos) const { return grid[pos.x][pos.y] == 'O'; }

    bool isClearCell(const Position& pos) const { return grid[pos.x][pos.y] == ' '; }

    bool isFrightenedGhost(const Position& pos) const { return grid[pos.x][pos.y] == '$'; }

    void clearCell(const Position& pos) { grid[pos.x][pos.y] = ' '; }

    void setCell(const Position& pos, char character) { grid[pos.x][pos.y] = character; }

    Ghost& fantasmaFactory(Position& pos, int x);

    int manhattanDistance(const Position &a, const Position &b) { return abs(a.x - b.x) + abs(a.y - b.y); }

public:
    Map(std::vector<std::vector<char>> layout): grid(layout), pacman(PacMan()), blinky(Ghost(14,12, '1')), pinky(Ghost(14,13, '2')), inky(Ghost(14,14, '3')), clyde(Ghost(14,15, '4')) {}

    void mostrarMapa() const;

    void newLevel(int level) const;
    
    void movePacman(int x, int y);

    void moveGhostChase(int numGhost);

    void moveBlinky();

    void movePinky();

    void moveInky();

    void moveClyde();

    PacMan getPacMan() { return pacman; }

    void gameOver() const;
};

#endif // MAP_H