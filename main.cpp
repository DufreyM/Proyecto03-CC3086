#include "FUNCS.h" // Incluye las declaraciones
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 

void* pacmanThread(void* arg) {
    PacMan *pacman = (PacMan*) arg;
    char input;
    
    while (pacman->lives > 0) {
        printf("Ingrese la dirección de movimiento (w: arriba, s: abajo, a: izquierda, d: derecha): ");
        scanf(" %c", &input);
        
        pacManMovement(pacman, input);
        printf("\n");
        printMaze(pacman);
    }

    return NULL;
}

void* ghostThread(void* arg) {
    Ghost *ghost = (Ghost*) arg;
    while (ghost->lives > 0) {
        ghostMovement(ghost);
        printf("\n");
        printMaze(NULL);
        sleep(1);
    }

    return NULL;
}

int main() {
    srand(time(NULL));  // Inicializar srand() solo una vez.

    PacMan pacman;
    Ghost ghost1;
    
    initPacMan(&pacman, 1, 1); // Inicializamos a Pac-Man en una posición válida
    initGhost(&ghost1, 1, 25);

    printMaze(&pacman);

    pthread_t pacman_tid, ghost_tid;

    pthread_create(&pacman_tid, NULL, pacmanThread, &pacman);
    pthread_create(&ghost_tid, NULL, ghostThread, &ghost1);

    pthread_join(pacman_tid, NULL);
    pthread_join(ghost_tid, NULL);

    pthread_mutex_destroy(&mazeLock);  // Destruir mutex principal
    pthread_mutex_destroy(&pacman.lock);  // Destruir mutex de PacMan
    pthread_mutex_destroy(&ghost1.lock);  // Destruir mutex del fantasma

    return 0;
}
