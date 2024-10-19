#include "FUNCS.h"  // Asegúrate de tener la declaración correcta
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Función para inicializar Pac-Man
void initPacMan(PacMan *pacman, int startX, int startY) {
    pacman-> x = startX;
    pacman-> y = startY;
    pacman-> lives = 3;
    pacman-> score = 0;
    pacman-> symbol = '@';
    maze[pacman -> x][pacman -> y] = pacman->symbol;
    pthread_mutex_init(&pacman->lock, NULL);
}


// Verifica si el movimiento es válido
int isValidMove(int newX, int newY) {
    // Asegurarse de que los límites del laberinto no se sobrepasen
    if (newX >= 0 && newX < 11 && newY >= 0 && newY < 31) {
        return (maze[newX][newY] != '#');  // Verificar que no sea una pared
    }
    return 0;  // Movimiento inválido
}

// Mueve a Pac-Man según la entrada
void pacManMovement(PacMan *pacman, char direction) {
    pthread_mutex_lock(&pacman->lock);

    int newX = pacman->x;
    int newY = pacman->y;

    switch (direction) {
        case 'w': newX--; break;  // Arriba
        case 's': newX++; break;  // Abajo
        case 'a': newY--; break;  // Izquierda
        case 'd': newY++; break;  // Derecha
        default: 
            printf("Entrada no válida.\n"); 
            pthread_mutex_unlock(&pacman->lock);
            return;
    }

    if (isValidMove(newX, newY)) {
        // Actualizar posición de Pac-Man en el laberinto
        maze[pacman->x][pacman->y] = ' ';  // Borra la posición anterior
        pacman-> x = newX;
        pacman-> y = newY;
        maze[pacman->x][pacman->y] = pacman->symbol;  // Coloca a Pac-Man en la nueva posición
    }

    pthread_mutex_unlock(&pacman->lock);
}
