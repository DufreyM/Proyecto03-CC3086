// Map.cpp
#include <Map.h>
#include <ncurses.h>

void Map::mostrarMapa() const {
    // Limpia la pantalla de forma eficiente
    clear();
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            // Dibuja cada celda en su posición
            mvaddch(i, j, grid[i][j]);
        }
    }
    mvprintw(35, 0, "Puntuación: %d", pacman.getScore());
    mvprintw(35, 20, "Vidas: %d", pacman.getLives());
    // Actualiza la pantalla
    refresh();
}

void Map::gameOver() const {
    clear();
    mvprintw(20, 10, "Game Over");
    mvprintw(21, 10, "Puntuación: %d", pacman.getScore());
    mvprintw(23, 10, "Presiona 'x' para salir.");
    refresh();
}

bool Map::isGhost(const Position& pos) const {
    char cell = grid[pos.x][pos.y];
    return cell == '1' || cell == '2' || cell == '3' || cell == '4' || cell == '$';
}

void Map::movePacman(int x, int y) {
    Position newPos(pacman.getPosition().x + x, pacman.getPosition().y + y);
    
    if(isWall(newPos)) {
        mostrarMapa();
    } else {
        if(isPellet(newPos)) {
            clearCell(pacman.getPosition());
            pacman.setPosition(newPos);
            pacman.eatPellet();
            setCell(newPos, '@');
        }
        if(isPowerPellet(newPos)) {
            clearCell(pacman.getPosition());
            pacman.setPosition(newPos);
            pacman.eatPowerPellet();
            setCell(newPos, '@');
        } else {
            clearCell(pacman.getPosition());
            pacman.setPosition(newPos);
            setCell(newPos, '@');
        }
        mostrarMapa();
    }
}

void Map::moveBlinky(int x, int y) {
    Position newPos(blinky.getPosition().x + x, blinky.getPosition().y + y);
    char ghostState;
    if(pacman.isPoweredUp()) { ghostState = '$';} else { ghostState = '1';}

    if (newPos == pacman.getPosition() && pacman.isPoweredUp()) {
        blinky.regenerate(14, 13);
        pacman.eatGhost();
    } else {
        if(newPos == pacman.getPosition() && !pacman.isPoweredUp()) {
            pacman.loseLife();
            clearCell(pacman.getPosition());
            pacman.setPosition(23, 13);
        } else {
            if(isWall(newPos) || isGhost(newPos)) {
                mostrarMapa();
            } else {
                if(isPellet(newPos) && (blinky.getWasInPellet() || blinky.getWasInPowerPellet())) {
                    if (blinky.getWasInPellet()) {
                        setCell(blinky.getPosition(), '.');
                    } else {
                        setCell(blinky.getPosition(), 'O');
                        blinky.setWasInPowerPellet(false);
                        blinky.setWasInPellet(true);
                    }
                    blinky.setPosition(newPos);
                    setCell(newPos, ghostState);
                }
                if(isPowerPellet(newPos) && (blinky.getWasInPellet() || blinky.getWasInPowerPellet())) {
                    if (blinky.getWasInPellet()) {
                        setCell(blinky.getPosition(), '.');
                        blinky.setWasInPowerPellet(true);
                        blinky.setWasInPellet(false);
                    } else {
                        setCell(blinky.getPosition(), 'O');
                    }
                    blinky.setPosition(newPos);
                    setCell(newPos, ghostState);
                } else {
                    if(isPellet(newPos)) {
                        blinky.setWasInPellet(true);
                    }
                    if(isPowerPellet(newPos)) {
                        blinky.setWasInPowerPellet(true);
                    }
                    clearCell(blinky.getPosition());
                    blinky.setPosition(newPos);
                    setCell(newPos, ghostState);
                }
            }
        }
    }
    mostrarMapa();
}