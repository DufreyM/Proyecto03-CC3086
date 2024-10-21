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

    int movement = rand() % 4;
    int newX = ghost->x;
    int newY = ghost->y;

    switch (movement) {
        case 0: newX--; break;
        case 1: newX++; break;
        case 2: newY--; break;
        case 3: newY++; break;
    }

    if (isValidMove(newX, newY)) {
        // Verifica si el fantasma colisiona con Pac-Man
        if (maze[newX][newY] == '@') {  // Supone que Pac-Man tiene el símbolo '@'
            printf("¡El fantasma ha atrapado a Pac-Man!\n");
            pthread_mutex_unlock(&mazeLock);
            return;
        } else {
            maze[ghost->x][ghost->y] = ghost->prevChar;  // Deja lo que estaba antes
            ghost->x = newX;
            ghost->y = newY;
            ghost->prevChar = maze[newX][newY];
            maze[ghost->x][ghost->y] = ghost->symbol;
        }
    }

    pthread_mutex_unlock(&mazeLock);
}
