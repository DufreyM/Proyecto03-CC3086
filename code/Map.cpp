// Map.cpp
#include <Map.h>
#include <ncurses.h>
#include <iostream>
#include <random>

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
    mvprintw(23, 10, "Presiona 'q' para salir.");
    refresh();
    while(getchar()!='q');//loop until the user presses q
	endwin();
    exit(0);
}

void Map::newLevel(int level) const {
    clear();
    mvprintw(20, 10, "¡Felicidades pasas al Nivel %d!", level);
    mvprintw(21, 10, "Puntuación: %d", pacman.getScore());
    mvprintw(23, 10, "Presiona 'c' para continuar.");
    refresh();
    while(getchar()!='c');//loop until the user presses q
    exit(0);
}

bool Map::isGhost(const Position& pos) const {
    char cell = grid[pos.x][pos.y];
    return cell == '1' || cell == '2' || cell == '3' || cell == '4' || cell == '$';
}

void Map::movePacman(int x, int y) {
    Position newPos(pacman.getPosition().x + x, pacman.getPosition().y + y);
    
    if(isGhost(newPos)) {
        if(isFrightenedGhost(newPos)) {
            clearCell(pacman.getPosition());
            pacman.setPosition(newPos);
            setCell(pacman.getPosition(), '@');
            fantasmaFactory(newPos, 0).regenerate(14, 14);
            setCell(fantasmaFactory(newPos, 0).getPosition(), fantasmaFactory(newPos, 0).getId());
            pacman.eatGhost();
            pacman.resetPower();
        } else {
            pacman.loseLife();
            clearCell(pacman.getPosition());
            pacman.setPosition(23, 13);
            setCell(pacman.getPosition(), '@');
        }
    } else if(isPellet(newPos)) {
        clearCell(pacman.getPosition());
        pacman.setPosition(newPos);
        pacman.eatPellet();
        setCell(pacman.getPosition(), '@');
    } else if(isPowerPellet(newPos)) {
        clearCell(pacman.getPosition());
        pacman.setPosition(newPos);
        pacman.eatPowerPellet();
        setCell(pacman.getPosition(), '@');
    } else if(!isWall(newPos)) {
        clearCell(pacman.getPosition());
        pacman.setPosition(newPos);
        setCell(pacman.getPosition(), '@');
    }
    mostrarMapa();
}

