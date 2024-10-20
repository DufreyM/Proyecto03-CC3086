#include "FUNCS.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void initGhost(Ghost *ghost, int startX, int startY) {
    ghost->x = startX;
    ghost->y = startY;
    ghost->lives = 1;
    ghost->symbol = '$';
    ghost->prevChar = maze[startX][startY];
    pthread_mutex_init(&ghost->lock, NULL);  // Inicializa el mutex para el fantasma
}

void ghostMovement(Ghost *ghost) {
    pthread_mutex_lock(&mazeLock);

    int movement = rand() % 4;  // rand() se puede usar directamente, no hace falta srand aquí.
    int newX = ghost->x;
    int newY = ghost->y;

    // En esta sección, el fantasma buscará hacia dónde moverse.
    switch (movement) {
        case 0: newX--; break;
        case 1: newX++; break;
        case 2: newY--; break;
        case 3: newY++; break;
    }

    if (isValidMove(newX, newY)) {
        maze[ghost->x][ghost->y] = ghost->prevChar;

        ghost->x = newX;
        ghost->y = newY;

        ghost->prevChar = maze[newX][newY];

        maze[ghost->x][ghost->y] = ghost->symbol;
    }

    pthread_mutex_unlock(&mazeLock);
}
