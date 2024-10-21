#include "FUNCS.h" // Incluye las declaraciones
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 


// Función para mostrar el menú y elegir el modo de juego
int showMenu() {
    int choice;
    printf("Seleccione el modo de juego:\n");
    printf("1. Un jugador (Pac-Man vs IA Fantasma)\n");
    printf("2. Dos jugadores (Jugador 1 controla Pac-Man, Jugador 2 controla Fantasma)\n");
    printf("Ingrese su opción (1 o 2): ");
    scanf("%d", &choice);
    return choice;
}

// Función del hilo para Pac-Man
void* pacmanThread(void* arg) {
    PacMan *pacman = (PacMan*) arg;
    char input;
    
    while (pacman->lives > 0) {
        printf("Jugador 1 - Ingrese la dirección de movimiento (w: arriba, s: abajo, a: izquierda, d: derecha): ");
        scanf(" %c", &input);
        
        pacManMovement(pacman, input);
        printf("\n");
        printMaze(pacman);
    }

    return NULL;
}

// Función del hilo para el fantasma controlado por IA
void* ghostThreadAI(void* arg) {
    Ghost *ghost = ((Ghost*)(((void**)arg)[0]));
    PacMan *pacman = ((PacMan*)(((void**)arg)[1]));
    
    while (ghost->lives > 0) {
        ghostMovement(ghost, pacman);  // Movimiento automático del fantasma
        printf("\n");
        printMaze(NULL);
        sleep(1);  // El fantasma se mueve cada 1 segundo
    }

    return NULL;
}

// Función del hilo para el fantasma controlado por el jugador 2
void* ghostThreadPlayer(void* arg) {
    Ghost *ghost = (Ghost*) arg;
    char input;

    while (ghost->lives > 0) {
        printf("Jugador 2 - Ingrese la dirección de movimiento del fantasma (i: arriba, k: abajo, j: izquierda, l: derecha): ");
        scanf(" %c", &input);
        
        pthread_mutex_lock(&mazeLock);

        int newX = ghost->x;
        int newY = ghost->y;

        switch (input) {
            case 'i': newX--; break;    // Arriba
            case 'k': newX++; break;    // Abajo
            case 'j': newY--; break;    // Izquierda
            case 'l': newY++; break;    // Derecha
            default:
                printf("Entrada no válida.\n");
                pthread_mutex_unlock(&mazeLock);
                continue;
        }

        if (isValidMove(newX, newY)) {
            maze[ghost->x][ghost->y] = ghost->prevChar;  // Restaurar carácter anterior
            ghost->x = newX;
            ghost->y = newY;
            ghost->prevChar = maze[newX][newY];  // Guardar lo que había en la nueva posición
            maze[ghost->x][ghost->y] = ghost->symbol;  // Colocar al fantasma
        }

        pthread_mutex_unlock(&mazeLock);
        printMaze(NULL);  // Mostrar laberinto después del movimiento
    }

    return NULL;
}

int main() {
    srand(time(NULL));  // Inicializar la semilla para rand()

    PacMan pacman;
    Ghost ghost1;
    
    initPacMan(&pacman, 1, 1);  // Inicializar Pac-Man en una posición válida
    initGhost(&ghost1, 1, 25);  // Inicializar fantasma en una posición válida

    printMaze(&pacman);

    int choice = showMenu();  // Mostrar menú y seleccionar modo de juego

    pthread_t pacman_tid, ghost_tid;

    if (choice == 1) {
        // Modo de un jugador: Pac-Man vs IA Fantasma
        printf("Modo de juego: Un jugador.\n");
        
        pthread_create(&pacman_tid, NULL, pacmanThread, &pacman);
        void *args[] = {&ghost1, &pacman};  // Pasar ambos punteros a la IA
        pthread_create(&ghost_tid, NULL, ghostThreadAI, args);
        
    } else if (choice == 2) {
        // Modo de dos jugadores: Pac-Man controlado por Jugador 1, Fantasma controlado por Jugador 2
        printf("Modo de juego: Dos jugadores.\n");

        pthread_create(&pacman_tid, NULL, pacmanThread, &pacman);
        pthread_create(&ghost_tid, NULL, ghostThreadPlayer, &ghost1);  // Fantasma controlado por Jugador 2
    } else {
        printf("Opción no válida. Saliendo...\n");
        exit(1);
    }

    pthread_join(pacman_tid, NULL);
    pthread_join(ghost_tid, NULL);

    pthread_mutex_destroy(&mazeLock);  // Destruir mutex principal
    pthread_mutex_destroy(&pacman.lock);  // Destruir mutex de PacMan
    pthread_mutex_destroy(&ghost1.lock);  // Destruir mutex del fantasma

    return 0;
}
