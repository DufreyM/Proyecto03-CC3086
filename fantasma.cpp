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

void ghostMovement(Ghost *ghost, PacMan *pacman) {
    pthread_mutex_lock(&mazeLock);

    int bestX = ghost->x;
    int bestY = ghost->y;
    int minDistance = abs(ghost->x - pacman->x) + abs(ghost->y - pacman->y);

    // Movimientos posibles: arriba, abajo, izquierda, derecha
    int possibleMoves[4][2] = {
        {ghost->x - 1, ghost->y}, // Arriba
        {ghost->x + 1, ghost->y}, // Abajo
        {ghost->x, ghost->y - 1}, // Izquierda
        {ghost->x, ghost->y + 1}  // Derecha
    };

    // Revisar cada movimiento posible
    for (int i = 0; i < 4; i++) {
        int newX = possibleMoves[i][0];
        int newY = possibleMoves[i][1];

        if (isValidMove(newX, newY)) {
            // Calcular la distancia de la posición actual del fantasma a Pac-Man
            int distance = abs(newX - pacman->x) + abs(newY - pacman->y);

            // Si esta nueva posición nos acerca más a Pac-Man, la seleccionamos
            if (distance < minDistance) {
                bestX = newX;
                bestY = newY;
                minDistance = distance;
            }
        }
    }

    // Actualizar la posición del fantasma si encontró un mejor movimiento
    maze[ghost->x][ghost->y] = ghost->prevChar;  // Restaurar el carácter anterior en la posición anterior del fantasma
    ghost->x = bestX;
    ghost->y = bestY;
    ghost->prevChar = maze[bestX][bestY];  // Guardar lo que había en la nueva posición
    maze[ghost->x][ghost->y] = ghost->symbol;  // Colocar al fantasma en la nueva posición

    pthread_mutex_unlock(&mazeLock);
}
