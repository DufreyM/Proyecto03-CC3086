#include "FUNCS.h"  // Incluye las declaraciones
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    PacMan pacman;
    initPacMan(&pacman, 1, 1); // Inicializamos a Pac-Man en una posición válida

    char input;
    while (pacman.lives > 0) {
        printMaze();
        printf("Ingrese la dirección de movimiento (w: arriba, s: abajo, a: izquierda, d: derecha): ");
        scanf(" %c", &input);
        pacManMovement(&pacman, input);
    }

    pthread_mutex_destroy(&pacman.lock); // Destruye el mutex al final
    return 0;
}