void Map::moveGhostChase(int numGhost) {
    std::random_device rd;  // Semilla aleatoria del sistema
    std::mt19937 gen(rd()); // Mersenne Twister como generador

    // Distribución uniforme de enteros entre -1 y 1
    std::uniform_int_distribution<int> distrib(-1, 1);

    Position someOtherPosition(40, 40); // Inicializa someOtherPosition con valores específicos
    Position& originPos = someOtherPosition; // originPos es una referencia a someOtherPosition

    // Determine the correct ghost state based on Pac-Man's power-up status
    Ghost currentGhost = fantasmaFactory(originPos,numGhost);
    if (pacman.isPoweredUp()) {
        currentGhost.markAsEaten();
        currentGhost.setId('$');
    }

    Position bestMove(currentGhost.getPosition().x + distrib(gen), currentGhost.getPosition().y + distrib(gen));

    if(isWall(bestMove) || isGhost(bestMove)) {
        mostrarMapa();
        return;
    }

    // Handle ghost-Pac-Man collision
    if(isPacman(bestMove)) {
        if(currentGhost.isEaten()) {
            if(currentGhost.getWasInPellet()) {
                setCell(currentGhost.getPosition(), '.');
                currentGhost.regenerate(14, 14);
                setCell(currentGhost.getPosition(), fantasmaFactory(originPos, numGhost).getId());
                currentGhost.setWasInPellet(false);
            } else if(currentGhost.getWasInPowerPellet()) {
                setCell(currentGhost.getPosition(), 'O');
                currentGhost.regenerate(14, 14);
                setCell(currentGhost.getPosition(), fantasmaFactory(originPos, numGhost).getId());
                currentGhost.setWasInPowerPellet(false);
            } else {
                clearCell(currentGhost.getPosition());
                currentGhost.regenerate(14, 14);
                setCell(currentGhost.getPosition(), currentGhost.getId());
            }
        } else {
            pacman.loseLife();
            pacman.setPosition(23, 13);
            setCell(pacman.getPosition(), '@');

            if(currentGhost.getWasInPellet()) {
                setCell(currentGhost.getPosition(), '.');
                currentGhost.setPosition(bestMove);
                setCell(currentGhost.getPosition(), currentGhost.getId());
                currentGhost.setWasInPellet(false);
            } else if(currentGhost.getWasInPowerPellet()) {
                setCell(currentGhost.getPosition(), 'O');
                currentGhost.setPosition(bestMove);
                setCell(currentGhost.getPosition(), currentGhost.getId());
                currentGhost.setWasInPowerPellet(false);
            } else {
                clearCell(currentGhost.getPosition());
                currentGhost.setPosition(bestMove);
                setCell(currentGhost.getPosition(), currentGhost.getId());
            }
        }
        mostrarMapa();
        return;
    }

    if(isPellet(bestMove)) {
        if(currentGhost.getWasInPellet()) {
            setCell(currentGhost.getPosition(), '.');
            currentGhost.setPosition(bestMove);
            setCell(currentGhost.getPosition(), currentGhost.getId());
        } else if(currentGhost.getWasInPowerPellet()) {
            setCell(currentGhost.getPosition(), 'O');
            currentGhost.setPosition(bestMove);
            setCell(currentGhost.getPosition(), currentGhost.getId());
            currentGhost.setWasInPowerPellet(false);
            currentGhost.setWasInPellet(true);
        } else {
            clearCell(currentGhost.getPosition());
            currentGhost.setPosition(bestMove);
            setCell(currentGhost.getPosition(), currentGhost.getId());
            currentGhost.setWasInPellet(true);
        }
        mostrarMapa();
        return;
    }

    if(isPowerPellet(bestMove)) {
        if(currentGhost.getWasInPellet()) {
            setCell(currentGhost.getPosition(), '.');
            currentGhost.setPosition(bestMove);
            setCell(currentGhost.getPosition(), currentGhost.getId());
            currentGhost.setWasInPellet(false);
            currentGhost.setWasInPowerPellet(true);
        } else if(currentGhost.getWasInPowerPellet()) {
            setCell(currentGhost.getPosition(), 'O');
            currentGhost.setPosition(bestMove);
            setCell(currentGhost.getPosition(), currentGhost.getId());
        } else {
            clearCell(currentGhost.getPosition());
            currentGhost.setPosition(bestMove);
            setCell(currentGhost.getPosition(), currentGhost.getId());
            currentGhost.setWasInPowerPellet(true);
        }
        mostrarMapa();
        return;
    }

    mostrarMapa();  // Display the updated map (por si a caso)
}

