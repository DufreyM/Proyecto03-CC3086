#include "FUNCS.h"
#include <stdio.h>

// Primer laberinto creado.
char maze[11][31] = {
    "##############################",
    "#............##............###",
    "#.####.#####.##.#####.####..##",
    "#.#  #.#   #.##.#   #.#  #..##",
    "#.#  #.#   #.##.#   #.#  #..##",
    "#.####.#####.##.#####.####..##",
    "#..........................###",
    "#.####.##.########.##.####..##",
    "#.####.##.########.##.####..##",
    "#............##............###",
    "##############################"
};

pthread_mutex_t mazeLock = PTHREAD_MUTEX_INITIALIZER;

// Función para mostrar el laberinto.
void printMaze(PacMan *pacman) {
    pthread_mutex_lock(&mazeLock);

    for (int i = 0; i < 11; i++) {
        printf("%s\n", maze[i]);
    }

    if (pacman != NULL) {
        printf("Score: %d\n", pacman->score);
    }

    pthread_mutex_unlock(&mazeLock);
}