void Map::moveBlinky() {
    std::random_device rd;  // Semilla aleatoria del sistema
    std::mt19937 gen(rd()); // Mersenne Twister como generador

    // Distribución uniforme de enteros entre -1 y 1
    std::uniform_int_distribution<int> distrib(-1, 1);

    Position newPos(blinky.getPosition().x + distrib(gen), blinky.getPosition().y + distrib(gen));

    char ghostState;
    if(pacman.isPoweredUp()) { ghostState = '$';} else { ghostState = '1';}

    if (newPos == pacman.getPosition() && pacman.isPoweredUp()) {
        blinky.regenerate(14, 12);
        pacman.eatGhost();
    } else {
        if(newPos == pacman.getPosition() && !pacman.isPoweredUp()) {
            pacman.loseLife();
            clearCell(blinky.getPosition());
            blinky.setPosition(newPos);
            setCell(newPos, ghostState);
            pacman.setPosition(23, 13);
            setCell(pacman.getPosition(), '@');
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

void Map::movePinky() {
    std::random_device rd;  // Semilla aleatoria del sistema
    std::mt19937 gen(rd()); // Mersenne Twister como generador

    // Distribución uniforme de enteros entre -1 y 1
    std::uniform_int_distribution<int> distrib(-1, 1);

    Position newPos(pinky.getPosition().x + distrib(gen), pinky.getPosition().y + distrib(gen));

    char ghostState;
    if(pacman.isPoweredUp()) { ghostState = '$';} else { ghostState = '2';}

    if (newPos == pacman.getPosition() && pacman.isPoweredUp()) {
        pinky.regenerate(14, 13);
        pacman.eatGhost();
    } else {
        if(newPos == pacman.getPosition() && !pacman.isPoweredUp()) {
            pacman.loseLife();
            clearCell(pinky.getPosition());
            pinky.setPosition(newPos);
            setCell(newPos, ghostState);
            pacman.setPosition(23, 13);
            setCell(pacman.getPosition(), '@');
        } else {
            if(isWall(newPos) || isGhost(newPos)) {
                mostrarMapa();
            } else {
                if(isPellet(newPos) && (pinky.getWasInPellet() || pinky.getWasInPowerPellet())) {
                    if (pinky.getWasInPellet()) {
                        setCell(pinky.getPosition(), '.');
                    } else {
                        setCell(pinky.getPosition(), 'O');
                        pinky.setWasInPowerPellet(false);
                        pinky.setWasInPellet(true);
                    }
                    pinky.setPosition(newPos);
                    setCell(newPos, ghostState);
                }
                if(isPowerPellet(newPos) && (pinky.getWasInPellet() || pinky.getWasInPowerPellet())) {
                    if (pinky.getWasInPellet()) {
                        setCell(pinky.getPosition(), '.');
                        pinky.setWasInPowerPellet(true);
                        pinky.setWasInPellet(false);
                    } else {
                        setCell(pinky.getPosition(), 'O');
                    }
                    pinky.setPosition(newPos);
                    setCell(newPos, ghostState);
                } else {
                    if(isPellet(newPos)) {
                        pinky.setWasInPellet(true);
                    }
                    if(isPowerPellet(newPos)) {
                        pinky.setWasInPowerPellet(true);
                    }
                    clearCell(pinky.getPosition());
                    pinky.setPosition(newPos);
                    setCell(newPos, ghostState);
                }
            }
        }
    }
    mostrarMapa();
}

void Map::moveInky() {
    std::random_device rd;  // Semilla aleatoria del sistema
    std::mt19937 gen(rd()); // Mersenne Twister como generador

    // Distribución uniforme de enteros entre -1 y 1
    std::uniform_int_distribution<int> distrib(-1, 1);

    Position newPos(inky.getPosition().x + distrib(gen), inky.getPosition().y + distrib(gen));

    char ghostState;
    if(pacman.isPoweredUp()) { ghostState = '$';} else { ghostState = '3';}

    if (newPos == pacman.getPosition() && pacman.isPoweredUp()) {
        inky.regenerate(14, 13);
        pacman.eatGhost();
    } else {
        if(newPos == pacman.getPosition() && !pacman.isPoweredUp()) {
            pacman.loseLife();
            clearCell(inky.getPosition());
            inky.setPosition(newPos);
            setCell(newPos, ghostState);
            pacman.setPosition(23, 13);
            setCell(pacman.getPosition(), '@');
        } else {
            if(isWall(newPos) || isGhost(newPos)) {
                mostrarMapa();
            } else {
                if(isPellet(newPos) && (inky.getWasInPellet() || inky.getWasInPowerPellet())) {
                    if (inky.getWasInPellet()) {
                        setCell(inky.getPosition(), '.');
                    } else {
                        setCell(inky.getPosition(), 'O');
                        inky.setWasInPowerPellet(false);
                        inky.setWasInPellet(true);
                    }
                    inky.setPosition(newPos);
                    setCell(newPos, ghostState);
                }
                if(isPowerPellet(newPos) && (inky.getWasInPellet() || inky.getWasInPowerPellet())) {
                    if (inky.getWasInPellet()) {
                        setCell(inky.getPosition(), '.');
                        inky.setWasInPowerPellet(true);
                        inky.setWasInPellet(false);
                    } else {
                        setCell(inky.getPosition(), 'O');
                    }
                    inky.setPosition(newPos);
                    setCell(newPos, ghostState);
                } else {
                    if(isPellet(newPos)) {
                        inky.setWasInPellet(true);
                    }
                    if(isPowerPellet(newPos)) {
                        inky.setWasInPowerPellet(true);
                    }
                    clearCell(inky.getPosition());
                    inky.setPosition(newPos);
                    setCell(newPos, ghostState);
                }
            }
        }
    }
    mostrarMapa();
}

void Map::moveClyde() {
    std::random_device rd;  // Semilla aleatoria del sistema
    std::mt19937 gen(rd()); // Mersenne Twister como generador

    // Distribución uniforme de enteros entre -1 y 1
    std::uniform_int_distribution<int> distrib(-1, 1);

    Position newPos(clyde.getPosition().x + distrib(gen), clyde.getPosition().y + distrib(gen));

    char ghostState;
    if(pacman.isPoweredUp()) { ghostState = '$';} else { ghostState = '4';}

    if (newPos == pacman.getPosition() && pacman.isPoweredUp()) {
        clyde.regenerate(14, 13);
        pacman.eatGhost();
    } else {
        if(newPos == pacman.getPosition() && !pacman.isPoweredUp()) {
            pacman.loseLife();
            clearCell(clyde.getPosition());
            clyde.setPosition(newPos);
            setCell(newPos, ghostState);
            pacman.setPosition(23, 13);
            setCell(pacman.getPosition(), '@');
        } else {
            if(isWall(newPos) || isGhost(newPos)) {
                mostrarMapa();
            } else {
                if(isPellet(newPos) && (clyde.getWasInPellet() || clyde.getWasInPowerPellet())) {
                    if (clyde.getWasInPellet()) {
                        setCell(clyde.getPosition(), '.');
                    } else {
                        setCell(clyde.getPosition(), 'O');
                        clyde.setWasInPowerPellet(false);
                        clyde.setWasInPellet(true);
                    }
                    clyde.setPosition(newPos);
                    setCell(newPos, ghostState);
                }
                if(isPowerPellet(newPos) && (clyde.getWasInPellet() || clyde.getWasInPowerPellet())) {
                    if (clyde.getWasInPellet()) {
                        setCell(clyde.getPosition(), '.');
                        clyde.setWasInPowerPellet(true);
                        clyde.setWasInPellet(false);
                    } else {
                        setCell(clyde.getPosition(), 'O');
                    }
                    clyde.setPosition(newPos);
                    setCell(newPos, ghostState);
                } else {
                    if(isPellet(newPos)) {
                        clyde.setWasInPellet(true);
                    }
                    if(isPowerPellet(newPos)) {
                        clyde.setWasInPowerPellet(true);
                    }
                    clearCell(clyde.getPosition());
                    clyde.setPosition(newPos);
                    setCell(newPos, ghostState);
                }
            }
        }
    }
    mostrarMapa();
}

Ghost& Map::fantasmaFactory(Position &pos, int x) {
    if(pos == blinky.getPosition() || x == 1) {
        blinky.setId('1');
        return blinky;
    } else if(pos == pinky.getPosition() || x == 2) {
        pinky.setId('2');
        return pinky;
    } else if (pos == inky.getPosition() || x == 3) {
        inky.setId('3');
        return inky;
    } else {
        clyde.setId('4');
        return clyde;
    }
}